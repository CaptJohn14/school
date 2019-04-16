/*
 * File: TemplateFSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Flat State Machine to work with the Events and Services
 * Frameword (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that this file
 * will need to be modified to fit your exact needs, and most of the names will have
 * to be changed to match your code.
 *
 * This is provided as an example and a good place to start.
 *
 *Generally you will just be modifying the statenames and the run function
 *However make sure you do a find and replace to convert every instance of
 *  "Template" to your current state machine's name
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

#include "TemplateFSM.h"
#include "roach.h"
#include <BOARD.h>
//Uncomment these for the Roaches
//#include "roach.h"
//#include "RoachFrameworkEvents.h"
#include <stdio.h>


/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define TURN_TICK 1000
#define MOTOR_SPEED 100
#define MOTOR_OFF 0
#define NEG_MOTOR_SPEED -100
#define BUMPER_FRONT_LEFT 0b0001
#define BUMPER_FRONT_RIGHT 0b0010
#define BUMPER_BACK_LEFT 0b0100
#define BUMPER_BACK_RIGHT 0b1000

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine.*/


/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/

/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

typedef enum {
    //    InitPState,
    //    FirstState,
    FSMState_Init,
    FSMState_Hide,
    FSMState_Forward,
    FSMState_Left,
    FSMState_Right,
} RoachFSMState_t;

static const char *StateNames[] = {
	"FSMState_Init",
	"FSMState_Hide",
	"FSMState_Forward",
	"FSMState_Left",
	"FSMState_Right",
};

//static TemplateFSMState_t CurrentState = InitPState; // <- change enum name to match ENUM
static RoachFSMState_t CurrentState = FSMState_Init;
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateFSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */

uint8_t InitRoachFSM(uint8_t Priority) {
    MyPriority = Priority;
    //start with init stage
    CurrentState = FSMState_Init;
    //then transition into the first state.
    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostTemplateFSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostRoachFSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunTemplateFSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the flat state machine,
 *        as this is called any time a new event is passed to the event queue. This
 *        function will be called recursively to implement the correct order for a
 *        state transition to be: exit current state -> enter next state using the
 *        ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       Returns ES_NO_EVENT if the event have been "consumed."
 * @author J. Edward Carryer, 2011.10.23 19:25 */

ES_Event RunRoachFSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    RoachFSMState_t nextState;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case FSMState_Init:
            /*Init: Not sure yet :D. Transition to idle*/
            if (ThisEvent.EventType == ES_INIT) {
                nextState = FSMState_Hide;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
                printf("\r\n");
            }
            break;
        case FSMState_Hide:
            /*Idle: Motors off. Read light. Reset Timer. ->Forward of bright.*/
            // lightState == Dark
            //read light level
            //if light, go to Forward state
            Roach_RightMtrSpeed(0);
            Roach_LeftMtrSpeed(0);
            switch (ThisEvent.EventType) {
                case INTO_LIGHT:
                    nextState = FSMState_Forward;
                    makeTransition = TRUE;
                    Roach_BarGraph(0);
                    break;
                case INTO_DARK:
                    Roach_BarGraph(12);
                    break;
                default:
                    break;
            }

            break;
        case FSMState_Forward:
            /*Forward: Read light. Moves till bump or dark*/
            //turn on motor on
            Roach_RightMtrSpeed(MOTOR_SPEED);
            Roach_LeftMtrSpeed(MOTOR_SPEED);
            switch (ThisEvent.EventType) {
                case INTO_LIGHT:
                    break;
                case INTO_DARK:
                    nextState = FSMState_Hide;
                    makeTransition = TRUE;
                    break;
                case BUMPER_HIT:
                    Roach_RightMtrSpeed(MOTOR_OFF);
                    Roach_LeftMtrSpeed(MOTOR_OFF);
                    if (BUMPER_FRONT_RIGHT & ThisEvent.EventParam) {
                        nextState = FSMState_Left;
                        makeTransition = TRUE;
                        ES_Timer_StartTimer(TURN_TIMER);
                    } else if (BUMPER_FRONT_LEFT & ThisEvent.EventParam) {
                        nextState = FSMState_Right;
                        makeTransition = TRUE;
                        ES_Timer_StartTimer(TURN_TIMER);
                    }
                default:
                    break;
            }
            break;
        case FSMState_Left:
            //turn left. right off, left neg. if timer is done, trans to forward
            //if one/both back hits, trans to forward. 
            Roach_RightMtrSpeed(MOTOR_OFF);
            Roach_LeftMtrSpeed(NEG_MOTOR_SPEED);

            if (ThisEvent.EventType == ES_TIMEOUT) {
                ES_Timer_StopTimer(TURN_TIMER);
                nextState = FSMState_Forward;
                makeTransition = TRUE;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
            } else if (ThisEvent.EventType == BUMPER_HIT && ((ThisEvent.EventParam & BUMPER_BACK_LEFT) || (ThisEvent.EventParam & BUMPER_BACK_RIGHT))) {
                ES_Timer_StopTimer(TURN_TIMER);
                nextState = FSMState_Forward;
                makeTransition = TRUE;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
            }
            break;
        case FSMState_Right:
            Roach_RightMtrSpeed(NEG_MOTOR_SPEED);
            Roach_LeftMtrSpeed(MOTOR_OFF);

            if (ThisEvent.EventType == ES_TIMEOUT) {
                ES_Timer_StopTimer(TURN_TIMER);
                nextState = FSMState_Forward;
                makeTransition = TRUE;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
            } else if (ThisEvent.EventType == BUMPER_HIT && ((ThisEvent.EventParam & BUMPER_BACK_LEFT) || (ThisEvent.EventParam & BUMPER_BACK_RIGHT))) {
                ES_Timer_StopTimer(TURN_TIMER);
                nextState = FSMState_Forward;
                makeTransition = TRUE;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
            }
            break;
        default:
            break;
    }
    if (makeTransition == TRUE) {
        RunRoachFSM(EXIT_EVENT);
        CurrentState = nextState;
        RunRoachFSM(ENTRY_EVENT);
    }

    ES_Tail();
    return ThisEvent;
}



/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
