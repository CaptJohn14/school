
/*==============================================================================
 * Name: Johnson Le
 * Date: 1/17/2019
 * Desc: Main for lab 1.
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "BOARD.h"
#include "AD.h"
#include "Oled.h"
#include "ToneGeneration.h"
#include <stdio.h>
#include <xc.h>

/*==============================================================================
 * Defines/Functions
 =============================================================================*/
#define PART4
int k;
#define DELAY(x) for(k=0; k<x; k++)

#define PIN_FLEX AD_A1
#define PIN_VIBR AD_A3
#define bool int
#define TRUE 1
#define FALSE 0

//#define SLOPE -2.22767
//#define ZEROVAL 1002.9
#define SLOPE -5.625
#define ZEROVAL 1006.3

unsigned int deg();
unsigned int read_FLEX();
bool isVibrate();

/*==============================================================================
 * Main
 =============================================================================*/
int main() {
    /*==========================================================================
     * Initialization
     =========================================================================*/
    BOARD_Init();
    AD_Init();
    OledInit();
    ToneGeneration_Init();
    char string[40];
    char ErrorCheck;
    ErrorCheck = AD_AddPins(PIN_FLEX);
    ErrorCheck = AD_AddPins(PIN_VIBR);
    if (!ErrorCheck) {
        printf("ERROR! \r\n");
        while (1);
    }

    unsigned int flex_read;
    unsigned int tone;
    int degree;
#ifdef PART123
    ToneGeneration_ToneOn();
    while (1) {
        flex_read = read_FLEX();
        degree = deg(flex_read);
        tone = 100 + (((850 - 100) / (100 - 0))*(degree - 0));
        //        sprintf(string, "READ: %u    %d \n %u", flex_read, degree, tone);
        OledDrawString(string);
        OledUpdate();
        OledClear(0);
        ToneGeneration_SetFrequency(tone);
    }
#endif
#ifdef PART4
    int i = 0;
    DELAY(500000);
    while (1) {
        sprintf(string, "flex: %u ~~ deg: %u", AD_ReadADPin(PIN_FLEX), deg());
        OledDrawString(string);
        OledUpdate();
        OledClear(0);
        ToneGeneration_ToneOff();
        tone = 100 + (((850 - 100) / (100 - 0))*(deg() - 0));
        ToneGeneration_SetFrequency(tone);
        if (isVibrate() == TRUE) {
            ToneGeneration_ToneOn();
            for (i = 0; i < 50000; i++) {
                if (isVibrate() == TRUE) {
                    i = 0;
                }
            }
        }
    }
#endif  
#ifdef TEST
    while (1) {
        sprintf(string, "flex reading: %u", AD_ReadADPin(PIN_FLEX));
        OledDrawString(string);
        OledUpdate();
        OledClear(0);
    }
#endif
}

/*==============================================================================
 * Functions
 =============================================================================*/
unsigned int deg() {
    unsigned int out = ((read_FLEX() - ZEROVAL) / SLOPE);
    if (out >= 100) {
        out = 100;
    }
    return out;
}

unsigned int read_FLEX() {
    static unsigned int out = 0;
    unsigned int curr = AD_ReadADPin(PIN_FLEX);
    if (curr >= 950) {
        curr = 950;
    } else if (curr <= 50) {
        curr = 50;
    }
    if ((curr > out + 25) || (curr < out - 25)) {
        out = curr;
    }
    return out;
}

bool isVibrate() {
    int i = 0;
    if (AD_ReadADPin(PIN_VIBR) > 200) {
        return TRUE;
    }
    return FALSE;
}