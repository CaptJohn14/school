
#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BumperService.h"
#include "ES_Events.h"
#include "roach.h"
#include <stdio.h>
//#include "roach.h"

#define TIMER_0_TICKS 100

static uint8_t MyPriority;

uint8_t InitBumperService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    // in here you write your initialization code
    // this includes all hardware and software initialization
    // that needs to occur.
    // post the initial transition event

    ThisEvent.EventType = ES_INIT;
    ES_Timer_InitTimer(SIMPLE_SERVICE_TIMER, TIMER_0_TICKS);

    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }

}

uint8_t PostBumperService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

ES_Event RunBumperService(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    static unsigned char lastBumperRead = 0b0000;
    //    static ES_EventTyp_t lastEvent = BUMPER_RELEASE;
    ES_EventTyp_t currBumperEvent;
    unsigned char currBumperRead = Roach_ReadBumpers();

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
            ES_Timer_InitTimer(SIMPLE_SERVICE_TIMER, TIMER_0_TICKS);
            if (Debounce(currBumperRead)) {
                if (currBumperRead != lastBumperRead) {
                    if (currBumperRead > lastBumperRead) {
                        currBumperEvent = BUMPER_HIT;
                    } else if (currBumperRead < lastBumperRead) {
                        currBumperEvent = BUMPER_RELEASE;
                    }
                    lastBumperRead = currBumperRead;
                    ReturnEvent.EventType = currBumperEvent;
                    ReturnEvent.EventParam = currBumperRead;

#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
                    PostRoachHSM(ReturnEvent);
//                    printf("\r\nPLEASE: %d\r\n", currBumperRead);
#else
                    PostRoachHSM(ReturnEvent);
//                    printf("\r\nPLEASE: %d\r\n", currBumperRead);
#endif   
                }
            }
            break;
//#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
//        default:
//            printf("\r\nEvent: %s\tParam: %d",
//                    EventNames[ThisEvent.EventType], ThisEvent.EventParam);
//            break;
//#endif    
    }

    return ReturnEvent;
}

uint8_t Debounce(unsigned char oldValue) {
    unsigned char myArray[5] = {0};
    int i;
    for(i = 0; i<4; i++)
    {
        myArray[i] = Roach_ReadBumpers();        
    }
    int j;
    for(j = 0; j<3; j++)
    {
        if(myArray[j] != myArray[j+1])
        {
            return FALSE;
        }
    }
    return TRUE;
}