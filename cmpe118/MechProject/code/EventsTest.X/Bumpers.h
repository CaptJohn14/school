/*==============================================================================
 * File Name: Bumpers.h
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Header for Controlling Motors
 =============================================================================*/

#ifndef BUMPERS_H
#define	BUMPERS_H

/*==============================================================================
 * Libraries
 =============================================================================*/

/*==============================================================================
 * Macros and Declarations
 =============================================================================*/

#define BUMPER_SUCCESS 1
#define BUMPER_ERROR 0

#define BUMPERS_PORT PORTZ
#define BUMPERS_INPUTPATTERN (1<<8) | (1<<7)
#define LEFT_BUMP 8
#define RIGHT_BUMP 7

/*==============================================================================
 * Initialize Motors // Sets up IO Ports? 
 =============================================================================*/
int Bumpers_Init(void);

/*==============================================================================
 * Read Left Bumper value (1 hit, 0 not hit) // returns 1 bit 
 =============================================================================*/
int Bumpers_ReadLeftBumper(void);

/*==============================================================================
 * Read Right Bumper value (1 hit, 0 not hit) // returns 1 bit
 =============================================================================*/
int Bumpers_ReadRightBumper(void);

/*==============================================================================
 * Reads Both Bumpers // returns Left Bit, Right Bit (00, 10, 01, 11) 
 =============================================================================*/
int Bumpers_ReadBumpers(void); 

#endif	/* BUMPERS_H */

