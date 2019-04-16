/*
 * File:    main.c
 * Authors: Johnson Le
 *          Talin Hallajian
 * Description: Creating test harness
 * Created on October 1, 2018, 5:04 PM
 */


#include <BOARD.h>
#include <stdio.h>
#include "xc.h"
#include <roach.h>

#define FrontLeftHit 0b0001
#define FrontRightHit 0b0010
#define BackLeftHit 0b0100
#define BackRightHit 0b1000

void FrontLeftTest(void);
void FrontRightTest(void);
void BackLeftTest(void);
void BackRightTest(void);
void Delay(long long time);
void FlashLED(int amount);

int main(void) {
    //=========================================================================
    //Initializations
    //=========================================================================
    BOARD_Init();
    Roach_Init();
    //=========================================================================
    //welcome the user 
    //=========================================================================
    printf("Welcome to our test harness\r\n");
    printf("Front Left Bumper: left motor test.\r\n");
    printf("Front Right Bumper: right motor test.\r\n");
    printf("Back Left Bumper: display light level and battery voltage\r\n");
    printf("Back Right Bumper: led test\r\n");

    while (1) {
        char bumperState = Roach_ReadBumpers();
        switch (bumperState) {
            case FrontLeftHit:
                FrontLeftTest();
                break;
            case FrontRightHit:
                FrontRightTest();
                break;
            case BackLeftHit:
                BackLeftTest();
                break;
            case BackRightHit:
                BackRightTest();
                break;
            default:
                break;
        }
    }
}

//=========================================================================
//Will be used to test the left motor. Will start at 100 and decrement down
//till it stops. Then it will start at -100 and go up till it stops.
//The speed of the Roach will be displayed on the serial port.
//The corresponding motor LED will be lit up depending on FWD or REV.
//=========================================================================

void FrontLeftTest(void) {
    //char Roach_LeftMtrSpeed(char newSpeed);
    printf("Beginning left motor test.\r\n");
    printf("Left motor Speed: ");
    char i;
    for (i = 100; i >= 0; i -= 10) {
        Roach_LeftMtrSpeed(i);
        printf("%d, ", i);
        Delay(200);
    }
    for (i = -100; i <= 0; i += 10) {
        Roach_LeftMtrSpeed(i);
        printf("%d, ", i);
        Delay(200);
    }
    printf("\r\nLeft motor test completed.\r\n");
}

//=========================================================================
//Will be used test the right motor. It is similar to the left motor test.
//=========================================================================
void FrontRightTest(void) {
    //char Roach_LeftMtrSpeed(char newSpeed);
    printf("Beginning right motor test.\r\n");
    printf("right motor Speed: ");
    char i;
    for (i = 100; i >= 0; i -= 10) {
        Roach_RightMtrSpeed(i);
        printf("%d, ", i);
        Delay(200);
    }
    for (i = -100; i <= 0; i += 10) {
        Roach_RightMtrSpeed(i);
        printf("%d, ", i);
        Delay(200);
    }
    printf("\r\nRight motor test completed.\r\n");
}

//=========================================================================
//This will test our light sensor as well as the battery voltage reading.
//=========================================================================
void BackLeftTest(void) {
    printf("Beginning light sensor and battery voltage test.\r\n");
    printf("Battery Voltage: %u\r\n", Roach_BatteryVoltage());
    int i;
    printf("Light Level: ");
    for(i=0; i<10; i++){
        printf("%u, ", Roach_LightLevel());
        Delay(250);
    }
    printf("\r\nLight sensor and battery voltage test completed.\r\n");
}

//=========================================================================
//This tests the LED bar. First it tests individual LED's, and then flashes 
//all LED's 5 times. 
//=========================================================================
void BackRightTest(void) {
    printf("Beginning LED bar test.\r\n");
    int i;
    uint16_t LedMask = 0b1;
    for (i = 0; i < 12; i++) {
        Roach_LEDSSet(LedMask);
        LedMask <<= 1;
        Delay(100);
    }
    FlashLED(5);
    printf("LED bar test completed.\r\n");
}
//=========================================================================
//This is a helper function used for flashing the LED bar.
//=========================================================================
void FlashLED(int amount) {
    int i;
    for (i = 0; i < amount; i++) {
        Roach_BarGraph(12);
        Delay(100);
        Roach_BarGraph(0);
        Delay(100);
    }
}

//=========================================================================
//Helper delay function
//=========================================================================
void Delay(long long time) {
    long long i;
    for (i = 0; i < (time * 500); i++);
}

