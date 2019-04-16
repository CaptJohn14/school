// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Buttons.h"
#include "Adc.h"
#include "Ascii.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>



// **** Set any macros or preprocessor directives here ****
#define LONG_PRESS 5

#define TIME 0      //time and temp will be used for inputselection
#define TEMP 1

// Set a macro for resetting the timer, makes the code a little clearer.

#define TIMER_2HZ_RESET() (TMR1 = 0)    //given? (:

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****

//using enums as well as macro to get better with both(: I think I prefer enums
//easier to keep organized?

typedef enum { //this is for the state machine. 
    START,
    COUNTDOWN,
    PENDING_RESET,
    PENDING_SELECTOR_CHANGE,
    RESET
} kitchenstate;

typedef enum { //for cookmode. will be used in following struct.
    BAKE,
    TOAST,
    BROIL
} kitchenmode;

typedef struct {
    int initial_time; //initial cook time in seconds
    int timeleft; //time left from cook time in seconds
    int temperature; //temperature in Fahrenheit
    kitchenmode cookmode; //bake,broil,toast. created an separate enum.
    kitchenstate state; //state of machine
    int btncounter; //button counter for inputs.
    int input; //switches between time or temp. will make 0 for time. 1 for temp
} mykitchen;

//------------------------
static mykitchen kitchen; //not sure if supposed to static this. doesn't hut >:D
static ButtonEventFlags btnevent; //will be used for ButtonsCheckEvents()

static bool hz2;
static bool hz5;
//static char display[128];

void OledPrint(void);
void LedCook(void);

// Configuration Bit settings

