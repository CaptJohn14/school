/*
 * File:   main.c
 * Author: daaraujo, jonle
 *
 * Created on October 27, 2018, 12:15 AM
 */


#include "xc.h"
#include "BOARD.h"
#include "LED.h"
#include "serial.h"
#include "timers.h"
#include "pwm.h"
#include "AD.h"
#include <stdio.h>
#include <RC_Servo.h>


#define MASK_BANK1 0x000F00
#define MASK_BANK2 0x0000F0
#define MASK_BANK3 0x00000F
#define led 0xFFF000

int main() {
    BOARD_Init();
    PWM_Init();
    LED_Init();
    AD_Init();
    RC_Init();
    
    
    char CheckPins;
    CheckPins = AD_AddPins(AD_PORTV4);
    if (!CheckPins) {
        printf("AD Pin ERROR found.\n");
    }    
    CheckPins = PWM_AddPins(PWM_PORTZ06);
    if (!CheckPins) {
        printf("PWM Pin ERROR found.\n");
    }
    CheckPins = RC_AddPins(RC_PORTY06);
    if (!CheckPins) {
        printf("RC pin ERROR found.\n");
    }
    
    int i;
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);
    unsigned int ADValues = AD_ReadADPin(AD_PORTV3);
    unsigned int shift = 0;    
    
    while(1){
        while (!AD_IsNewDataReady());

        ADValues = AD_ReadADPin(AD_PORTV4);
        shift = (ADValues * 12) / 1023;

        LED_SetBank(LED_BANK1, ((led >> shift) & MASK_BANK1) >> 8);
        LED_SetBank(LED_BANK2, ((led >> shift) & MASK_BANK2) >> 4);
        LED_SetBank(LED_BANK3, ((led >> shift) & MASK_BANK3));  
        //Will operate at 20% to 80% duty cycle, wont go below 20% or above 80%
        PWM_SetDutyCycle(PWM_PORTZ06, ((ADValues * 600)/1023) + 200);
        printf("%u\r\n", PWM_GetDutyCycle(PWM_PORTZ06));
        
        RC_SetPulseTime(RC_PORTY06, ((ADValues * 1000) / 1023) + 1000);
       // for(i = 0; i <300000; i++);

    }
    
}
