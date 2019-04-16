/*
 * File:   main.c
 * Author: daaraujo
 *
 * Created on October 28, 2018, 7:29 PM
 */


#include "xc.h"
#include "AD.h"
#include <BOARD.h>
#include "LED.h"
#include "pwm.h"
#include "serial.h"
#include "Stepper.h"
#include "timers.h"
#include "stdio.h"

int main() {
    //======================================================//
    //-------------------- Initializations -----------------//
    //======================================================//
    BOARD_Init();
    PWM_Init();
    LED_Init();
    AD_Init();
    Stepper_Init();

   
    AD_AddPins(AD_PORTW6);
    PWM_AddPins(PWM_PORTX11);
    Stepper_SetRate(1337);
    int32_t numSteps;
    numSteps = 50;
    printf("\n\r\n\rNow setting stepper to turn forwards and backwards %d steps at %d Hz", numSteps, Stepper_GetRate());
    Stepper_InitSteps(FORWARD, numSteps);
    printf("\r\nStepping Forwards");
    while (1) {
        if (!Stepper_IsStepping()) {
            if (Stepper_GetDirection() == FORWARD) {
                Stepper_InitSteps(REVERSE, numSteps);
                printf("\r\nStepping Backwards");
            } else {
                Stepper_InitSteps(FORWARD, numSteps);
                printf("\r\nStepping Forwards");
            }
        }
    }
    
    
    //======================================================//
}
