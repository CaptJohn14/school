/*==============================================================================
 * 
 =============================================================================*/

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "EventChecker.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"
#include "TapeSensor.h"
#include <stdio.h>
#include "Bumpers.h"
#include <IO_Ports.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define BATTERY_DISCONNECT_THRESHOLD 175

/*******************************************************************************
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/

//#define EVENTCHECKER_TEST
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this EventChecker. They should be functions
   relevant to the behavior of this particular event checker */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function TemplateCheckBattery(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function is a prototype event checker that checks the battery voltage
 *        against a fixed threshold (#defined in the .c file). Note that you need to
 *        keep track of previous history, and that the actual battery voltage is checked
 *        only once at the beginning of the function. The function will post an event
 *        of either BATTERY_CONNECTED or BATTERY_DISCONNECTED if the power switch is turned
 *        on or off with the USB cord plugged into the Uno32. Returns TRUE if there was an 
 *        event, FALSE otherwise.
 * @note Use this code as a template for your other event checkers, and modify as necessary.
 * @author Gabriel H Elkaim, 2013.09.27 09:18
 * @modified Gabriel H Elkaim/Max Dunne, 2016.09.12 20:08 */
uint8_t TemplateCheckBattery(void) {
    static ES_EventTyp_t lastEvent = BATTERY_DISCONNECTED;
    ES_EventTyp_t curEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    uint16_t batVoltage = AD_ReadADPin(BAT_VOLTAGE); // read the battery voltage

    if (batVoltage > BATTERY_DISCONNECT_THRESHOLD) { // is battery connected?
        curEvent = BATTERY_CONNECTED;
    } else {
        curEvent = BATTERY_DISCONNECTED;
    }
    if (curEvent != lastEvent) { // check for change from last time
        thisEvent.EventType = curEvent;
        thisEvent.EventParam = batVoltage;
        returnVal = TRUE;
        lastEvent = curEvent; // update history
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
        //        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
    }
    return (returnVal);
}

static int k;
#define DELAY(x) for(k=0; k<x; k++) 

/* =============================================================================
 * Function: CheckTapeSensors(void)
 =============================================================================*/
uint8_t CheckTapeSensors(void) {
    //    static ES_EventTyp_t lastEvent = TAPE_OnToOff;
    static char lastTapeRead = 0b0;
    ES_EventTyp_t currEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;

    unsigned char currTapeRead = TAPE_Read();

    if (currTapeRead != lastTapeRead) {
        if (currTapeRead > lastTapeRead) {
            currEvent = TAPE_OffToOn;
        } else if (currTapeRead < lastTapeRead) {
            currEvent = TAPE_OnToOff;
        }
        thisEvent.EventType = currEvent;
        thisEvent.EventParam = currTapeRead;
        returnVal = TRUE;
        lastTapeRead = currTapeRead;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        //        SaveEvent(thisEvent);
#endif
    }
    return (returnVal);
}


/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/
#define BUMPER_MASK 0x0f

#define BUMPER_PRESS_MASK 0x0f
#define BUMPER_RELEASE_MASK 0xf0

#define BUMPER_PRESS_EVENT 0x01
#define BUMPER_RELEASE_EVENT 0x08

#define FL_BUMPER_PRESS_EVENT 0x01
#define FR_BUMPER_PRESS_EVENT 0x02

#define FL_BUMPER_RELEASE_EVENT 0x10
#define FR_BUMPER_RELEASE_EVENT 0x20

uint8_t CheckBumpers(void) {
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    static unsigned int bumperStateHistory[2] = {0, 0};
    //Read in current bumper states
    //    printf("poo");
    char currentBumperStates = Bumpers_ReadBumpers();
//    printf("\r\n %d", currentBumperStates);
//    DELAY(50000);
    //make space for the new history spot
    bumperStateHistory[0] <<= 1;
    if (currentBumperStates & FL_BUMPER_PRESS_EVENT) {
        bumperStateHistory[0] += 1;
    }

    bumperStateHistory[1] <<= 1;
    if (currentBumperStates & FR_BUMPER_PRESS_EVENT) {
        bumperStateHistory[1] += 1;
    }

    uint16_t bumperEvent = 0;
    //Check for BUMPER_PRESS
    if ((bumperStateHistory[0] & BUMPER_MASK) == BUMPER_PRESS_EVENT) {
        bumperEvent |= FL_BUMPER_PRESS_EVENT;
    }

    if ((bumperStateHistory[1] & BUMPER_MASK) == BUMPER_PRESS_EVENT) {
        bumperEvent |= FR_BUMPER_PRESS_EVENT;
    }

    //Check for BUMPER_RELEASE
    if ((bumperStateHistory[0] & BUMPER_MASK) == BUMPER_RELEASE_EVENT) {
        bumperEvent |= FL_BUMPER_RELEASE_EVENT;
    }

    if ((bumperStateHistory[1] & BUMPER_MASK) == BUMPER_RELEASE_EVENT) {
        bumperEvent |= FR_BUMPER_RELEASE_EVENT;
    }

    if ((bumperEvent & BUMPER_PRESS_MASK) != 0) { // check for change from last time
        thisEvent.EventType = BUMPER_PRESS;
        thisEvent.EventParam = (uint16_t) bumperEvent;
        returnVal = TRUE;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        printf("\r\n press");
        SaveEvent(thisEvent);
#endif
    }
    if ((bumperEvent & BUMPER_RELEASE_MASK) != 0) { // check for change from last time
        thisEvent.EventType = BUMPER_RELEASE;
        thisEvent.EventParam = (uint16_t) bumperEvent;
        returnVal = TRUE;

#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        printf("\r\n release");
        SaveEvent(thisEvent);
#endif  
    }
}

/* 
 * The Test Harness for the event checkers is conditionally compiled using
 * the EVENTCHECKER_TEST macro (defined either in the file or at the project level).
 * No other main() can exist within the project.
 * 
 * It requires a valid ES_Configure.h file in the project with the correct events in 
 * the enum, and the correct list of event checkers in the EVENT_CHECK_LIST.
 * 
 * The test harness will run each of your event detectors identically to the way the
 * ES_Framework will call them, and if an event is detected it will print out the function
 * name, event, and event parameter. Use this to test your event checking code and
 * ensure that it is fully functional.
 * 
 * If you are locking up the output, most likely you are generating too many events.
 * Remember that events are detectable changes, not a state in itself.
 * 
 * Once you have fully tested your event checking code, you can leave it in its own
 * project and point to it from your other projects. If the EVENTCHECKER_TEST marco is
 * defined in the project, no changes are necessary for your event checkers to work
 * with your other projects.
 */
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    AD_Init();
    AD_AddPins(AD_PORTW3 | AD_PORTW4);
    IO_PortsSetPortInputs(BUMPERS_PORT, BUMPERS_INPUTPATTERN);
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
        if (IsTransmitEmpty()) {
            for (i = 0; i< sizeof (EventList) >> 2; i++) {
                if (EventList[i]() == TRUE) {
                    PrintEvent();
                    break;
                }

            }
        }
    }
}

void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
}
#endif