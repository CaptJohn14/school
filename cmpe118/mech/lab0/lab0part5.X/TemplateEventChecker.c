/*
 * File:   mainPart5.c
 * Author:  Johnson Le
 *          Talin Hallajian
 * 
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
#include "roach.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define BATTERY_DISCONNECT_THRESHOLD 175
#define LIGHT_LEVEL_THRESHOLD 600

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

#ifdef EVENTCHECKER_TEST
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};

void PrintEvent(void);

/*******************************************************************************
 * main                                                       *
 ******************************************************************************/
void main(void) {
    BOARD_Init();
    Roach_Init();
    /* user initialization code goes here */

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

/*******************************************************************************
 * Check Battery function example                                                         *
 ******************************************************************************/
uint8_t DisplayBatteryVoltage(void) {
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
        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
    }
    return (returnVal);
}

/*******************************************************************************
 * Event for detecting light          
 * TODO: The light parameter displayed is always around the threshold. 
 ******************************************************************************/
uint8_t CheckLightDetection(void) {
    static ES_EventTyp_t lastLightEvent = INTO_DARK;
    ES_EventTyp_t currLightEvent;
    ES_Event lightEvent;
    unsigned int lightLevel = Roach_LightLevel();
    lightEvent.EventParam = lightLevel;
    uint8_t returnVal = FALSE;
    if (lightLevel > LIGHT_LEVEL_THRESHOLD) {
        currLightEvent = INTO_DARK;
    } else {
        currLightEvent = INTO_LIGHT;
    }
    if (currLightEvent != lastLightEvent) {
        lightEvent.EventType = currLightEvent;
        returnVal = TRUE;
        lastLightEvent = currLightEvent;
#ifndef EVENTCHECKER_TEST
        PostGenericService(lightEvent);
#else
        SaveEvent(lightEvent);
#endif   
    }
    return (returnVal);
}

/*******************************************************************************
 * Event for detecting bumper has been hit          
 ******************************************************************************/


uint8_t bumperHit(void){
    static unsigned char lastBumperRead = 0b0000;
    ES_EventTyp_t currBumperEvent;
    ES_Event bumperEvent;
    uint8_t returnVal = FALSE;
    unsigned char currBumperRead = Roach_ReadBumpers();
    if(currBumperRead != lastBumperRead){
        if(currBumperRead>lastBumperRead){
            currBumperEvent = BUMPER_HIT;
        }
        else if(currBumperRead<lastBumperRead){
            currBumperEvent = BUMPER_RELEASE;
        }
        lastBumperRead = currBumperRead;
        bumperEvent.EventType = currBumperEvent;
        bumperEvent.EventParam = currBumperRead;
        returnVal = TRUE;
        
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
        PostGenericService(bumperEvent);
#else
        SaveEvent(bumperEvent);
#endif   
    }
    return (returnVal);
}

/*******************************************************************************
 * prints                                                           *
 ******************************************************************************/
void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: %d", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
}

#endif