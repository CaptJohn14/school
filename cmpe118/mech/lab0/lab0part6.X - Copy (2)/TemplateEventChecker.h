/* 
 * File:   CheckEvents.h
 * Author: jonle
 *
 * Created on October 2, 2018, 6:25 PM
 */

#ifndef CHECKEVENTS_H
#define	CHECKEVENTS_H

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT
#include "BOARD.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
uint8_t CheckLightDetection(void);

/*  Function: bumperHit(void)
    return: TRUE or FALSE
    description:    This function detects if a bumper was pressed or released.
*/
#endif	/* CHECKEVENTS_H */

