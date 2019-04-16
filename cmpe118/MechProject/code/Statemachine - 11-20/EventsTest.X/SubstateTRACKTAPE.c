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
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
typedef enum {
    InitPSubState,
    CHECK_TAPE_SENSOR,
    LEFT_TURN_FIRST,
    RIGHT_TURN_FIRST,
    LTURN_FRB,
    RTURN_FLB,
    RTURN_FRB,
    LTURN_FLB,
    TRACK_FRB,
    TRACK_FLB,
    CROSSING_R,
    CROSSING_L,
    REACH_IFZ,
    CORNER_R,
    CORNER_L,
    NINETYTURN_R,
    NINETYTURN_L,

} SubstateTRACKTAPEState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"CHECK_TAPE_SENSOR",
	"LEFT_TURN_FIRST",
	"RIGHT_TURN_FIRST",
	"LTURN_FRB",
	"RTURN_FLB",
	"RTURN_FRB",
	"LTURN_FLB",
	"TRACK_FRB",
	"TRACK_FLB",
	"CROSSING_R",
	"CROSSING_L",
	"REACH_IFZ",
	"CORNER_R",
	"CORNER_L",
	"NINETYTURN_R",
	"NINETYTURN_L",
};

#define Movingspeed 300


#define FRONT_TAPE_MASK (1<<TAPE_FrontCenter)
#define LEFT_TAPE_MASK (1<<TAPE_FrontLeft)
#define RIGHT_TAPE_MASK (1<<TAPE_FrontRight)
#define BACK_TAPE_MASK (1<<TAPE_RearCenter)
#define FRB_TAPE_MASK (FRONT_TAPE_MASK|RIGHT_TAPE_MASK|BACK_TAPE_MASK)
#define FLB_TAPE_MASK (FRONT_TAPE_MASK|LEFT_TAPE_MASK|BACK_TAPE_MASK)
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

