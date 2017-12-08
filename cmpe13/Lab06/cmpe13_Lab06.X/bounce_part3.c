// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Buttons.h"
#include "Leds.h"
// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

typedef struct BtnEvent {
    uint8_t event;
    uint16_t value;
} BtnEvent;

// **** Define global, module-level, or external variables here ****
BtnEvent eventData;

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    //printf("Welcome to the Lab 6 Part 3 blank. Please remove before starting.");

    eventData.event = FALSE;
    eventData.value = 0;
    LEDS_INIT();
    ButtonsInit();
    int switchstore;

    while (TRUE) {
        if (eventData.event == TRUE) {
            switchstore = SWITCH_STATES(); //manual says to do this but don't need?
            if ((SWITCH_STATES() & SWITCH_STATE_SW1) && (eventData.value == BUTTON_EVENT_1UP)) {
                if ((LEDS_GET() & 0x03) == 0x03) {          //check if LEDS1&2 are on
                    LEDS_SET(LEDS_GET() & 0xFC);            //if on, turn off
                } else if ((LEDS_GET() & 0x03) != 0x03) {   //checks if LEDS1&2 are BOTH off
                    LEDS_SET(LEDS_GET() | 0x03);            //if off, turn on
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW1) && (eventData.value == BUTTON_EVENT_1DOWN)) {
                if ((LEDS_GET() & 0x03) == 0x03) {
                    LEDS_SET(LEDS_GET() & 0xFC);
                } else if ((LEDS_GET() & 0x03) != 0x03) {
                    LEDS_SET(LEDS_GET() | 0x03);
                }
            }
            if ((SWITCH_STATES() & SWITCH_STATE_SW2) && (eventData.value == BUTTON_EVENT_2UP)) {
                if ((LEDS_GET() & 0x0C) == 0x0C) {
                    LEDS_SET(LEDS_GET() &0xF3);
                } else if ((LEDS_GET() & 0x0C) != 0x0C) {
                    LEDS_SET(LEDS_GET() | 0x0C);
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW2) && (eventData.value == BUTTON_EVENT_2DOWN)) {
                if ((LEDS_GET() & 0x0C) == 0x0C) {
                    LEDS_SET(LEDS_GET() &0xF3);
                } else if ((LEDS_GET() & 0x0C) != 0x0C) {
                    LEDS_SET(LEDS_GET() | 0x0C);
                }
            }
            if ((SWITCH_STATES() & SWITCH_STATE_SW3) && (eventData.value == BUTTON_EVENT_3UP)) {
                if ((LEDS_GET() & 0x30) == 0x30) {
                    LEDS_SET(LEDS_GET() &0xCF);
                } else if ((LEDS_GET() & 0x30) != 0x30) {
                    LEDS_SET(LEDS_GET() | 0x30);
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW3) && (eventData.value == BUTTON_EVENT_3DOWN)) {
                if ((LEDS_GET() & 0x30) == 0x30) {
                    LEDS_SET(LEDS_GET() &0xCF);
                } else if ((LEDS_GET() & 0x30) != 0x30) {
                    LEDS_SET(LEDS_GET() | 0x30);
                }
            }
            if ((SWITCH_STATES() & SWITCH_STATE_SW4) && (eventData.value == BUTTON_EVENT_4UP)) {
                if ((LEDS_GET() & 0xC0) == 0xC0) {
                    LEDS_SET(LEDS_GET() &0x3F);
                } else if ((LEDS_GET() & 0xC0) != 0xC0) {
                    LEDS_SET(LEDS_GET() | 0xC0);
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW4) && (eventData.value == BUTTON_EVENT_4DOWN)) {
                if ((LEDS_GET() & 0xC0) == 0xC0) {
                    LEDS_SET(LEDS_GET() &0x3F);
                } else if ((LEDS_GET() & 0xC0) != 0xC0) {
                    LEDS_SET(LEDS_GET() | 0xC0);
                }
            }
            eventData.event = FALSE;
        }

    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void) {
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    eventData.value = ButtonsCheckEvents();
    if (eventData.value) {
        eventData.event = TRUE;
    }
}