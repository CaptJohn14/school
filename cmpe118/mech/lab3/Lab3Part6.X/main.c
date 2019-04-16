#include "xc.h"
#include "AD.h"
#include <BOARD.h>
#include "LED.h"
#include "serial.h"
#include "Stepper.h"
#include "timers.h"
#include "stdio.h"

void delay(long long time);

int main() {
    //======================================================//
    //-------------------- Initializations -----------------//
    //======================================================//
    //202 steps for 360 degrees
    BOARD_Init();
    uint8_t errors = 0;
    //    uint16_t setRate = 500;
    uint16_t setRate = 800;
    int32_t numSteps = 200;
    int8_t result;

    result = Stepper_Init();
    if (result != SUCCESS) {
        errors++;
    }
    if (errors) {
        printf("\n\rStepper_Init() function failed");
        while (1);
    } else {
        printf("\n\rStepper_Init() function passed");
    }
    printf("\r\n\r\n");
    int amnt = 8;
    int i = amnt;
    //======================================================//
    //---------------------- Code --------------------------//
    //======================================================//
    while (1) {
        Stepper_SetRate(setRate);
        Stepper_InitSteps(FORWARD, numSteps);
//        i--;
//        if(i <= 0){
//            setRate += 50;
//            printf("%u\r\n", setRate);
//            i = amnt;
//        }
//        Stepper_InitSteps(REVERSE, numSteps);
//        Stepper_StartSteps();
        delay(200000);
//        while(Stepper_IsStepping());
        
    }

}

void delay(long long time) {
    int i;
    for (i = 0; i < time; i++);
}