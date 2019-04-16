/*==============================================================================
 * File Name: TapeSensor.c
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Source for Reading TapeSensor
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "TapeSensor.h"
#include <AD.h>
#include <serial.h>
#include <stdio.h>
#include <BOARD.h>

static unsigned int TAPE_SensorRead[N_TapeSensors];
int TAPE_LightStatus[N_TapeSensors];
int i = 0;

/*==============================================================================
 * Initialize TAPE. Tried adding pins here but didn't work. so its now empty lol
 =============================================================================*/
char TAPE_Init(void) {

    return SUCCESS;
}

/*==============================================================================
 * Function: TAPE_Read(void);
 * Returns: 8 bit number.
 * Use: Function will return an 8-bit. Value is dependant on which TAPE SENSOR
 *      is in the dark. If the first is in dark and the rest is in light than it
 *      will return 0b00000001
 *      a HIGH indicates it sees black
 *      a LOW indicates it ain't black
 =============================================================================*/
char TAPE_Read(void) {
    char ret = 0b0;
    TAPE_AnalogRead();
    
    //binary masking. High if sees black.
    for (i = 0; i < N_TapeSensors; i++) {
        if (TAPE_SensorRead[i] > TAPE_IntoDark) {
            ret = ret | (1 << i);
        }
    }
    return ret;
}

/*==============================================================================
 * Function: static void TAPE_AnalogRead(void);
 * Use: prints out (: thats it.
 =============================================================================*/
void TAPE_PrintAnalogAll(void){
    TAPE_AnalogRead();
    for (i = 0; i < N_TapeSensors; i++) {
        printf("%d: %u \r\n", i, TAPE_SensorRead[i]);
    }
}

void TAPE_PrintDigitalAll(void){
    printf("0b");
    printBinary(TAPE_Read());
    printf("\r\n");
}

/*==============================================================================
 * Function: static void TAPE_AnalogRead(void);
 * Use: private function that reads all tape sensors and throws into array
 =============================================================================*/
static void TAPE_AnalogRead(void) {
    for (i = 0; i < N_TapeSensors; i++) {
        TAPE_SensorRead[i] = AD_ReadADPin(AD_PORTW_Pin(i));
    }
}

static void printBinary(char n) {
    if (n > 1)
        printBinary(n / 2);
    printf("%d", n % 2);
}
