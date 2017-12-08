/* 
 * File:   Leds.h
 * Author: Johnson Le
 *
 * Created on February 16, 2017, 11:22 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#include "BOARD.h"
#include <xc.h>


#define LEDS_INIT() do { \
    TRISE = 0x00; \
    LATE = 0x00; \
} while(0)

#define LEDS_GET() LATE
    
#define LEDS_SET(x) (LATE = (x))
#endif	/* LEDS_H */

