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
#define SERVO_PORT RC_PORTY06
#define SERVO_ONE_BALL 1250
#define DC_RIGHT_DIR_PATTERN PIN7
#define DC_LEFT_DIR_PATTERN PIN8
#define DC_PORTS PORTY 
#define DC_PORTOUTPUT_PATTERN (DC_RIGHT_DIR_PATTERN) | (DC_LEFT_DIR_PATTERN)
// Port Y7 and Y8 for direction control 

#define MOTOR_BIAS 0
#define MOTOR_MIN_SPEED 50
#define MOTOR_MAX_SPEED 100 - MOTOR_BIAS
#define MOTOR_FREQUENCY 10000

#define Moving_Speed 70
#define Soft_Turn 70
#define Hard_Turn 70

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
 * FOR DEBUGGING ONLY: 
 * Set Both Motor Values and Print the Value // Can input negative value 
 =============================================================================*/
void Motors_SetBoth_Print(int value);

/*==============================================================================
 * Set Shooter Motor Value // Only Positive value
 =============================================================================*/
int Motors_SetShooter(int value);

/*==============================================================================
 * Set Servo [ONLY VALUES BETWEEN 1000 and 2000 for uS] for pulse width. 
 * Servo_oneBall is to send one ball.
 *  =============================================================================*/
void Servo_Set(int value);
void Servo_OneBall(void);
#endif	/* MOTORS_H */

