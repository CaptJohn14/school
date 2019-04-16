/*==============================================================================
 * File Name: RobotTestHarness.c
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Source for Reading TapeSensor
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "RobotTestHarness.h"
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
#define LONG_DELAY 200000
#define MOTOR_DELAY 400000

#define RBUMPER 0x01
#define LBUMPER 0x02

/*==============================================================================
 * RUN TEST HARNESS LOL
 =============================================================================*/
int main(void) {

    BOARD_Init();
    AD_Init();
    PWM_Init();
    AD_AddPins(AD_PORTW3 | AD_PORTW4 | BEACON_PIN);

    Bumpers_Init();
    Motors_Init();

    if (Bumpers_ReadBumpers())
        printf("Failed:Bumper Falsely Triggered\r\n");

    // Test L Bumper
    printf("\r\n Trigger Front Left Bumper\r\n");
    while (!(Bumpers_ReadLeftBumper()));
    printf("\r\n Left Bumper Triggered");


    // Test F Bumper
    printf("\r\n Trigger Front Right Bumper\r\n");
    while (!(Bumpers_ReadRightBumper()));
    printf("\r\n Right Bumper Triggered");

    //	print "All Bumpers are working"
    printf("Bumper Test Complete. All bumpers Working!\r\n");
    DELAY(LONG_DELAY);


    printf("\r\n INITIATING RIGHT MOTOR TEST:");
    for (i = 5; i >= 0; i--) {
        int speed = i * 20;
        Motors_SetRight(speed);
        printf("\r\nRight Motor Speed Set to: %d", speed);
        DELAY(MOTOR_DELAY);
    }
    for (i = -5; i <= 0; i++) {
        int speed = i * 20;
        Motors_SetRight(speed);
        printf("\r\nRight Motor Speed Set to: %d", speed);
        DELAY(MOTOR_DELAY);
    }

    printf("\r\n INITIATING LEFT MOTOR TEST:");
    for (i = 5; i >= 0; i--) {
        int speed = i * 20;
        Motors_SetLeft((char) speed);
        printf("\r\nLeft Motor Speed Set to: %d", speed);
        DELAY(MOTOR_DELAY);
    }
    for (i = -5; i <= 0; i++) {
        int speed = i * 20;
        Motors_SetLeft((char) speed);
        printf("\r\nLeft Motor Speed Set to: %d", speed);
        DELAY(MOTOR_DELAY);
    }

    printf("\r\nTrigger R  Bumper to get Tape reading\r\n");
    printf("Trigger L Bumper to get Beacon reading\r\n");

    int bumperStates;
    while (1) {
        bumperStates = Bumpers_ReadBumpers();

        switch (bumperStates) {
            case LBUMPER:
                printf("Beacon Value: %d\r\n", Beacon_ReadValue());
                break;
            case RBUMPER:
                int i;
                for (i = 0; i < 5; i++) {
                    TAPE_PrintAnalogAll();
                }
                break;
        }
    }


}
