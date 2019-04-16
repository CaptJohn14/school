/*==============================================================================
 * Name: Johnson Le
 * Date: 1/29/2019
 * Desc: Main for lab 2.
 =============================================================================*/

/*==============================================================================
 * Libraries
 =============================================================================*/
#include "BOARD.h"
#include "AD.h"
#include <stdio.h>
#include "Oled.h"
#include <xc.h>
#include "QEI.h"
#include "pwm.h"
//#include "PING.h"
#include "ToneGeneration.h"
//#include "CAPTOUCH.h"



/*==============================================================================
 * Defines/Functions
 =============================================================================*/
#define PART_encoder
//#define PART_3

int k;
#define DELAY(x) for(k=0; k<x; k++)

#define PIN_R PWM_PORTY12   //pin5
#define PIN_G PWM_PORTY10   //pin6
#define PIN_B PWM_PORTY04   //pin9

#define PIN_CAP AD_A5

char string[40];
int count = 0;
unsigned int dooty = 0;

int angle(int val);
void SetColor(int ang); //val = the angle
int map(int in, int in_min, int in_max, int out_min, int out_max);

/*==============================================================================
 * Main
 =============================================================================*/
int main() {
    /*==========================================================================
     * Initialization
     =========================================================================*/
    BOARD_Init();
    AD_Init();
    OledInit();
    PWM_Init();

#ifdef PART_encoder
    QEI_Init();
    char ErrorCheck;
    ErrorCheck = PWM_AddPins(PIN_B | PIN_G | PIN_R);
    if (!ErrorCheck) {
        printf("ERROR! \r\n");
        while (1);
    }
    SetColor(0);
    while (1);
#endif

#ifdef PART_3
#define dist_min 0
#define dist_max 1000
#define tone_min 100
#define tone_max 800
    PING_Init();
    ToneGeneration_Init();
    ToneGeneration_ToneOn();
    unsigned int dist = 0;
    unsigned int old = 0;
    unsigned int arr[5]={0, 0, 0, 0, 0};
    unsigned int index = 0;
    while (1) {
        arr[index] = PING_GetDistance();
        dist = (arr[0]+arr[1]+arr[2]+arr[3]+arr[4])/5;
//        dist = PING_GetDistance();
        if (dist > old + 15 || dist < old - 15) {
            old = dist;
        }
        ToneGeneration_SetFrequency(map(old, dist_min, dist_max, tone_min, tone_max));
        sprintf(string, "time(us): %u\ndist(): %u", PING_GetTimeofFlight(), PING_GetDistance());
        OledDrawString(string);
        OledUpdate();
        OledClear(0);
        index++;
        if(index==5){
            index = 0;
        }
    }
#endif

#ifdef PART_7
    AD_AddPins(PIN_CAP);
    unsigned int CAP_READ = 0;
    while (1) {
        CAP_READ = AD_ReadADPin(PIN_CAP);
        sprintf(string, "NO: %u", CAP_READ);
        if (CAP_READ > 60) {
            sprintf(string, "YEEEEEE BOIIIIIII\n%u", CAP_READ);
            OledDrawString(string);
            OledUpdate();
            OledClear(0);
            DELAY(500000);
        }
        OledDrawString(string);
        OledUpdate();
        OledClear(0);
    }
#endif

#ifdef PART_8
    CAPTOUCH_Init();
    while (1) {
        CAPTOUCH_IsTouched();
        if (CAPTOUCH_IsTouched()) {
            sprintf(string, "YEEEEE BOIIIIII");
        }
        else{
            sprintf(string, "\n\n\nNAHHHHHHH SONNNNNNNN");
        }
        OledDrawString(string);
        OledUpdate();
        OledClear(0);
    }
#endif

}


#ifdef PART_encoder

void __ISR(_CHANGE_NOTICE_VECTOR, ipl1auto) ChangeNotice_Handler(void) {
    count = QEI_GetPosition();
    SetColor(angle(count));
    INTClearFlag(INT_CN);
}

void SetColor(int ang) {
    static unsigned int r = 0;
    static unsigned int g = 0;
    static unsigned int b = 1000;

    //yellow->orange->red
    if (ang >= 0 && ang < 90) {
        r = 0;
        g = map(ang, 0, 90, 0, 1000);
        b = 1000;
    }//red->purple
    else if (ang >= 90 && ang < 135) {
        r = map(ang, 90, 134, 0, 500);
        b = map(ang, 90, 134, 1000, 0);
    }//purple->blue
    else if (ang >= 135 && ang < 180) {
        r = map(ang, 135, 179, 500, 1000);
    }//blue->lightblue
    else if (ang >= 180 && ang < 225) {
        g = map(ang, 180, 224, 1000, 500);
    }//lightblue->teal
    else if (ang >= 225 && ang < 270) {
        g = map(ang, 225, 269, 500, 0);
    }//teal->lime
    else if (ang >= 270 && ang < 315) {
        b = map(ang, 270, 314, 0, 500);
    }//lime->green
    else if (ang >= 315 && ang <= 360) {
        b = map(ang, 315, 360, 500, 1000);
    }//lime->green
    else if (ang >= -45 && ang < 0) {
        r = map(-ang, 1, 45, 0, 1000);
    } else if (ang >= -135 && ang < -45) {
        b = map(-ang, 44, 135, 1000, 0);
    } else if (ang >= -225 && ang < -135) {
        g = map(-ang, 134, 225, 0, 1000);
    } else if (ang >= -270 && ang < -225) {
        r = map(-ang, 224, 270, 1000, 500);
    } else if (ang >= -315 && ang < -270) {
        r = map(-ang, 269, 315, 500, 0);
        b = map(-ang, 269, 315, 0, 1000);
    } else if (ang >= -360 && ang <= -315) {
        g = map(-ang, 315, 360, 1000, 500);
    }

    PWM_SetDutyCycle(PIN_R, r);
    PWM_SetDutyCycle(PIN_G, g);
    PWM_SetDutyCycle(PIN_B, b);

    sprintf(string, "count: %d\nangle: %d\nrgb: %u, %u, %u", count, angle(count), r, g, b);
    OledDrawString(string);
    OledUpdate();
    OledClear(0);
    //    printf("ang: %d\r\nr: %u\r\ng: %u\r\nb: %u\r\n\r\n", ang, r, g, b);
}

int angle(int val) {

    return map(val, -96, 96, -360, 360);
}


#endif

int map(int in, int in_min, int in_max, int out_min, int out_max) {

    return ((((in - in_min)*(out_max - out_min)) / (in_max - in_min))+(out_min));
}