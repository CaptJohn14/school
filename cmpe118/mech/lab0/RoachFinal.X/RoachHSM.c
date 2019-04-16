/*
 * File: TemplateSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel Elkaim and Soja-Marie Morgens
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is another template file for the SubHSM's that is slightly differet, and
 * should be used for all of the subordinate state machines (flat or heirarchical)
 *
 * This is provided as an example and a good place to start.
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 * 01/15/12 11:12 jec      revisions for Gen2 framework
 * 11/07/11 11:26 jec      made the queue static
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "RoachHSM.h"
#include "AvoidSubHSM.h"
#include "roach.h" //#include all sub state machines called
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//Include any defines you need to do
#define SLOW_SPEED 100
#define NEG_SLOW_SPEED -100
#define HALFSEC_TICK 500

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


typedef enum {
    //    InitPState,
    //    FirstState,
    State_Init,
    State_Hide,
    State_Forward,
    State_Avoid,
    State_Dance,
} RoachHSMState_t;

static const char *StateNames[] = {
	"State_Init",
	"State_Hide",
	"State_Forward",
	"State_Avoid",
	"State_Dance",
};


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine
   Example: char RunAway(uint_8 seconds);*/
/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static RoachHSMState_t CurrentState = State_Init; // <- change enum name to match ENUM
static uint8_t MyPriority;
//extern int b = 0;




/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitRoachHSM(uint8_t Priority) {
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = State_Init;
    // post the initial transition event
    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostTemplateHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostRoachHSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunTemplateHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the heirarchical state
 *        machine, as this is called any time a new event is passed to the event
 *        queue. This function will be called recursively to implement the correct
 *        order for a state transition to be: exit current state -> enter next state
 *        using the ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       The lower level state machines are run first, to see if the event is dealt
 *       with there rather than at the current level. ES_EXIT and ES_ENTRY events are
 *       not consumed as these need to pass pack to the higher level state machine.
 * @author J. Edward Carryer, 2011.10.23 19:25
 * @author Gabriel H Elkaim, 2011.10.23 19:25 */
ES_Event RunRoachHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    RoachHSMState_t nextState;

    ES_Tattle();

    switch (CurrentState) {
        case State_Init:
            if (ThisEvent.EventType == ES_INIT) {
                //init sub fsm when implemented
                InitAvoidSubHSM();
                nextState = State_Hide;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
                ES_Timer_SetTimer(DANCE_TIMER, DANCE_TICK);
                ES_Timer_StartTimer(DANCE_TIMER);

            }
            break;

        case State_Forward:
            if (ThisEvent.EventType == INTO_DARK) {
                nextState = State_Hide;
                makeTransition = TRUE;
                break;
            }
            ThisEvent = RunAvoidSubHSM(ThisEvent);

            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    nextState = State_Hide;
                    makeTransition = TRUE;
                    ES_Event test;
                    test.EventType = INTO_LIGHT;
                    PostRoachHSM(test);
                    break;
                default:
                    break;
            }
            break;
        case State_Hide:
            Roach_RightMtrSpeed(MOTOR_OFF);
            Roach_LeftMtrSpeed(MOTOR_OFF);
            switch (ThisEvent.EventType) {
                case INTO_LIGHT:
                    nextState = State_Forward;
                    makeTransition = TRUE;
                    break;
                case BUMPER_HIT:
                    ES_Timer_SetTimer(TURN_TIMER, HALFSEC_TICK);
                    if (BUMPER_FRONT_RIGHT & ThisEvent.EventParam) {
                        Roach_RightMtrSpeed(MOTOR_OFF);
                        Roach_LeftMtrSpeed(NEG_SLOW_SPEED);
                    }
                    if (BUMPER_FRONT_LEFT & ThisEvent.EventParam) {
                        Roach_RightMtrSpeed(NEG_SLOW_SPEED);
                        Roach_LeftMtrSpeed(MOTOR_OFF);
                    }
                    if ((ThisEvent.EventParam & BUMPER_BACK_LEFT) || (ThisEvent.EventParam & BUMPER_BACK_RIGHT)) {
                        Roach_RightMtrSpeed(SLOW_SPEED);
                        Roach_LeftMtrSpeed(SLOW_SPEED);
                    }
                    break;
                case BUMPER_RELEASE:
                    ES_Timer_StartTimer(TURN_TIMER);
                    //start timer;
                    break;
                case ES_TIMEOUT:
                    ES_Timer_StopTimer(TURN_TIMER);
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunRoachHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunRoachHSM(ENTRY_EVENT); // <- rename to your own Run function
    }


    ES_Tail();
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
