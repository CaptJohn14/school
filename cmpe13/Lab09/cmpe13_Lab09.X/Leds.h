/* 
 * File:   Leds.h
 * Author: Joe
 * Collaborators: none
 * Created on February 21, 2017, 4:27 PM
 */

#include "BOARD.h"
#include <xc.h>

#ifndef LEDS_H
#define	LEDS_H

// Set TRISE and LATE to 0:
#define LEDS_INIT() do{TRISE = 0x0; LATE = 0x0;}while(0)

// Return the current value of the LATE register:
#define LEDS_GET() (LATE)

// Set the LATE register to the given input:
#define LEDS_SET(x) (LATE = (x))

#endif	/* LEDS_H */