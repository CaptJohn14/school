// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

typedef struct TimerResult {
    uint8_t event;
    uint8_t value;
} TimerResult;

// **** Define global, module-level, or external variables here ****
TimerResult eventData;
#define LEFT 1
#define RIGHT 0
//#define TRUE 1
//#define FALSE 0
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
    //printf("Welcome to the Lab 6 Part 1 blank. Please remove before starting.");
    //my testing code to get a 'feel' for it. )
    //    LEDS_INIT();
    //    LEDS_SET(0xCC);
    //    int test;
    //    while (TRUE) {
    //        for (test = 0; test < 10000000; ++test) {
    //            LEDS_SET(0xDD);
    //        }
    //        for (test = 0; test < 10000000; ++test) {
    //            LEDS_SET(0x00);
    //        }
    //        for (test = 0; test < 10000000; ++test) {
    //            LEDS_SET(0xFF);
    //        }
    //    }
    /*code for lab attempt 1. failed..b*/

    //    LEDS_INIT();
    //    eventFlag.event = 0;
    //    eventFlag.value = 0;
    //    //int i;
    //    LEDS_SET(0x01);
    //    int direction = LEFT; //start left first
    //    while (TRUE) {
    //        if (eventFlag.event == TRUE) {
    //            if (direction == LEFT) {
    //                LEDS_SET(LEDS_GET() << 1);
    //            } else if(direction == RIGHT) {
    //                LEDS_SET(LEDS_GET() >> 1);
    //            }
    //            if (LEDS_GET() == 0x01) {
    //                direction == LEFT;
    //            } else if (LEDS_GET() == 0x80) {
    //                direction == RIGHT;
    //            }
    //            eventFlag.event = FALSE;
    //        }
    //    }

    /*code is here*/
    eventData.value = 0;
    eventData.event = FALSE;
    LEDS_INIT();
    // Main event-based program loop
    LEDS_SET(0x01);         //initializing a start position can be anywhere but 0x80
    int direction = LEFT;   //gonna start going left first.
    while (TRUE) {
        if (eventData.event == TRUE) {
            // Bounce LEDs
            if (direction == LEFT) {
                if (LEDS_GET() == 0x80) {
                    direction = RIGHT;
                    LEDS_SET(0x40);     //same as if LEDS_SET(LEDS_GET()<<1)
                } else {
                    LEDS_SET(LEDS_GET() << 1);  //shift LED left 
                }
            } else {
                if (LEDS_GET() == 0x01) {
                    direction = LEFT;
                    LEDS_SET(0x02);
                } else {
                    LEDS_SET(LEDS_GET() >> 1);  //shift LED right
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
 * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
 * value member of a module-level TimerResult struct). This counter is then checked against the
 * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
 * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
 * module-level TimerResult struct is set to true and the value member is cleared.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void) {
    //Increment the value member of your struct TimerResult variable by 1
    eventData.value++;
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    /*If value ends up being greater than the value returned by the
    SWITCH_STATES() macro, set the event member to true. Also reset
    value to 0 if that is the case*/
    if (eventData.value > SWITCH_STATES()) {
        eventData.event = TRUE;
        eventData.value = 0;
    }
}