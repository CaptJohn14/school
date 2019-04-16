//used heap size of 4096.

// **** Include libraries here ****
// Standard C libraries


//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>
#include <string.h>
#include <stdio.h>

// User libraries
#include "Buttons.h"
#include "Morse.h"
#include "Oled.h"
#include "OledDriver.h"

// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static MorseEvent event;
//static MorseEvent morseE;
//static int myinterrupt; //using as interrupt flag. 
static char display[128];
//static char top[32];
//static char bot[64];
static char thechar;
static int botcounter;
static int topcounter;

// **** Declare any function prototypes here ****
static void OledClearTop(void);
static void OledCharToTop(char x);
static void OledCharToBot(char x);
//static void OledPrint(void);

// **** Declare any function prototypes here ****

int main() {
    BOARD_Init();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    //printf("Welcome to the CMPE13 Lab8 blank. Please remove this line.");

    //myinterrupt = FALSE;
    MorseInit();
    OledInit();
    botcounter = 0;
    topcounter = 0;
    //ButtonsInit();
    //char thechar;
    //bot[0] = '\n';
    if (MorseInit() == STANDARD_ERROR) {
        OledClear(OLED_COLOR_WHITE);
        OledDrawString("oops. morseinit fail.");
        OledUpdate();
        FATAL_ERROR();
        //    } else {
        //        OledClear(OLED_COLOR_BLACK);
        //        OledDrawString("ah yea, morseinit work.");
        //        OledUpdate();
        //    }/
    }
    //printf("error checkdlsaflkjfsjfklasjfjlkasjflasjfaslkfsaklfjlsa");
    event = MORSE_EVENT_NONE;

    while (1) {

        switch (event) {
            case MORSE_EVENT_DOT:
                MorseDecode(MORSE_CHAR_DOT); //goes left
                thechar = MORSE_CHAR_DOT; //print dot
                OledCharToTop(thechar);
                event = MORSE_EVENT_NONE;
                break;
            case MORSE_EVENT_DASH:
                MorseDecode(MORSE_CHAR_DASH); //goes right
                thechar = MORSE_CHAR_DASH; //print dash
                OledCharToTop(thechar);
                event = MORSE_EVENT_NONE;
                break;
            case MORSE_EVENT_INTER_LETTER:
                //OledClearTop();
                //                thechar = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                thechar = MorseDecode(MORSE_CHAR_END_OF_CHAR); //print decoded char
                OledCharToBot(thechar);
                event = MORSE_EVENT_NONE;
                break;
            case MORSE_EVENT_INTER_WORD:
                //OledClearTop();
                thechar = MorseDecode(MORSE_CHAR_END_OF_CHAR); //print decoded char
                OledCharToBot(thechar);
                thechar = ' '; //add a space
                OledCharToBot(thechar);
                event = MORSE_EVENT_NONE;
                break;
            case MORSE_EVENT_NONE:
                break;
        }
        //myinterrupt = FALSE;
    }

    //thechar = 'S';
    //    while (TRUE) {
    //        if (myinterrupt == TRUE) {
    //            if (event == MORSE_EVENT_DOT) {
    //                MorseDecode(MORSE_CHAR_DOT); //will return leftchild
    //                OledCharToTop();
    //
    //            } else if (event == MORSE_EVENT_DASH) {
    //                MorseDecode(MORSE_CHAR_DASH);
    //                OledCharToTop();
    //
    //            } else if (event == MORSE_EVENT_INTER_LETTER) {
    //                OledClearTop();
    //                thechar = MorseDecode(MORSE_CHAR_END_OF_CHAR); //now holds the decoded char
    //                //printf("%c",thechar);
    //                //sprintf(display + strlen(display), MORSE_CHAR_END_OF_CHAR);
    //                //printf(thechar);
    //                printf("plzwork%c", MorseDecode(MORSE_CHAR_END_OF_CHAR));
    //                OledCharToBot();
    //
    //            } else if (event == MORSE_EVENT_INTER_WORD) {
    //                OledClearTop();
    //                MorseDecode(MORSE_CHAR_DECODE_RESET);
    //
    //            }
    //            myinterrupt = FALSE;
    //        }
    //    }


    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}

void OledClearTop(void) {
    //    memset(display, 0, sizeof (OLED_CHARS_PER_LINE)); //clears the dots and dashes
    //    //memset(display, 0, sizeof (display));
    //    //sprintf(display, "%s\n%s", top, bot);
    //    //printf(display); //just for testing on coolterms
    //    OledDrawString(display);
    //    OledUpdate();
    int i;
    for (i = 0; i < OLED_CHARS_PER_LINE; i++) { //oledcharsperline is defined in oled.h
        display[i] = ' ';
    }
    topcounter = 0; //reset counter for how many chars in top
    OledDrawString(display);
    OledUpdate();
}

void OledCharToTop(char x) {
    //    if (event == MORSE_EVENT_DOT) {
    //        display[strlen(display)] = MORSE_CHAR_DOT;
    //    } else if (event == MORSE_EVENT_DASH) {
    //        //        sprintf(top+strlen[top], "%c", MORSE_CHAR_DASH);
    //        display[strlen(display)] = MORSE_CHAR_DASH;
    //    }
    //    //sprintf(display, "%s\n%s", top, bot);
    //    //printf(display); //just for testing on coolterms
    //    OledDrawString(display);
    //    OledUpdate();
    //    if (event == MORSE_EVENT_DOT) {
    //        display[topcounter] = MORSE_CHAR_DOT;
    //        topcounter++;
    //    } else if (event == MORSE_EVENT_DASH) {
    //        display[topcounter] = MORSE_CHAR_DASH;
    //        topcounter++;
    //    }
    display[topcounter] = x; //places either dot or dash in x
    topcounter++; //number of chars in top
    OledDrawString(display);
    OledUpdate();

}

void OledCharToBot(char x) {
    //bot[0] = '\n';
    //    OledClearTop();
    //    if (thechar == '\0') {
    //        thechar = '#';
    //    }
    //    //sprintf(bot + strlen(bot), "%c", thechar);
    //    //bot[strlen(bot)] = thechar;
    //   // sprintf(display, "%s\n%s", top, bot);
    //    //printf(display); //just for testing on coolterms
    //    display[OLED_CHARS_PER_LINE + counter] = thechar;
    //    counter++;
    //    OledDrawString(display);
    //    OledUpdate();
    if (x == '\0') {
        x = '#';
    }
    display[OLED_CHARS_PER_LINE + botcounter] = x; //prints char to bot
    botcounter++; //number of chars in bot
    OledClearTop();
    OledDrawString(display);
    OledUpdate();
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//
    event = MorseCheckEvents();
    //    if (event != MORSE_EVENT_NONE){
    //        myinterrupt = TRUE;
    //}
}