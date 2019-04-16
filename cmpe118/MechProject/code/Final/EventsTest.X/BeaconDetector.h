/*==============================================================================
 * File Name: BeaconDetector.h
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Header for Reading the Beacon Detector
 =============================================================================*/

#ifndef BEACONDETECTOR_H
#define	BEACONDETECTOR_H

/*==============================================================================
 * Libraries
 =============================================================================*/

/*==============================================================================
 * Macros and Declarations
 =============================================================================*/

#define BEACON_PORT PORTV
#define BEACON_PATTERN PIN3
#define BEACON_PIN AD_PORTV3

#define IR_SUCCESS 1
#define IR_FAILURE 0

#define IR_HIGH 620
#define IR_LOW  470

#define IR_HISTORY_MAX 10

/*==============================================================================
 * Initialize Beacon Detector  
 =============================================================================*/
int Beacon_Init(void);

/*==============================================================================
 * Read the current IR Value
 =============================================================================*/
int Beacon_ReadValue(void);

/*==============================================================================
 *  Print the current IR value
 =============================================================================*/
void Beacon_PrintValue(void);
int Beacon_FindEnemy(void);
/*==============================================================================
 * The average of the last 10 values 
 =============================================================================*/
//int Beacon_HistoryAvg(void);

/*==============================================================================
 * Find the Peak Value // used for rotating and finding the peak value
 *                     // if Value > IR_HIGH and > all the previous values, 
 *                        plus the threshold, return 1
 *                     // if Value > IR_HIGH and < all the previous values, 
 *                        return 0 
 =============================================================================*/
int Beacon_FindPeak(void);

#endif	/* BEACONDETECTOR_H */

