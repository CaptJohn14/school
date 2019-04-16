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
#include "TemplateHSM.h"
#include "SubstateINIFZ.h"
#include "SubstateFINDIFZ.h"
#include "SubstateTRACKTAPE.h"
#include "TapeSensor.h"
#include "Bumpers.h"
#include "Motors.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
typedef enum {
    InitPSubState,
    ROTATING_RIGHT,
    ROTATING_LEFT,
    AIMING,
    LEFT_ADJUST,
    FIRING,
} SubstateINIFZState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"ROTATING_RIGHT",
	"ROTATING_LEFT",
	"AIMING",
	"LEFT_ADJUST",
	"FIRING",
};

#define SHOOTING_TIMER_TICKS 6000

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

static SubstateINIFZState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;

//#define Moving_Speed 60
//#define Soft_Turn 60
//#define Hard_Turn 60
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
uint8_t InitSubstateINIFZ(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunSubstateINIFZ(INIT_EVENT);
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
ES_Event RunSubstateINIFZ(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    SubstateINIFZState_t nextState; // <- change type to correct enum
    static char rotatingflag = 0;
    static flag = 0;
    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = ROTATING_RIGHT;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case ROTATING_RIGHT:
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    ES_Timer_InitTimer(INIFZ_TIMER, 1500);
                    nextState = ROTATING_LEFT;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ENEMY_BEACON_DETECTED:
                    Servo_Shoot();
                    ES_Timer_InitTimer(INIFZ_TIMER, SHOOTING_TIMER_TICKS);
                    nextState = AIMING;
                    flag = 0;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_ENTRY:
                    ES_Timer_InitTimer(INIFZ_TIMER, 2500);
                    Motors_SetRight(ROTATE_TURN + 3);
                    Motors_SetLeft(-ROTATE_TURN - 3);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case ROTATING_LEFT:
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    nextState = ROTATING_RIGHT;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ENEMY_BEACON_DETECTED:
                    ES_Timer_InitTimer(INIFZ_TIMER, 5000);
                    Servo_Shoot();
                    nextState = AIMING;
                    flag = 1;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_ENTRY:
                    Motors_SetRight(-ROTATE_TURN - 3);
                    Motors_SetLeft(ROTATE_TURN + 3);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case AIMING:
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == INIFZ_TIMER) {
                        ES_Timer_InitTimer(INIFZ_TIMER, 350);
                        rotatingflag = 0;
                        nextState = LEFT_ADJUST;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;
                    }
                case ES_ENTRY:
                    Motors_SetBoth(0);
                    Motors_SetShooter(55);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case LEFT_ADJUST:
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == INIFZ_TIMER) {
                        ES_Timer_InitTimer(INIFZ_TIMER, 1000);
                        rotatingflag = 0;
                        nextState = FIRING;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;
                    }
                case ES_ENTRY:
                    Motors_SetRight(ROTATE_TURN);
                    Motors_SetLeft(-ROTATE_TURN);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case FIRING:
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == INIFZ_TIMER) {
                        nextState = FIRING;
                        makeTransition = TRUE;
                        ThisEvent.EventParam = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetShooter(55);
                    if (rotatingflag == 0) {
                        Motors_SetRight(-ROTATE_TURN);
                        Motors_SetLeft(ROTATE_TURN + 2);
                    } else {
                        Motors_SetRight(ROTATE_TURN);
                        Motors_SetLeft(-ROTATE_TURN - 4);
                    }
                    break;
                case ES_EXIT:
                    if (rotatingflag == 0) {
                        ES_Timer_InitTimer(INIFZ_TIMER, 750);
                        rotatingflag = 1;
                    } else {
                        ES_Timer_InitTimer(INIFZ_TIMER, 950);
                        rotatingflag = 0;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetShooter(100);
                    break;
            }
            break;
        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubstateINIFZ(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubstateINIFZ(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

