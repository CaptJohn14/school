
/*==============================================================================
 * Name: Johnson Le
 * Date: 1/10/2019
 * Desc: Main for lab 0.
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "BOARD.h"
#include "AD.h"
#include "ToneGeneration.h"
#include "Oled.h"
#include <stdio.h>
#include <xc.h>

/*==============================================================================
 * Defines/macros
 =============================================================================*/
#define PART4and5
//#define BTN_TEST

int k;
#define DELAY(x) for(k=0; k<x; k++)

/*==============================================================================
 * Main
 =============================================================================*/
int main() {
    /*==========================================================================
     * Initialization
     =========================================================================*/
    BOARD_Init();
    AD_Init();
    ToneGeneration_Init();
    OledInit();
    char string[40];

    char ErrorCheck;
    ErrorCheck = AD_AddPins(AD_A0);
    if (!ErrorCheck) {
        printf("ERROR! \r\n");
        while (1);
    }

#ifdef PART2
    /*==========================================================================
     * Part2
     =========================================================================*/
    unsigned int POT_value;
    while (1) {
        POT_value = AD_ReadADPin(AD_A0);
        printf("AD Value: %u\r\n", POT_value);
        DELAY(500000);
    }
#endif
#ifdef PART3
    /*==========================================================================
     * Part3
     =========================================================================*/
    ToneGeneration_SetFrequency(DEFAULT_TONE);
    ToneGeneration_ToneOn();
    while (1);
#endif
#ifdef PART4and5
    /*==========================================================================
     * Part4 and part 5
     =========================================================================*/
    ToneGeneration_ToneOn();
    unsigned int POT_old = 0;
    unsigned int POT_curr;
    while (1) {
        POT_curr = AD_ReadADPin(AD_A0);
        if (POT_curr < 10) {
            POT_old = 0;
        } else if (POT_curr > 1013) {
            POT_old = 1023;
        } else if (POT_curr > POT_old + 6 || POT_curr < POT_old - 6) {
            POT_old = POT_curr;
            printf("AD Value: %u\r\n", POT_curr);
        }
        ToneGeneration_SetFrequency(POT_old);
    }
#endif
#ifdef PART6
    /*==========================================================================
     * Part6
     =========================================================================*/
    unsigned int POT_old = 0;
    unsigned int POT_curr;
    unsigned int offset = 0;
    unsigned int tone = 0;

    while (1) {
        POT_curr = AD_ReadADPin(AD_A0);
        if (POT_curr < 10) {
            POT_old = 0;
        } else if (POT_curr > 1013) {
            POT_old = 1023;
        } else if (POT_curr > POT_old + 6 || POT_curr < POT_old - 6) {
            POT_old = POT_curr;
            offset = (500*POT_old) / 1024;
            printf("AD Value: %u\r\n", POT_curr);
            printf("Offset: %u\r\n", offset);
            sprintf(string,"AD Value: %d", POT_curr);
            OledDrawString(string);
            OledUpdate();
        }

        if (BUTTON_STATES()) {
            ToneGeneration_ToneOn();
            if (BUTTON_STATES() & 0b0001) {
                tone = 150 + offset;
            } else if (BUTTON_STATES() & 0b0010) {
                tone = 200 + offset;
            } else if (BUTTON_STATES() & 0b0100) {
                tone = 250 + offset;
            } else if (BUTTON_STATES() & 0b1000) {
                tone = 300 + offset;
            }
        } else {
            ToneGeneration_ToneOff();
        }

        ToneGeneration_SetFrequency(tone);
    }

#endif




#ifdef BTN_TEST
    while (1) {
        printf("%d\r\n", BUTTON_STATES());
        DELAY(500000);
    }
#endif
}