int main() {
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    OledInit(); //initializing oled
    LEDS_INIT(); //initializing leds
    ButtonsInit(); //initializing buttons
    AdcInit(); //initialized adc (analog to digital convertor). always forget what it stands for

    //initializing my kitchen
    kitchen.initial_time = 1; //defaulted at 1 second
    kitchen.timeleft = kitchen.initial_time;
    //kitchen.temperature = 350; //defaulted at 350 Fahrenheit
    kitchen.cookmode = BAKE; //starts up at BAKE.
    //kitchen.input = TIME; //selector starts at time.
    kitchen.btncounter = 0;
    kitchen.state = RESET;

    //int initialtime;
    //int freetime;

    while (TRUE) {
        switch (kitchen.state) {
            case RESET:
                kitchen.btncounter = 0;
                btnevent = BUTTON_EVENT_NONE;
                kitchen.input = TIME; //set input selection for time
                kitchen.temperature = 350; //reset temp to default
                kitchen.state = START; //turn oven off
                OledPrint(); //update the display
                LEDS_SET(0x00); //turn off all LED
                break;
            case START:
                if (AdcChanged()) {//if potentiometer changed
                    if (kitchen.input == TIME) {
                        kitchen.initial_time = (AdcRead() >> 2) + 1; //update time
                        kitchen.timeleft = kitchen.initial_time;
                    } else {
                        kitchen.temperature = (AdcRead() >> 2) + 300; //update temp
                    }
                    OledPrint(); //update the display
                    kitchen.state = START; //still at state START
                }
                if (btnevent == BUTTON_EVENT_4DOWN) {//if button 4 pressed
                    TIMER_2HZ_RESET(); //reset 2hz timer
                    hz2 = FALSE;
                    //initialtime = kitchen.initial_time;
                    kitchen.initial_time = kitchen.initial_time; //save initial start time
                    kitchen.state = COUNTDOWN; //turn oven on
                    btnevent = BUTTON_EVENT_NONE; //clear button event
                    OledPrint(); //update the display
                }
                if (btnevent == BUTTON_EVENT_3DOWN) {
                    kitchen.btncounter = 0; //clear how long pressing button.
                    btnevent = BUTTON_EVENT_NONE;
                    kitchen.state = PENDING_SELECTOR_CHANGE;
                }
                break;
            case COUNTDOWN:
                if (btnevent == BUTTON_EVENT_4DOWN) {
                    kitchen.btncounter = 0; //does this still increment while held?
                    btnevent = BUTTON_EVENT_NONE;
                    hz5 = FALSE;
                    kitchen.state = PENDING_RESET;
                } else if (hz2 == TRUE && kitchen.timeleft > 0) {//2hz timer triggered && cooking time left>0
                    //kitchen.timeleft -= 1; //cooking time decrement
                    //LedCook(); //update LEDS
                    OledPrint();
                    LedCook();
                    kitchen.timeleft -= 1; //cooking time decrement
                    //LedCook(); //reupdating to get time == 0 to update
                    //OledPrint(); //update display
                    hz2 = FALSE; //clear 2hz timer flag
                    kitchen.state = COUNTDOWN;
                } else if (hz2 == TRUE && kitchen.timeleft == 0) { //beepbeepbeep. cook is finished.
                    OledPrint();
                    LEDS_SET(0x00);
                    hz2 = FALSE;
                    kitchen.state = RESET; //go to reset to start over.
                }
                break;
            case PENDING_RESET:
                if (btnevent == BUTTON_EVENT_4UP && kitchen.btncounter < LONG_PRESS) {
                    kitchen.btncounter = 0; //getting ready to check how long is pressed.
                    btnevent = BUTTON_EVENT_NONE;
                    kitchen.state = COUNTDOWN;
                    //as soon as button 4 is released, goes back to reset.
                } else if (btnevent == BUTTON_EVENT_4UP && kitchen.btncounter >= LONG_PRESS) {
                    kitchen.state = RESET;
                }
                if (hz2 == TRUE && kitchen.timeleft > 0) { //basically countdown continues till release button.
                    LedCook(); //update LEDS
                    OledPrint();
                    kitchen.timeleft -= 1; //cooking time decrement
                    hz2 = FALSE; //clear 2hz timer flag
                    kitchen.state = PENDING_RESET; // supposed to go back into pending_reset but.. countdown does same
                }
                break;
            case PENDING_SELECTOR_CHANGE:
                if (kitchen.btncounter < LONG_PRESS && btnevent == BUTTON_EVENT_3UP) {
                    kitchen.initial_time = 1;
                    kitchen.timeleft = 1;
                    kitchen.initial_time = TIME;
                    if (kitchen.cookmode == BAKE) {
                        kitchen.cookmode = TOAST;
                    } else if (kitchen.cookmode == TOAST) {
                        kitchen.cookmode = BROIL;
                    } else {
                        kitchen.cookmode = BAKE;
                    }
                    OledPrint();
                    kitchen.btncounter = 0;
                    btnevent = BUTTON_EVENT_NONE;
                    kitchen.state = START;
                } else if (kitchen.btncounter >= LONG_PRESS) {
                    if (kitchen.input == TIME) {
                        kitchen.input = TEMP;
                    } else {
                        kitchen.input = TIME;
                    }
                    btnevent = BUTTON_EVENT_NONE;
                    kitchen.state = START;
                }
                break;
        }
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

void OledPrint(void) {
    char display[128];
    OledClear(OLED_COLOR_BLACK);
    if (kitchen.cookmode == BAKE && kitchen.state != COUNTDOWN && kitchen.state != PENDING_RESET) {
        sprintf(display, "|%c%c%c%c%c|  Mode: Bake\n", 0x02, 0x02, 0x02, 0x02, 0x02);
        if (kitchen.input == TIME) {
            sprintf(display + strlen(display), "|     | >Time: %1d:%02d\n", kitchen.timeleft / 60, kitchen.timeleft % 60); //adding onto print[]
            sprintf(display + strlen(display), "|-----|  Temp: %3d%cF\n", kitchen.temperature, 0xF8);
        } else if (kitchen.input == TEMP) {
            sprintf(display + strlen(display), "|     |  Time: %1d:%02d\n", kitchen.timeleft / 60, kitchen.timeleft % 60);
            sprintf(display + strlen(display), "|-----| >Temp: %3d%cF\n", kitchen.temperature, 0xF8);
        }
        sprintf(display + strlen(display), "|%c%c%c%c%c|", 0x04, 0x04, 0x04, 0x04, 0x04);
        OledDrawString(display);
        OledUpdate();
        printf("Baking is off\n");

    } else if (kitchen.cookmode == BAKE && (kitchen.state == COUNTDOWN || kitchen.state == PENDING_RESET)) {
        sprintf(display, "|%c%c%c%c%c|  Mode: Bake\n", 0x01, 0x01, 0x01, 0x01, 0x01);
        sprintf(display + strlen(display), "|     |  Time: %1d:%02d\n", kitchen.timeleft / 60, kitchen.timeleft % 60);
        sprintf(display + strlen(display), "|-----|  Temp: %3d%cF\n", kitchen.temperature, 0xF8);
        sprintf(display + strlen(display), "|%c%c%c%c%c|", 0x03, 0x03, 0x03, 0x03, 0x03);
        OledDrawString(display);
        OledUpdate();

    } else if (kitchen.cookmode == TOAST) {
        sprintf(display, "|%c%c%c%c%c|  Mode: Toast\n", 0x02, 0x02, 0x02, 0x02, 0x02);
        sprintf(display + strlen(display), "|     |  Time: %1d:%02d\n", kitchen.timeleft / 60, kitchen.timeleft % 60);
        sprintf(display + strlen(display), "|-----|\n");
        if (kitchen.state != COUNTDOWN || kitchen.state != PENDING_RESET) {
            sprintf(display + strlen(display), "|%c%c%c%c%c|", 0x04, 0x04, 0x04, 0x04, 0x04);
        } else {
            sprintf(display + strlen(display), "|%c%c%c%c%c|", 0x03, 0x03, 0x03, 0x03, 0x03);
        }
        OledDrawString(display);
        OledUpdate();

    } else if (kitchen.cookmode == BROIL) {
        //kitchen.temperature = 500;  //put this here just incase. or just print 500.. lol
        if (kitchen.state != COUNTDOWN || kitchen.btncounter != PENDING_RESET) {
            sprintf(display, "|%c%c%c%c%c|  Mode: Broil\n", 0x02, 0x02, 0x02, 0x02, 0x02);
        } else {
            sprintf(display, "|%c%c%c%c%c|  Mode: Broil\n", 0x01, 0x01, 0x01, 0x01, 0x01);
        }
        sprintf(display + strlen(display), "|     |  Time: %1d:%02d\n", kitchen.timeleft / 60, kitchen.timeleft % 60);
        sprintf(display + strlen(display), "|-----|  Temp: 500%cF\n", 0xF8);
        sprintf(display + strlen(display), "|%c%c%c%c%c|", 0x04, 0x04, 0x04, 0x04, 0x04);
        OledDrawString(display);
        OledUpdate();
    } else {
        OledClear(OLED_COLOR_WHITE);
        OledDrawString("broken..dsaklnei");
        OledUpdate();
    }
}

void LedCook(void) {
    //    if (kitchen.timeleft == 0) {
    //        LEDS_SET(0x00);
    //    } else if (kitchen.timeleft <= 1 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0x80);
    //    } else if (kitchen.timeleft <= 2 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0xC0);
    //    } else if (kitchen.timeleft <= 3 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0xE0);
    //    } else if (kitchen.timeleft <= 4 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0xF0);
    //    } else if (kitchen.timeleft <= 5 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0xF8);
    //    } else if (kitchen.timeleft <= 6 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0xFC);
    //    } else if (kitchen.timeleft <= 7 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0xFE);
    //    } else if (kitchen.timeleft <= 8 * (kitchen.initial_time / 8)) {
    //        LEDS_SET(0xFF);
    //    }
    int temp = (int) ((kitchen.timeleft * 8 / kitchen.initial_time) + 1);
    if (temp == 8) {
        LEDS_SET(0xFF);
    } else if (temp == 7) {
        LEDS_SET(0xFE);
    } else if (temp == 6) {
        LEDS_SET(0xFC);
    } else if (temp == 5) {
        LEDS_SET(0xF8);
    } else if (temp == 4) {
        LEDS_SET(0xF0);
    } else if (temp == 3) {
        LEDS_SET(0xE0);
    } else if (temp == 2) {
        LEDS_SET(0xC0);
    } else if (temp == 1) {
        LEDS_SET(0x80);
    } else if (temp == 0) {
        LEDS_SET(0x00);
    }
}

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void) {
    // Clear the interrupt flag.

    IFS0CLR = 1 << 4;
    hz2 = TRUE;

}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.

    IFS0CLR = 1 << 12;
    hz5 = TRUE;
    kitchen.btncounter++;

}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    btnevent = ButtonsCheckEvents(); //100hz used for checking button event
    //makes more responsive.

}