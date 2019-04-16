/*
 * File:   TemplateEventChecker.c
 * Author: Gabriel Hugh Elkaim
 *
 * Template file to set up typical EventCheckers for the  Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 *
 * This EventCheckers file will work with both FSM's and HSM's.
 *
 * Remember that EventCheckers should only return TRUE when an event has occured,
 * and that the event is a TRANSITION between two detectable differences. They
 * should also be atomic and run as fast as possible for good results.
 *
 * This file includes a test harness that will run the event detectors listed in the
 * ES_Configure file in the project, and will conditionally compile main if the macro
 * EVENTCHECKER_TEST is defined (either in the project or in the file). This will allow
 * you to check you event detectors in their own project, and then leave them untouched
 * for your project unless you need to alter their post functions.
 *
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "TemplateEventChecker.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define BATTERY_DISCONNECT_THRESHOLD 175
#define DARK_THRESHOLD 550
#define LIGHT_THRESHOLD 450
#define LIGHT_LEVEL_THRESHOLD 600
/*******************************************************************************
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/

//#define EVENTCHECKER_TEST
#ifdef SIMPLESERVICE_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

/*******************************************************************************
 template check battery function that is what the light checker is based off of. 
 when adding to FSM, comment out the print. edit the SaveEvent to PostTemplateFSM 
 ******************************************************************************/
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
#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
        printf("\r\nEvent: %s\tParam: %d",
                EventNames[thisEvent.EventType], thisEvent.EventParam);
#endif   
    }
    return (returnVal);
}

/*******************************************************************************
 * Event for detecting light          
 ******************************************************************************/
uint8_t CheckLightDetection(void) {
    static ES_EventTyp_t lastLightEvent = INTO_DARK;
    ES_EventTyp_t currLightEvent;
    ES_Event lightEvent;

    unsigned int currLightRead = Roach_LightLevel();
    uint8_t returnVal = FALSE;

    if (lastLightEvent == INTO_DARK && currLightRead <= LIGHT_THRESHOLD) {
        currLightEvent = INTO_LIGHT;
    } else if (lastLightEvent == INTO_LIGHT && currLightRead >= DARK_THRESHOLD) {
        currLightEvent = INTO_DARK;
    } else {
        currLightEvent = lastLightEvent;
    }
    //    if (lightLevel > LIGHT_LEVEL_THRESHOLD) {
    //        currLightEvent = INTO_DARK;
    //    } else {
    //        currLightEvent = INTO_LIGHT;
    //    }
    if (currLightEvent != lastLightEvent) {
        lightEvent.EventType = currLightEvent;
        lightEvent.EventParam = currLightRead;
        returnVal = TRUE;
        lastLightEvent = currLightEvent;
#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
        PostTemplateHSM(thisEvent);
#else
        PostTemplateHSM(lightEvent);
#endif   
#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
        printf("\r\nEvent: %s\tParam: %d",
                EventNames[lightEvent.EventType], lightEvent.EventParam);
#endif   
    }

    return (returnVal);
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
//#ifdef SIMPLESERVICE_TEST
//#include <stdio.h>
//static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
//
//void PrintEvent(void);
//
//void main(void) {
//    BOARD_Init();
//    /* user initialization code goes here */
//
//    // Do not alter anything below this line
//    int i;
//
//    printf("\r\nEvent checking test harness for %s", __FILE__);
//
//    while (1) {
//        if (IsTransmitEmpty()) {
//            for (i = 0; i< sizeof (EventList) >> 2; i++) {
//                if (EventList[i]() == TRUE) {
//                    PrintEvent();
//                    break;
//                }
//
//            }
//        }
//    }
//}
//
//void PrintEvent(void) {
//    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X", eventName,
//            EventNames[storedEvent.EventType], storedEvent.EventParam);
//}
//#endif