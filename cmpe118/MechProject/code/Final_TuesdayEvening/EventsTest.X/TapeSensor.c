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
#include <LED.h>

static unsigned int TAPE_SensorRead[N_TapeSensors];
static unsigned char TapeRead = 0b0;
int i = 0;

/*==============================================================================
 * Initialize TAPE. Tried adding pins here but didn't work. so its now empty lol
 =============================================================================*/
char TAPE_Init(void) {
    AD_AddPins(AD_PORTW3 | AD_PORTW4 | AD_PORTW5 | AD_PORTW6);
    //    AD_AddPins(AD_PORTV5 | AD_PORTV6 |AD_PORTV7 | AD_PORTV8);
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);
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


unsigned char TAPE_Read(void) {
    TAPE_AnalogRead();
    //binary masking. High if sees black.
    for (i = 0; i < N_TapeSensors; i++) {
        if (((1 << i) & TapeRead) == 0 && TAPE_SensorRead[i] > TAPE_IntoDark) {
            TapeRead = TapeRead | (1 << i);
        } else if (((1 << i) & TapeRead) != 0 && TAPE_SensorRead[i] <= TAPE_IntoLight) {
            TapeRead = TapeRead & ~(1 << i);
        }
    }
    setLED();
    return TapeRead;
}

/*==============================================================================
 * Function: static void TAPE_AnalogRead(void);
 * Use: prints out (: thats it.
 =============================================================================*/
void TAPE_PrintAnalogAll(void) {
    TAPE_AnalogRead();
    for (i = 0; i < N_TapeSensors; i++) {
        printf("%d: %u \r\n", i, TAPE_SensorRead[i]);
    }
}

void TAPE_PrintDigitalAll(void) {
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

/*==============================================================================
 * Function: printBinary
 * Use: recursion. prints given param in binary
 =============================================================================*/
static void printBinary(char n) {
    if (n > 1)
        printBinary(n / 2);
    printf("%d", n % 2);
}

/*==============================================================================
 * Function: void setLED(void)
 * Use: turns on LED based on tape sensor
 =============================================================================*/
static void setLED(void) {
    LED_SetBank(LED_BANK2, TapeRead & 0b0010);
    LED_SetBank(LED_BANK3, TapeRead & 0b1001);
    //    LED_SetBank(LED_BANK1, (TapeRead & 0b0100) >> 1);

    LED_SetBank(LED_BANK1, TapeRead & 0b0100);
}

/*==============================================================================
 * Test Harness
 =============================================================================*/
//#define TAPE_TEST
#ifdef TAPE_TEST

static int k;
#define DELAY(x) for(k=0; k<x; k++) 
#include <stdio.h>

int main(void) {
    BOARD_Init();
    AD_Init();
    LED_Init();

    TAPE_Init();

    while (1) {
        printf("%d\r\n", TAPE_Read());
        TAPE_PrintDigitalAll();
        TAPE_PrintAnalogAll();
        DELAY(500000);
    }
}

#endif

