/*
 * File: LightService.h

 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "LightService.h"
#include "ES_Events.h"
#include <stdio.h>
#include "roach.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define DARK_THRESHOLD 550
#define LIGHT_THRESHOLD 450

#define TIMER_1_TICKS 50

static uint8_t MyPriority;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateService(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateService function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitLightService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    // in here you write your initialization code
    // this includes all hardware and software initialization
    // that needs to occur.
    // post the initial transition event

    ThisEvent.EventType = ES_INIT;
    ES_Timer_InitTimer(LIGHT_TIMER, TIMER_1_TICKS);

    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }

}

/**
 * @Function PostTemplateService(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostLightService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

ES_Event RunLightService(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
    
    static ES_EventTyp_t lastLightEvent = INTO_DARK;
    ES_EventTyp_t currLightEvent;
    unsigned int currLightRead = Roach_LightLevel();
    
    switch (ThisEvent.EventType) {
        case ES_INIT:
            // No hardware initialization or single time setups, those
            // go in the init function above.
            //
            // This section is used to reset service for some reason
            break;
        case ES_TIMERACTIVE:
            break;
        case ES_TIMERSTOPPED:
            break;
        case ES_TIMEOUT:
            ES_Timer_InitTimer(LIGHT_TIMER, TIMER_1_TICKS);
            if(lastLightEvent==INTO_DARK && currLightRead<=LIGHT_THRESHOLD){
                currLightEvent = INTO_LIGHT;
            }
            else if(lastLightEvent==INTO_LIGHT && currLightRead>=DARK_THRESHOLD){
                currLightEvent= INTO_DARK;
            }
            else{
                currLightEvent = lastLightEvent;
            }
//            if (currLightRead >= LIGHT_THRESHOLD) {
//                currLightEvent = INTO_DARK;
//            } 
//            else{
//                currLightEvent = INTO_LIGHT;
//            }
            if (currLightEvent != lastLightEvent) {
                lastLightEvent = currLightEvent;
                ReturnEvent.EventType = currLightEvent;
                ReturnEvent.EventParam = currLightRead;

#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
                PostGenericService(ReturnEvent);
#else
                PostLightService(ReturnEvent);
#endif   
            }
            break;
#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
        default:
            printf("\r\nEvent: %s\tParam: %d",
                    EventNames[ThisEvent.EventType], ThisEvent.EventParam);
            break;
#endif    
    }
    //    static ES_EventTyp_t lastLightEvent = INTO_LIGHT;
    //    ES_EventTyp_t currLightEvent;
    // 
    //
    //    unsigned int lightLevel = Roach_LightLevel();
    //    switch (ThisEvent.EventType) {
    //        case ES_INIT:
    //            // No hardware initialization or single time setups, those
    //            // go in the init function above.
    //            //
    //            // This section is used to reset service for some reason
    //            break;
    //        case ES_TIMERACTIVE:
    //            break;
    //        case ES_TIMERSTOPPED:
    //            break;
    //        case ES_TIMEOUT:
    //            ES_Timer_InitTimer(LIGHT_TIMER, TIMER_1_TICKS);
    //            if(lightLevel < LIGHT_THRESHOLD){
    //                lastLightEvent = INTO_LIGHT;
    //            }
    //            else if(lightLevel > LIGHT_THRESHOLD){
    //                lastLightEvent = INTO_DARK;
    //            }
    //            if(lastLightEvent != currLightEvent){
    //    
    //                lastLightEvent= currLightEvent;
    //                ReturnEvent.EventType = currLightEvent;
    //                ReturnEvent.EventParam = lightLevel;
    //
    //#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
    //                PostGenericService(ReturnEvent);
    //#else
    //                PostLightService(ReturnEvent);
    //#endif   
    //            }
    //            break;
    //#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
    //        default:
    //            printf("\r\nEvent: %s\tParam: %d",
    //                    EventNames[ThisEvent.EventType], ThisEvent.EventParam);
    //            break;
    //#endif    
    //    }

    return ReturnEvent;
}
/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/

