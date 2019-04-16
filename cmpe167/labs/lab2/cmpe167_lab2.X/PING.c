/*******************************************************************************
 * INCLUDES                                                                    *
 ******************************************************************************/
#include "BOARD.h"
#include <xc.h>
#include <plib.h>
#include <proc/p32mx320f128h.h>
#include "PING.h"
#include "timers.h"
#include "Oled.h"
#include <stdio.h>

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define PIN_TRIG(x) TRISDbits.TRISD11=x //pin35
#define TRIG_SET(x) LATDbits.LATD11=x
#define TRIG_READ PORTDbits.RD11

#define PIN_ECHO TRISDbits.TRISD5
#define ECHO_READ PORTDbits.RD5

#define ON 1
#define OFF 0
#define PIN_INPUT 1
#define PIN_OUTPUT 0

static unsigned int t1 = 0;
static unsigned int t2 = 0;
static unsigned int t_flight = 0;

static char string[40];

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @function    PING_Init(void)
 * @brief       Sets up both the timer and Change notify peripherals along with their
 *              respective interrupts.  Also handles any other tasks needed such as pin 
 *              I/O directions, and any other things you need to initialize the sensor.
 *              TIMERS library must be inited before this library.
 * @return      SUCCESS or ERROR (as defined in BOARD.h)
 */
char PING_Init(void) {
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_64, 0xFFFF); //that is not the time you want to use
    INTClearFlag(INT_T4);
    INTSetVectorPriority(INT_TIMER_4_VECTOR, 3);
    INTSetVectorSubPriority(INT_TIMER_4_VECTOR, 3);
    INTEnable(INT_T4, INT_ENABLED);
    mCNOpen(CN_ON, CN14_ENABLE, 0);
    int temp = PORTD;
    ConfigIntCN(CHANGE_INT_ON | CHANGE_INT_PRI_1);

    //Anything else that needs to
    TIMERS_Init();
    //    PR4 = 6.25; //start interrupt at 10us
    PIN_TRIG(PIN_OUTPUT);
    LATDbits.LATD11 = 0;

}

/**
 * @function    PING_GetDistance(void)
 * @brief       Returns the calculated distance in mm using the sensor model determined
 *              experimentally. 
 *              No I/O should be done in this function
 * @return      distance in mm
 */
#define slope 1.0031
#define y_int 0.9737
unsigned int PING_GetDistance(void) {
    static unsigned int t_expected = 0;
    static unsigned int t_experiment = 0;
    t_experiment = t_flight/5.8;
    t_expected = (t_experiment-y_int)/slope;
    if(t_expected > 1000){
        t_expected = 1000;
    }
    return t_expected;
//    return (t_flight/(5.8));
}

/**
 * @function    PING_GetTimeofFlight(void)
 * @brief       Returns the raw microsecond duration of the echo from the sensor.
 *              NO I/O should be done in this function.
 * @return      time of flight in uSec
 */
unsigned int PING_GetTimeofFlight(void) {
//    sprintf(string, "time: %u\nt1: %u\nt2: %u", t_flight, t1, t2);
//    OledDrawString(string);
//    OledUpdate();
//    OledClear(0);
    return t_flight;
}

void __ISR(_CHANGE_NOTICE_VECTOR, ipl1auto) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    INTClearFlag(INT_CN);
    //Anything else that needs to occur goes here
    switch (ECHO_READ) {
        case ON:
            t1 = TIMERS_GetMicroSeconds();
            break;
        case OFF:
            t2 = TIMERS_GetMicroSeconds();
            t_flight = t2 - t1;
            break;
        default:
            break;
    }
}

void __ISR(_TIMER_4_VECTOR, ipl3auto) Timer4IntHandler(void) {
    INTClearFlag(INT_T4);
    //Anything else that needs to occur goes here
    switch (TRIG_READ) {
        case OFF:
            TRIG_SET(ON);
            PR4 = 6.25;
            break;
        default:
            TRIG_SET(OFF);
            PR4 = 37500;
            break;
    }
}