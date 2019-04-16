/*******************************************************************************
 * INCLUDES                                                                    *
 ******************************************************************************/
#include <stdio.h>
#include "BOARD.h"
#include <xc.h>
#include <plib.h>
#include <proc/p32mx320f128h.h>
#include "CAPTOUCH.h"
#include "Oled.h"
//#include "timers.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
char string[40];
static unsigned int t1 = 0;
static unsigned int t2 = 0;
static unsigned int per = 0;
/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @function    CAPTOUCH_Init(void)
 * @brief       This function initializes the module for use. Initialization is 
 *              done by opening and configuring timer 2, opening and configuring the input capture
 *              peripheral, and setting up the interrupt.
 * @return      SUCCESS or ERROR (as defined in BOARD.h)
 */
char CAPTOUCH_Init(void) {
    OpenTimer2(T2_ON | T2_PS_1_64, 0xFFFF);
    OpenCapture4(IC_EVERY_RISE_EDGE | IC_TIMER2_SRC | IC_ON);
    INTClearFlag(INT_IC4);
    INTSetVectorPriority(INT_INPUT_CAPTURE_4_VECTOR, 1);
    INTSetVectorSubPriority(INT_INPUT_CAPTURE_4_VECTOR, 1);
    INTEnable(INT_IC4, INT_ENABLED);
    // whatever else you need to do to initialize your module
    //    TIMERS_Init();
}

/**
 * @function    CAPTOUCH_IsTouched(void)
 * @brief       Returns TRUE if finger is detected. Averaging of previous measurements 
 *              may occur within this function, however you are NOT allowed to do any I/O
 *              inside this function.
 * @return      TRUE or FALSE
 */
char CAPTOUCH_IsTouched(void) {
//    sprintf(string, "uh: %u us", per);
//    OledDrawString(string);
//    OledUpdate();
//    OledClear(0);
    static char old = FALSE;
    if (old == FALSE && per > 120) {
        old = TRUE;
    } else if (old == TRUE && per < 100) {
        old = FALSE;
    }
    return old;
}

/*==============================================================================
 * Interrupt
 =============================================================================*/
void __ISR(_INPUT_CAPTURE_4_VECTOR, ipl1auto) InputCapture4Handler(void) {
    static int count = 0;
    static unsigned int t_hold = 0;
    static int t = 0;
    INTClearFlag(INT_IC4);
    // IC4BUF contains the timer value when the rising edge occurred.
    t_hold = TMR2 - IC4BUF;
    if (t_hold < 2000) {
        t = t + t_hold;
        count++;
        //    sprintf(string, "IC$BUF: %u\nTMR4: %u\n%d\nwhatup: %u", 0, 0, count++, TMR2-IC4BUF);
        if (count >= 50) {
            per = (t / (count)) / 2;
            t = 0;
            count = 0;
        }
    }
}