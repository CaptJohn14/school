/*=============================================================================
# Libraries
==============================================================================*/
#include <stdio.h>
#include <Board.h>
#include "timers.h"
#include "IO_Ports.h"

/*=============================================================================
# Functions
==============================================================================*/
uint8_t Problem6a(void);

/*=============================================================================
# Main code
==============================================================================*/
//trigger is on w3, echo is on w4. trigger is controlled from uno32 so will be output
int main(){
    IO_PortsSetPortInputs(PORTW, PIN4); //set echo pin as input
    IO_PortsSetPortOutputs(PORTW, PIN3);    //set trigger pin as output
    uint8_t Distance = Problem6a();
}
//function for getting distance.
uint8_t Probleme6a(void){
    unsigned int time1 = 0;  //read time when set trigger
    unsigned int time2 = 0;  //read time when receive echo

    //set trigger pin high for a bit
    IO_PortsWritePort(PORTW, PIN3);
    time1 = TIMERS_GetTime();
    DELAY(0.01);
    IO_PortsClearPortBits(PORTW, PIN3);

    //echo should be high. Now to wait for it to go low.
    while((IO_PortsReadPort(PORTW) & PIN4) == PIN4);
    //once echo is low, grab time and calculate distance
    time2 = TIMERS_GetTime
    //D=Ew/148 for distance in inches
    return ((time2-time1)/148);
}
