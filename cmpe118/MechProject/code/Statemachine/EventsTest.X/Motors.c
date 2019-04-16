/*==============================================================================
 * File Name: Motors.c
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Source for Controlling Motors
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "Motors.h"
#include <AD.h>
#include <serial.h>
#include <stdio.h>
#include <BOARD.h>
#include <IO_Ports.h>
#include <pwm.h>
#include <RC_Servo.h>

/*==============================================================================
 * Initialize Motors // Sets up IO Ports and PWM
 =============================================================================*/
int Motors_Init(void) {
    RC_Init();
    PWM_AddPins(DC_RIGHT_PORT | DC_LEFT_PORT | DC_SHOOTER_PORT);
    RC_AddPins(SERVO_PORT);
    IO_PortsSetPortOutputs(DC_PORTS, DC_PORTOUTPUT_PATTERN);
    PWM_SetFrequency(MOTOR_FREQUENCY);
    return MOTOR_SUCCESS;
}

/*==============================================================================
 * Set Right Motor Value // Can input negative value 
 =============================================================================*/

void Motors_SetRight(int value) {
    if ((value < -MOTOR_MAX_SPEED) || (value > MOTOR_MAX_SPEED)) {
        printf("\r\n Motor value too high");
        return;
    }
    if (value > 0) {
        IO_PortsWritePort(DC_PORTS, DC_RIGHT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, value * (10));
    } else {
        IO_PortsClearPortBits(DC_PORTS, DC_RIGHT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, (unsigned int) value * (-10));
    }
}

/*==============================================================================
 * Set Left Motor Value // Can input negative value 
 =============================================================================*/
void Motors_SetLeft(int value) {
    if ((value < -MOTOR_MAX_SPEED) || (value > MOTOR_MAX_SPEED)) {
        return;
    }
    if (value > 0) {
        IO_PortsWritePort(DC_PORTS, DC_LEFT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_LEFT_PORT, value * (10) + MOTOR_BIAS);
    } else {
        IO_PortsClearPortBits(DC_PORTS, DC_LEFT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_LEFT_PORT, (unsigned int) value * (-10) - MOTOR_BIAS);
    }
}

/*==============================================================================
 * Set Both Motor Values // Can input negative value 
 =============================================================================*/
void Motors_SetBoth(int value) {
    if ((value < -MOTOR_MAX_SPEED) || (value > MOTOR_MAX_SPEED)) {
        return;
    }
    //    if (value > 0) {
    //        IO_PortsWritePort(DC_PORTS, DC_PORTOUTPUT_PATTERN);
    //        PWM_SetDutyCycle(DC_RIGHT_PORT, value * 10);
    //        PWM_SetDutyCycle(DC_LEFT_PORT, value * 10 + MOTOR_BIAS);
    //    } else {
    //        IO_PortsClearPortBits(DC_PORTS, DC_PORTOUTPUT_PATTERN);
    //        PWM_SetDutyCycle(DC_RIGHT_PORT, value * (-10));
    //        PWM_SetDutyCycle(DC_LEFT_PORT, value * (-10) - MOTOR_BIAS);
    //    }
//    Motors_SetRight(value);
//    Motors_SetLeft(value);
    Motors_Set(value, value);
}

void Motors_Set(int rightval, int leftval) {
    if ((rightval < -MOTOR_MAX_SPEED) || (rightval > MOTOR_MAX_SPEED)||(leftval < -MOTOR_MAX_SPEED) || (leftval > MOTOR_MAX_SPEED)) {
        return;
    }
    
    if ((rightval > 0) && (leftval > 0)) {
        IO_PortsWritePort(DC_PORTS, DC_PORTOUTPUT_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, rightval * 10);
        PWM_SetDutyCycle(DC_LEFT_PORT, leftval * 10 + MOTOR_BIAS);
    } else if ((rightval > 0) && (leftval < 0)) {
        IO_PortsWritePort(DC_PORTS, DC_RIGHT_DIR_PATTERN);
        IO_PortsClearPortBits(DC_PORTS, DC_LEFT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, rightval * 10);
        PWM_SetDutyCycle(DC_LEFT_PORT, leftval * (-10) - MOTOR_BIAS);
    } else if ((rightval < 0) && (leftval > 0)) {
        IO_PortsWritePort(DC_PORTS, DC_LEFT_DIR_PATTERN);
        IO_PortsClearPortBits(DC_PORTS, DC_RIGHT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, rightval * (-10));
        PWM_SetDutyCycle(DC_LEFT_PORT, leftval * 10 + MOTOR_BIAS);
    } else {
        IO_PortsClearPortBits(DC_PORTS, DC_PORTOUTPUT_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, rightval * (-10));
        PWM_SetDutyCycle(DC_LEFT_PORT, leftval * (-10) - MOTOR_BIAS);
    }
}

/*==============================================================================
 *  FOR DEBUGGING ONLY:
 *  Set Both Motor Values and Printing the Value // Can input negative value 
 =============================================================================*/
void Motors_SetBoth_Print(int value) {
    Motors_SetBoth(value);
    printf("\r\n Motors: %d", value);
}

/*==============================================================================
 * Set Shooter Motor Value // Only Positive value
 =============================================================================*/
int Motors_SetShooter(int value) {
    if (value < 0) {
        return MOTOR_ERROR;
    }
    PWM_SetDutyCycle(DC_SHOOTER_PORT, value * 10);
    return MOTOR_SUCCESS;
}

void Servo_Set(int value) {
    RC_SetPulseTime(SERVO_PORT, value);
}

void Servo_OneBall(void) {
    Servo_Set(SERVO_ONE_BALL);
}

/*==============================================================================
 * TEST BENCH
 =============================================================================*/

//#define MOTOR_TEST
#ifdef MOTOR_TEST

static int k;
#define DELAY(x) for(k=0; k<x; k++) 
#include <stdio.h>

int main(void) {
    BOARD_Init();
    PWM_Init();
    AD_Init();
    Motors_Init();

    printf("\r\n Initiating the motor test code: \r\n");
//    Motors_SetBoth(50);
    Servo_OneBall();
    while (1) {

    }
}

#endif

