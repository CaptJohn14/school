/* 
 * File:   BumperService.h
 * Author: jonle
 *
 * Created on October 4, 2018, 1:19 AM
 */

#ifndef BUMPERSERVICE_H
#define	BUMPERSERVICE_H

#include "ES_Configure.h"

uint8_t InitBumperService(uint8_t Priority);
uint8_t PostBumperService(ES_Event ThisEvent);

ES_Event RunBumperService(ES_Event ThisEvent);

uint8_t Debounce(unsigned char oldValue);

#endif	/* BUMPERSERVICE_H */

