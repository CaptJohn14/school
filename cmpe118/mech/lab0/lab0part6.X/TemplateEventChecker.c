/*
 * File:   mainPart5.c
 * Author:  Johnson Le
 *          Talin Hallajian
 * 
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include "TemplateEventChecker.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"
#include "roach.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIGHT_LEVEL_THRESHOLD 555

/*******************************************************************************
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/
//#define EVENTCHECKER_TESTghk
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
 * prints                                                           *
 ******************************************************************************/
void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: %d", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
}

#endif