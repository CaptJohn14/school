/*==============================================================================
 * File Name: Motors.h
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Header for Controlling Motors
 =============================================================================*/

#ifndef MOTORS_H
#define	MOTORS_H

/*==============================================================================
 * Libraries
 =============================================================================*/

/*==============================================================================
 * Macros and Declarations
 =============================================================================*/
#define MOTOR_SUCCESS 1
#define MOTOR_ERROR 0 

// PWM Ports
#define DC_RIGHT_PORT PWM_PORTY12
#define DC_LEFT_PORT PWM_PORTY10
#define DC_SHOOTER_PORT PWM_PORTZ06

// IO Ports
#define DC_RIGHT_DIR_PATTERN (1<<7)
#define DC_LEFT_DIR_PATTERN (1<<8)
#define DC_PORTS PORTY 
#define DC_PORTOUTPUT_PATTERN (1<<7) | (1<<8)
// Port Y7 and Y8 for direction control 

#define MOTOR_MAX_SPEED 100
#define MOTOR_FREQUENCY 20000

/*==============================================================================
 * Initialize Motors // Sets up IO Ports? 
 =============================================================================*/
int Motors_Init(void);

/*==============================================================================
 * Set Right Motor Value // Can input negative value 
 =============================================================================*/

void Motors_SetRight(int value);

/*==============================================================================
 * Set Left Motor Value // Can input negative value 
 =============================================================================*/
void Motors_SetLeft(int value);

/*==============================================================================
 * Set Both Motor Values // Can input negative value 
 =============================================================================*/
void Motors_SetBoth(int value);

/*==============================================================================
 * Set Shooter Motor Value // Only Positive value
 =============================================================================*/
int Motors_SetShooter(int value);

#endif	/* MOTORS_H */

