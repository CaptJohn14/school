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
#define BUMPERS_INPUTPATTERN PIN3 | PIN4
#define LEFT_BUMP 3
#define RIGHT_BUMP 4

/*==============================================================================
 * Initialize Motors // Sets up IO Ports? 
 =============================================================================*/
int Bumpers_Init(void);
int Bumpers_ReadLeftBumper(void);
int Bumpers_ReadRightBumper(void);
int Bumpers_ReadBumpers(void);
void Bumpers_Print(void); // primarily for testing

#endif	/* BUMPERS_H */
