//=============================================================================
//  File: Lab3Part1.c
//  Date: 10/27/2018
//  Description: Controlling RC Servo
//  Contributors:   Daniel Araujo
//                  Johnson Le
//=============================================================================

//=============================================================================
//  Libraries
//=============================================================================
#include <BOARD.h>
#include <xc.h>
#include <stdio.h>
#include <LED.h>
#include <AD.h>
#include <serial.h>
#include <RC_Servo.h>

//=============================================================================
//  Defines
//=============================================================================
#define MASK_BANK1 0x000F00
#define MASK_BANK2 0x0000F0
#define MASK_BANK3 0x00000F
#define led 0xFFF000

int main() {
    //==========================================================================
    //  Initializations
    //==========================================================================
    BOARD_Init();
    LED_Init();
    RC_Init();
    AD_Init();
    char CheckPins;
    CheckPins = AD_AddPins(AD_PORTV4);

    if (!CheckPins) {
        printf("AD Pin ERROR found.\n");
    }
    CheckPins = RC_AddPins(RC_PORTY06);

    if (!CheckPins) {
        printf("RC pin ERROR found.\n");
    }
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);
    //==========================================================================
    //  Code
    //==========================================================================
    unsigned int ADValues = AD_ReadADPin(AD_PORTV3);
    unsigned int shift = 0;

    while (1) {
        //Will halt until a new potentiometer reading is available
        while (!AD_IsNewDataReady());
        ADValues = AD_ReadADPin(AD_PORTV4);
        shift = (ADValues * 12) / 1023;

        //set leds according to potentiometer reading
        LED_SetBank(LED_BANK1, ((led >> shift) & MASK_BANK1) >> 8);
        LED_SetBank(LED_BANK2, ((led >> shift) & MASK_BANK2) >> 4);
        LED_SetBank(LED_BANK3, ((led >> shift) & MASK_BANK3));  

        //set RC servo to certain position based on potentiometer reading.
        RC_SetPulseTime(RC_PORTY06, ((ADValues * 1000) / 1023) + 1000);
    }
}