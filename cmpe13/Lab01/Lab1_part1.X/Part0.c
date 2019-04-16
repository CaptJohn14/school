/*
 * File:   newmainXC16.c
 * Author: Johnson Le
 *
 * Created on January 13, 2017, 3:46 PM
 */


#include "xc.h"
#include "BOARD.h"

void main(void) 
{
    BOARD_Init();
    printf("Hello World\n");
    while(1);
}
