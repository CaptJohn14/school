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

/*==============================================================================
 * Initialize Motors // Sets up IO Ports and PWM
 =============================================================================*/
int Motors_Init(void) {
    PWM_AddPins(DC_RIGHT_PORT|DC_LEFT_PORT|DC_SHOOTER_PORT);
    IO_PortsSetPortOutputs(PORTZ, DC_PORTOUTPUT_PATTERN);
    PWM_SetFrequency(MOTOR_FREQUENCY);
    return MOTOR_SUCCESS;
}

/*==============================================================================
 * Set Right Motor Value // Can input negative value 
 =============================================================================*/

void Motors_SetRight(int value) {
    if((value < -MOTOR_MAX_SPEED)|(value > MOTOR_MAX_SPEED)) {
        return; 
    }
    if (value < 0) {
        IO_PortsWritePort(DC_PORTS, DC_RIGHT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, value*(-1));

    } else {
        IO_PortsClearPortBits(DC_PORTS,DC_RIGHT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_RIGHT_PORT, (unsigned int) value);
    }
}

/*==============================================================================
 * Set Left Motor Value // Can input negative value 
 =============================================================================*/
void Motors_SetLeft(int value) {
        if((value < -MOTOR_MAX_SPEED)|(value > MOTOR_MAX_SPEED)) {
        return; 
    }
    if (value < 0) {
        IO_PortsWritePort(DC_PORTS, DC_LEFT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_LEFT_PORT, value*(-1));
    } else {
        IO_PortsClearPortBits(DC_PORTS,DC_LEFT_DIR_PATTERN);
        PWM_SetDutyCycle(DC_LEFT_PORT, (unsigned int) value);
    }
}

/*==============================================================================
 * Set Both Motor Values // Can input negative value 
 =============================================================================*/
void Motors_SetBoth(int value){
    Motors_SetRight(value);
    Motors_SetLeft(value);
}

/*==============================================================================
 * Set Shooter Motor Value // Only Positive value
 =============================================================================*/
int Motors_SetShooter(int value) {
    if(value <0){ 
        return MOTOR_ERROR;
    }
    PWM_SetDutyCycle(DC_SHOOTER_PORT, value);
    return MOTOR_SUCCESS;
}