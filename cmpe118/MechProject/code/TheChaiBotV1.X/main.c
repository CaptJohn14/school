/*==============================================================================
 * File Name: main.c
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: CMPE118 last project
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include <AD.h>
#include <BOARD.h>
#include <serial.h>
#include <stdio.h>
#include <IO_Ports.h>
#include "TapeSensor.h"
#include <pwm.h>
#include <ES_Configure.h>
#include "ES_Framework.h"

/*==============================================================================
 * Macros and Declarations
 =============================================================================*/
static int k;
#define DELAY(x) for(k=0; k<x; k++) 
#define NUM_PING 1
#define PING_PORT PORTZ
#define TRIG_PATTERN (1<<7)
#define PING_PATTERN (1<<3)
#define ECHO_1  3
#define TRIG_TICKS 10000
#define MICROSECOND_TICKS 1000

static int trig = 0;
static int echo = 0;

enum {
    TRIGGER_TIMER,
    ECHO_TIMER
} Timers;

/*==============================================================================
 * Functions
 =============================================================================*/

/*==============================================================================
 * Main Code
 =============================================================================*/
int main(void) {
    /*==========================================================================
     * Initialization
    ==========================================================================*/
    BOARD_Init();
    AD_Init();
    int i = 0;
    //    for(i=0; i<N_TapeSensors; i++){
    //        AD_AddPins()
    //    }
    AD_AddPins(AD_PORTW3 | AD_PORTW4);
    IO_PortsSetPortDirection(PING_PORT, PING_PATTERN);
    char ReadTape = 0b0;
    unsigned int end;
    unsigned int highcount;
    unsigned int tick;
    unsigned int distance;
    /*==========================================================================
     * While Loop: Reads Tape. Checks if dark or light. 
    ==========================================================================*/
    while (1) {

        TAPE_PrintAnalogAll();
        DELAY(1000000);
    }
}
