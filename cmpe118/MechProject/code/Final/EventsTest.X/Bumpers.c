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
 * Initialize Bumpers // Sets up IO Ports? 
 =============================================================================*/
int Bumpers_Init(void) {
    return (int) (IO_PortsSetPortInputs(BUMPERS_PORT, BUMPERS_INPUTPATTERN));
}

int Bumpers_ReadLeftBumper(void) {
//    int left = (IO_PortsReadPort(BUMPERS_PORT) >> LEFT_BUMP )& 1;
    return ((IO_PortsReadPort(BUMPERS_PORT) >> LEFT_BUMP)& 1);
}

int Bumpers_ReadRightBumper(void) {
//    int right = (IO_PortsReadPort(BUMPERS_PORT) >> RIGHT_BUMP )& 1;
    return ((IO_PortsReadPort(BUMPERS_PORT) >> RIGHT_BUMP)& 1);    
}

int Bumpers_ReadBumpers(void) {
    return ((IO_PortsReadPort(BUMPERS_PORT) >> LEFT_BUMP) & (1|1<<1));
}

void Bumpers_Print(void) {
    printf("\r\n Bumpers: %d", Bumpers_ReadBumpers());
}

/*==============================================================================
 * Function: printBinary
 * Use: recursion. prints given param in binary
 =============================================================================*/
static void printBinary(int16_t n) {
    if (n > 1)
        printBinary(n / 2);
    printf("%d", n % 2);
}

/*==============================================================================
* Test Harness
 =============================================================================*/
//#define BUMPER_TEST
#ifdef BUMPER_TEST

static int k;
#define DELAY(x) for(k=0; k<x; k++) 
#include <stdio.h>

int main(void){
    BOARD_Init();
    AD_Init();
    LED_Init();
    int success; 
    success = Bumpers_Init();
    if (!success) {
        printf("\r\n Initialization error.");
    }
    while(1){
        Bumpers_Print();
//        printf("\r\n PortZ: %d", ((IO_PortsReadPort(BUMPERS_PORT) >> 3)&3));
//        printf("\r\n PortZ:"); 
//        printBinary((IO_PortsReadPort(BUMPERS_PORT)) & (PIN3 | PIN4));
//        printBinary(20);
        DELAY(500000);
    }
}

#endif
