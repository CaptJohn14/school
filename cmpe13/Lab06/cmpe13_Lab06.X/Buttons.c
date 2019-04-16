/* NAME: Johnson Le*/

#include "Buttons.h"

void ButtonsInit(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

uint8_t ButtonsCheckEvents(void) {

    static int btn1;    //initialize to false? guess not needed (:
    static int btn2;
    static int btn3;
    static int btn4;

    //was using else if but what if I pressed multiple buttons? haha
    // need to check individual buttons and return press or release

    //button 1 press
    if ((BUTTON_STATES() & BUTTON_STATE_1) && !btn1) {
        //printf("btn1 press %d\n", BUTTON_STATES()); 
        btn1 = TRUE; 
        return BUTTON_EVENT_1DOWN;
    }
    //button 1 release
    if (!(BUTTON_STATES() & BUTTON_STATE_1) && btn1) {
        //printf("btn1 release %d\n", BUTTON_STATES());
        btn1 = FALSE;
        return BUTTON_EVENT_1UP;
    }

    //button 2 press
    if ((BUTTON_STATES() & BUTTON_STATE_2) && !btn2) {
        btn2 = TRUE;
        return BUTTON_EVENT_2DOWN;
    }
    //button 2 release
    if (!(BUTTON_STATES() & BUTTON_STATE_2) && btn2) {
        btn2 = FALSE;
        return BUTTON_EVENT_2UP;
    }
    
    //button 3 press
    if ((BUTTON_STATES() & BUTTON_STATE_3) && !btn3) {
        btn3 = TRUE;
        return BUTTON_EVENT_3DOWN;
    }
    //button 3 release
    if (!(BUTTON_STATES() & BUTTON_STATE_3) && btn3) {
        btn3 = FALSE;
        return BUTTON_EVENT_3UP;
    }

    //button 4 press
    if ((BUTTON_STATES() & BUTTON_STATE_4) && !btn4) {
        btn4 = TRUE;
        return BUTTON_EVENT_4DOWN;
    }
    //button 4 release
    if (!(BUTTON_STATES() & BUTTON_STATE_4) && btn4) {
        btn4 = FALSE;
        return BUTTON_EVENT_4UP;
    }

    return BUTTON_EVENT_NONE;
}