static SubstateTRACKTAPEState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;


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
uint8_t InitSubstateTRACKTAPE(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunSubstateTRACKTAPE(INIT_EVENT);
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
ES_Event RunSubstateTRACKTAPE(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    SubstateTRACKTAPEState_t nextState; // <- change type to correct enum
    static char turnflagFRB = 0;
    static char turnflagFLB = 0;
    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = CHECK_TAPE_SENSOR;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case CHECK_TAPE_SENSOR: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case TAPE_OffToOn:
                case ES_ENTRY:
                    if (ThisEvent.EventParam & (1 << TAPE_FrontCenter)) {
                        if (ThisEvent.EventParam & (1 << TAPE_FrontLeft)) {
                            ES_Timer_InitTimer(TRACKFLB_TIMER, 500);
                            nextState = RIGHT_TURN_FIRST;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                            break;
                        } else if (ThisEvent.EventParam & (1 << TAPE_FrontRight)) {
                            ES_Timer_InitTimer(TRACKFRB_TIMER, 500);
                            nextState = LEFT_TURN_FIRST;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                            break;
                        }
                    } else {
                        Motors_SetBoth(Movingspeed);
                        //                        Motors_SetLeft(100);
                        break;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(100);
                    //                    Motors_SetLeft(100);
                    break;
            }
            break;
        case LEFT_TURN_FIRST: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFRB_TIMER) {
                        nextState = TRACK_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(50); //soft left turn
                    Motors_SetLeft(-50);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(50); //soft left turn
                    //                    Motors_SetLeft(-50);
                    break;
            }
            break;
        case TRACK_FRB: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case TAPE_OnToOff:
                    if (!(ThisEvent.EventParam & FRONT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFRB_TIMER, 500);
                        nextState = RTURN_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (!(ThisEvent.EventParam & RIGHT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFRB_TIMER, 500);
                        nextState = LTURN_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam == 0xF) {
                        nextState = CROSSING_R;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case TAPE_OffToOn:
                    if (!(ThisEvent.EventParam & FRONT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFRB_TIMER, 500);
                        nextState = RTURN_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (!(ThisEvent.EventParam & RIGHT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFRB_TIMER, 500);
                        nextState = LTURN_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam == 0xF) {
                        nextState = CROSSING_R;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetBoth(100);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(100);
                    //                    Motors_SetLeft(100);
                    break;
            }
            break;
        case LTURN_FRB: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFRB_TIMER) {
                        nextState = TRACK_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(50); //soft left turn
                    Motors_SetLeft(-50);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(50); //soft left turn
                    //                    Motors_SetLeft(-50);
                    break;
            }
            break;
        case RTURN_FRB: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFRB_TIMER) {
                        nextState = TRACK_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(-50); //soft right turn
                    Motors_SetLeft(50);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(-50); //soft right turn
                    //                    Motors_SetLeft(50);
                    break;
            }
            break;
        case CROSSING_R: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    Motors_SetBoth(Movingspeed);
                    break;
                case TAPE_OnToOff:
                    if ((ThisEvent.EventParam & FRB_TAPE_MASK) == FRB_TAPE_MASK) {
                        if (turnflagFRB == 1) {
                            nextState = REACH_IFZ;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        } else if (turnflagFRB == 0) {
                            nextState = CORNER_R;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                    } else if (ThisEvent.EventParam == 0x0) {
                        nextState = REACH_IFZ;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case TAPE_OffToOn:
                    if ((ThisEvent.EventParam & FRB_TAPE_MASK) == FRB_TAPE_MASK) {
                        if (turnflagFRB == 1) {
                            nextState = REACH_IFZ;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        } else if (turnflagFRB == 0) {
                            nextState = CORNER_R;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                    } else if (ThisEvent.EventParam == 0x0) {
                        nextState = REACH_IFZ;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(100);
                    //                    Motors_SetLeft(100);
                    break;
            }
            break;
        case CORNER_R: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    Motors_SetBoth(0);
                    nextState = NINETYTURN_R;
                    turnflagFRB = 1;
                    makeTransition = TRUE;
                    ThisEvent.EventParam = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    ES_Timer_InitTimer(TRACKFRB_TIMER, 500);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    nextState = NINETYTURN_R;
                    //                    ES_Timer_InitTimer(TRACKFRB_TIMER, 500);
                    //                    Motors_SetRight(0);
                    //                    Motors_SetLeft(0);
                    //                    turnflagFRB = 1;
                    //                    makeTransition = TRUE;
                    //                    ThisEvent.EventParam = ES_NO_EVENT;
                    break;
            }
            break;
        case NINETYTURN_R: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFRB_TIMER) {
                        nextState = TRACK_FRB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(100);
                    Motors_SetLeft(-100); // 90degree turn left
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(100);
                    //                    Motors_SetLeft(-100); // 90degree turn left
                    break;
            }
            break;
        case REACH_IFZ: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    //                    nextState = IN_IFZ;
                    //                    makeTransition = TRUE;
                    //nextState = REACH_IFZ;
                    ThisEvent.EventType = IN_IFZ_EVENT;
                    ThisEvent.EventParam = (uint16_t) 0;
                    PostTemplateHSM(ThisEvent);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    nextState = REACH_IFZ;
                    //                    makeTransition = TRUE;
                    //                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
            }
            break;
        case RIGHT_TURN_FIRST: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFLB_TIMER) {
                        nextState = TRACK_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(-50); //soft right turn
                    Motors_SetLeft(50);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(-50); //soft right turn
                    //                    Motors_SetLeft(50);
                    break;
            }
            break;
        case TRACK_FLB: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case TAPE_OnToOff:
                    if (!(ThisEvent.EventParam & LEFT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFLB_TIMER, 500);
                        nextState = RTURN_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (!(ThisEvent.EventParam & FRONT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFLB_TIMER, 500);
                        nextState = LTURN_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam == 0xF) {
                        nextState = CROSSING_L;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case TAPE_OffToOn:
                    if (!(ThisEvent.EventParam & LEFT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFLB_TIMER, 500);
                        nextState = RTURN_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (!(ThisEvent.EventParam & FRONT_TAPE_MASK)) {
                        ES_Timer_InitTimer(TRACKFLB_TIMER, 500);
                        nextState = LTURN_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam == 0xF) {
                        nextState = CROSSING_L;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetBoth(Movingspeed);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(100);
                    //                    Motors_SetLeft(100);
                    break;
            }
            break;
        case RTURN_FLB: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFLB_TIMER) {
                        nextState = TRACK_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(-50); //soft right turn
                    Motors_SetLeft(50);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(-50); //soft right turn
                    //                    Motors_SetLeft(50);
                    break;
            }
            break;
        case LTURN_FLB: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFLB_TIMER) {
                        nextState = TRACK_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(50); //soft left turn
                    Motors_SetLeft(-50);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(50); //soft left turn
                    //                    Motors_SetLeft(-50);
                    break;
            }
            break;
        case CROSSING_L: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case TAPE_OffToOn:
                    if ((ThisEvent.EventParam & FLB_TAPE_MASK) == FLB_TAPE_MASK) {
                        if (turnflagFLB == 1) {
                            nextState = REACH_IFZ;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        } else if (turnflagFLB == 0) {
                            nextState = CORNER_L;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                    } else if (ThisEvent.EventParam == 0x0) {
                        nextState = REACH_IFZ;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case TAPE_OnToOff:
                    if ((ThisEvent.EventParam & FLB_TAPE_MASK) == FLB_TAPE_MASK) {
                        if (turnflagFLB == 1) {
                            nextState = REACH_IFZ;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        } else if (turnflagFLB == 0) {
                            nextState = CORNER_L;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                    } else if (ThisEvent.EventParam == 0x0) {
                        nextState = REACH_IFZ;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetBoth(Movingspeed);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(100);
                    //                    Motors_SetLeft(100);
                    break;
            }
            break;
        case CORNER_L: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    nextState = NINETYTURN_L;
                    Motors_SetBoth(0);
                    //                    Motors_SetRight(0);
                    //                    Motors_SetLeft(0);
                    turnflagFLB = 1;
                    makeTransition = TRUE;
                    ThisEvent.EventParam = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    ES_Timer_InitTimer(TRACKFLB_TIMER, 500);
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    nextState = NINETYTURN_L;
                    //                    ES_Timer_InitTimer(TRACKFLB_TIMER, 500);
                    //                    Motors_SetRight(0);
                    //                    Motors_SetLeft(0);
                    //                    turnflagFLB = 1;
                    //                    makeTransition = TRUE;
                    //                    ThisEvent.EventParam = ES_NO_EVENT;
                    break;
            }
            break;
        case NINETYTURN_L: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == TRACKFLB_TIMER) {
                        nextState = TRACK_FLB;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_ENTRY:
                    Motors_SetRight(-100);
                    Motors_SetLeft(100); // 90degree turn right
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    //                    Motors_SetRight(-100);
                    //                    Motors_SetLeft(100); // 90degree turn right
                    break;
            }
            break;
        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubstateTRACKTAPE(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunSubstateTRACKTAPE(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

