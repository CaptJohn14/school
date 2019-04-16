/*
 * File: TemplateSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is for a substate machine. Make sure it has a unique name
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
#include "roach.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
typedef enum {
    AvoidSubState_Init,
    AvoidSubState_Forward,
    AvoidSubState_TurnRight,
    AvoidSubState_TurnLeft,
    AvoidSubState_Dance,
    AvoidSubState_First,
} AvoidSubHSMState_t;

static const char *StateNames[] = {
	"AvoidSubState_Init",
	"AvoidSubState_Forward",
	"AvoidSubState_TurnRight",
	"AvoidSubState_TurnLeft",
	"AvoidSubState_Dance",
	"AvoidSubState_First",
};



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static AvoidSubHSMState_t CurrentState = AvoidSubState_Init; // <- change name to match ENUM
static uint8_t MyPriority;
static uint8_t tog = FALSE;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateSubHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitAvoidSubHSM(void) {
    ES_Event returnEvent;

    CurrentState = AvoidSubState_Init;
    returnEvent = RunAvoidSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunTemplateSubHSM(ES_Event ThisEvent)
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
ES_Event RunAvoidSubHSM(ES_Event ThisEvent) {

    uint8_t makeTransition = FALSE; // use to flag transition
    AvoidSubHSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case AvoidSubState_Init:
            if (ThisEvent.EventType == ES_INIT) {
                nextState = AvoidSubState_Forward;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
            }
            break;
        case AvoidSubState_Forward:
            Roach_RightMtrSpeed(80);
            Roach_LeftMtrSpeed(80);

            switch (ThisEvent.EventType) {
                case INTO_DARK:
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_HIT:
                    ES_Timer_InitTimer(DANCE_TIMER, DANCE_TICK);    //this one to reset after bump
                    Roach_RightMtrSpeed(MOTOR_OFF);
                    Roach_LeftMtrSpeed(MOTOR_OFF);
                    if (b >= 6) {
                        nextState = AvoidSubState_Dance;
                        makeTransition = TRUE;
                        ES_Timer_SetTimer(TURN_TIMER, 1700); //set how long to dance.
                        ES_Timer_StartTimer(TURN_TIMER);
                        ES_Timer_StopTimer(DANCE_TIMER);
                        ES_Timer_SetTimer(DANCE_TIMER, DANCE_TICK);
                        b=0;
                    }
                    else if (BUMPER_FRONT_RIGHT & ThisEvent.EventParam) {
                        nextState = AvoidSubState_TurnLeft;
                        makeTransition = TRUE;
                        ES_Timer_StartTimer(TURN_TIMER);
                            b++;
                    } else if (BUMPER_FRONT_LEFT & ThisEvent.EventParam) {
                        nextState = AvoidSubState_TurnRight;
                        makeTransition = TRUE;
                        ES_Timer_StartTimer(TURN_TIMER);
                            b++;
                    }
                    printf("\r\n %d \r\n", b);
                    break;
                case ES_TIMEOUT:
                    //                    printf("\r\n dsajkkladjskadjls ljkas d \r\n adsjkljkldsjkldas \r\n");
                    Roach_RightMtrSpeed(MOTOR_OFF);
                    Roach_LeftMtrSpeed(MOTOR_OFF);
                    ES_Timer_StopTimer(DANCE_TIMER);
                    ES_Timer_SetTimer(DANCE_TIMER, DANCE_TICK);
                    nextState = AvoidSubState_Dance;
                    makeTransition = TRUE;
                    ES_Timer_SetTimer(TURN_TIMER, 1000); //set how long to dance.
                    ES_Timer_StartTimer(TURN_TIMER);
                    break;
                default:
                    break;
            }
            break;
        case AvoidSubState_Dance:
            Roach_LeftMtrSpeed(-100);
            Roach_RightMtrSpeed(100);
            if (ThisEvent.EventType == ES_TIMEOUT && ThisEvent.EventParam == 1) {
                ES_Timer_StopTimer(TURN_TIMER);
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
                nextState = AvoidSubState_Forward;
                ThisEvent.EventType = ES_NO_EVENT;
                makeTransition = TRUE;
                ES_Timer_StartTimer(DANCE_TIMER);
            }
            break;
        case AvoidSubState_TurnLeft:
            Roach_RightMtrSpeed(MOTOR_OFF);
            Roach_LeftMtrSpeed(NEG_MOTOR_SPEED);

            if (ThisEvent.EventType == ES_TIMEOUT || (ThisEvent.EventType == BUMPER_HIT && ((ThisEvent.EventParam & BUMPER_BACK_LEFT) || (ThisEvent.EventParam & BUMPER_BACK_RIGHT)))) {
                ES_Timer_StopTimer(TURN_TIMER);
                nextState = AvoidSubState_Forward;
                ThisEvent.EventType = ES_NO_EVENT;
                makeTransition = TRUE;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
            }
            break;
        case AvoidSubState_TurnRight:
            Roach_RightMtrSpeed(NEG_MOTOR_SPEED);
            Roach_LeftMtrSpeed(MOTOR_OFF);

            if (ThisEvent.EventType == ES_TIMEOUT || (ThisEvent.EventType == BUMPER_HIT && ((ThisEvent.EventParam & BUMPER_BACK_LEFT) || (ThisEvent.EventParam & BUMPER_BACK_RIGHT)))) {
                ES_Timer_StopTimer(TURN_TIMER);
                nextState = AvoidSubState_Forward;
                ThisEvent.EventType = ES_NO_EVENT;
                makeTransition = TRUE;
                ES_Timer_SetTimer(TURN_TIMER, TURN_TICK);
            }
            break;
        default:
            break;
    }

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunAvoidSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunAvoidSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
