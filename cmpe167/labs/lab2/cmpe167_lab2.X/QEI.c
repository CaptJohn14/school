/*==============================================================================
 * Name: Johnson Le
 * Date: 1/29/2019
 * File: QEI.c
 * Desc: QEI for lab 2.
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "BOARD.h"
#include <stdio.h>
#include <xc.h>
#include "QEI.h"

//#define is24
#define is96
static int count = 0;
static char curr = 0;
static char old = 0b11;

/*==============================================================================
 * @function QEI_Init(void)
 * @param none
 * @brief  Enables the Change Notify peripheral and sets up the interrupt, anything
 *         else that needs to be done to initialize the module. 
 * @return SUCCESS or ERROR (as defined in BOARD.h)
 =============================================================================*/
char QEI_Init(void) {
    mCNOpen(CN_ON, (CN16_ENABLE | CN15_ENABLE), 0); // CNA CNB 
    int temp = PORTD;
    ConfigIntCN(CHANGE_INT_ON | CHANGE_INT_PRI_1);
    //anything else that needs to occur during init goes here
}

/*==============================================================================
 * @function QEI_GetPosition(void) 
 * @param none
 * @brief This function returns the current count of the Quadrature Encoder in ticks.      
 =============================================================================*/
int QEI_GetPosition(void) {
    static char readPort = 0;

    //    static int count = 0;

    readPort = PORTD; // this read is required to make the interrupt work
    INTClearFlag(INT_CN);
    //anything else that needs to happen goes here
    curr = (readPort & 0b011000000) >> 6;

#ifdef is24
    int max = 24;
    if (curr == 0 && count < max && old == 0b10) {
        count++;
    } else if (curr == 0 && count != 0 && old == 0b01) {
        count--;
    }
#endif
#ifdef is96
    static int max = 96;
    static int min = -96;
    switch (curr) {
        case 0b11:
            if (old == 0b01) {
                count++;
                if (count % 4 != 0 && count <= (max - 3)) {
                    count = count + (count % 4);
                }
            } else if (old == 0b10) {
                count--;
                if (count % 4 != 0 && count >= (min + 3)) {
                    count = count - (count % 4);
                }
            }
            break;
        case 0b10:
            //clockwise
            if (old == 0b11) {
                count++;
            } else if (old == 0b00) {
                count--;
            }
            break;
        case 0b00:
            if (old == 0b10) {
                count++;
            } else if (old == 0b01) {
                count--;
            }
            break;
        case 0b01:
            //counterclockwise
            if (old == 0b00) {
                count++;
            } else if (old == 0b11) {
                count--;
            }
            break;
        default:
            break;
    }
    if (count >= max) {
        count = max;
    } else if (count <= min) {
        count = min;
    }
#endif
    old = curr;
    return count;
}

/*==============================================================================
 * @Function QEI_ResetPosition(void) 
 * @param  none
 * @return none
 * @brief  Resets the encoder such that it starts counting from 0.
 =============================================================================*/
void QEI_ResetPosition() {
    count = 0;
    curr = 0;
    old = 0b11;
}

/*==============================================================================
 * ISR
 =============================================================================*/
//void __ISR(_CHANGE_NOTICE_VECTOR, ipl1auto) ChangeNotice_Handler(void) {
//    static char readPort = 0;
//    readPort = PORTD; // this read is required to make the interrupt work
//    INTClearFlag(INT_CN);
//    //anything else that needs to happen goes here
//}