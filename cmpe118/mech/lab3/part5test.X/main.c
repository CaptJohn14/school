

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
    uint16_t setRate = 500;
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

    //======================================================//
    //---------------------- Code --------------------------//
    //======================================================//
    int steps = 1;
    while (1) {
        steps++;
        Stepper_SetRate(setRate);
        Stepper_InitSteps(FORWARD, numSteps);

//        if (steps == 8) {
//            setRate = setRate + 10;
//            printf("%u\r\n", setRate);
//            steps = 1;
//        }

        delay(250000);
    }
    //    Stepper_InitSteps(FORWARD, numSteps);  


}

void delay(long long time) {
    int i;
    for (i = 0; i < time; i++);
}