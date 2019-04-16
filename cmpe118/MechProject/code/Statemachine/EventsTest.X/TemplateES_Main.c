/*==============================================================================
 * File Name: 
 * Coders: Team 14 - Johnson, Jacqueline, Minghan
 * Description: 
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include <BOARD.h>
#include <xc.h>
#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TapeSensor.h"
#include <AD.h>
#include <serial.h>
#include "pwm.h"
#include <LED.h>
#include "BeaconDetector.h"

#define MAIN_CODE
#ifdef MAIN_CODE
/*==============================================================================
 * Macros and Declarations
 =============================================================================*/
static int k;
#define DELAY(x) for(k=0; k<x; k++) 
#define PIN_PWM_Launcher PWM_PORTZ06

void Test(void);

/*==============================================================================
 * Main Code
 =============================================================================*/
void main(void) {
    /*==========================================================================
     * Initialization
    ==========================================================================*/
    ES_Return_t ErrorType; //error checking
    BOARD_Init();
    AD_Init();
    PWM_Init();
    LED_Init();
    AD_AddPins(AD_PORTW3 | AD_PORTW4 | AD_PORTW5 | AD_PORTW6 | BEACON_PIN);
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);
    // add Beacon Pins
    //    PWM_AddPins(PWM_PORTZ06);
    Motors_Init();
    //    Test();

    printf("HELLO! We are team 14. Starting ES Framework.\r\n");
    printf("Using the 2nd Generation Events & Services Framework\r\n");

    //init es framework and events
    ErrorType = ES_Initialize();
    if (ErrorType == Success) {
        ErrorType = ES_Run();
    }
    //if error occurred:
    switch (ErrorType) {
        case FailedPointer:
            printf("Failed on NULL pointer");
            break;
        case FailedInit:
            printf("Failed Initialization");
            break;
        default:
            printf("Other Failure: %d", ErrorType);
            break;
    }

    /*==========================================================================
     * While Loop
    ==========================================================================*/
    while (1);

};

void Test(void) {
    while (1) {
        //        printf("%d\r\n", TAPE_Read());
        //        TAPE_PrintDigitalAll();
        //        TAPE_PrintAnalogAll();
        Beacon_PrintValue();
        DELAY(500000);
    }
}
//void LauncherTest(void) {
//    printf("HELLO!\r\n");
//    while (1) {
//        PWM_SetDutyCycle(PWM_PORTZ06, 600);
//    }
//}
/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

#endif