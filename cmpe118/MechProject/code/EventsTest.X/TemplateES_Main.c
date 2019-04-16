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
#include "Motors.h"
#include "BeaconDetector.h"

/*==============================================================================
 * Macros and Declarations
 =============================================================================*/

static int k;
#define DELAY(x) for(k=0; k<x; k++) 
#define PIN_PWM_Launcher PWM_PORTZ06

void LauncherTest(void);
void ReadBeaconDetector(void);

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
    AD_AddPins(AD_PORTW3 | AD_PORTW4 | BEACON_PIN);
    Motors_Init();
    
    LauncherTest();
//    ReadBeaconDetector();

    //    IO_PortsSetPortInputs(BUMPERS_PORT, BUMPERS_INPUTPATTERN);

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

void LauncherTest(void) {
    printf("HELLO!\r\n");
    while (1) {
        Motors_SetRight(1000);
        Motors_SetLeft(1000);
    }
}

static int last_peak = 0;

void ReadBeaconDetector(void) {
    while (1) {
        int peak;
        peak = Beacon_FindPeak();
        if (peak != last_peak) {
//            printf("\r\n Peak: %d", peak);
            last_peak = peak;
        }
    }
}
/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/
