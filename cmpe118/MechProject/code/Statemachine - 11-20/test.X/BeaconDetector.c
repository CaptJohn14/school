/*==============================================================================
 * File Name: BeaconDetector.c
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Source for Reading the Beacon Detector
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "BeaconDetector.h"
#include <AD.h>
#include <serial.h>
#include <stdio.h>
#include <BOARD.h>
#include <IO_Ports.h>
#include <pwm.h>

#define PEAK_THRESHOLD 5

static int f = 0;
//static int g = 0;
static int h = 0;
static int beacon_avg = 0;
//static int BeaconHistory[IR_HISTORY_MAX] = {0};
static int LastBeaconValue = 0;

/*==============================================================================
 * Initialize Beacon Detector // nothing? 
 =============================================================================*/
int Beacon_Init(void) {
    return IR_SUCCESS;
}

int Beacon_ReadValue(void) {
    return (AD_ReadADPin(BEACON_PIN));
}

void Beacon_PrintValue(void){
    printf("\r\n Beacon: %d", Beacon_ReadValue());                       
}

//int Beacon_HistoryAvg(void) {
//    if (f < IR_HISTORY_MAX) {
//        BeaconHistory[f] = Beacon_ReadValue();
//        f++;
//        for (h = 0; h < IR_HISTORY_MAX; h++) {
//            beacon_avg += BeaconHistory[h];
//        }
//        beacon_avg = beacon_avg / IR_HISTORY_MAX;
//        return beacon_avg;
//    }
//    if (f = IR_HISTORY_MAX) {
//        f = 0;
//    }
//}

//int Beacon_FindPeak(void) {
//    if (f < IR_HISTORY_MAX) {
//        Beacon_HistoryAvg(); 
//    } else {
//        int peak;
//        peak = Beacon_HistoryAvg();
//        if (peak > LastBeaconAvg) {
//            LastBeaconAvg = peak;
//            printf("\r\n Avg: ", peak);
//            return 1;
//        }
//        if (peak < LastBeaconAvg) {
//            LastBeaconAvg = peak;
//            printf("\r\n Avg: ", peak);
//            return 0;
//        }
//        f = 0;
//    }
//}

int Beacon_FindPeak(void) {
    int value = Beacon_ReadValue();
    if ((value > (LastBeaconValue + PEAK_THRESHOLD))&&(value > IR_HIGH)) {
        LastBeaconValue = value;
        return 1;
    }
    return 0;
}