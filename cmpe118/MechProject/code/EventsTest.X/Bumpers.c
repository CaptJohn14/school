/*==============================================================================
 * File Name: Bumpers.c
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Source for Reading Bumpers
 =============================================================================*/
  
/*==============================================================================
 * Libraries
 =============================================================================*/
#include "Bumpers.h"
#include <AD.h>
#include <serial.h>
#include <stdio.h>
#include <BOARD.h>
#include <IO_Ports.h>
#include <pwm.h>

/*==============================================================================
 * Initialize Motors // Sets up IO Ports? 
 =============================================================================*/
int Bumpers_Init(void) {
    IO_PortsSetPortInputs(BUMPERS_PORT, BUMPERS_INPUTPATTERN);
    return BUMPER_SUCCESS;
}

/*==============================================================================
 * Read Left Bumper value (1 hit, 0 not hit) // returns 1 bit 
 =============================================================================*/
int Bumpers_ReadLeftBumper(void) {
//    int left = (IO_PortsReadPort(BUMPERS_PORT) >> LEFT_BUMP )& 1;
    return ((IO_PortsReadPort(BUMPERS_PORT) >> LEFT_BUMP )& 1);
}

/*==============================================================================
 * Read Right Bumper value (1 hit, 0 not hit) // returns 1 bit
 =============================================================================*/
int Bumpers_ReadRightBumper(void) {
//    int right = (IO_PortsReadPort(BUMPERS_PORT) >> RIGHT_BUMP )& 1;
    return ((IO_PortsReadPort(BUMPERS_PORT) >> RIGHT_BUMP )& 1);    
}

/*==============================================================================
 * Reads Both Bumpers // returns Left Bit, Right Bit (00, 10, 01, 11) 
 =============================================================================*/
int Bumpers_ReadBumpers(void) {
    return ((IO_PortsReadPort(BUMPERS_PORT) >> RIGHT_BUMP) & (1|1<<1));
}

