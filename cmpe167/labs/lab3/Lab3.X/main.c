/*==============================================================================
 * Name: Johnson Le
 * Date 2/21/2019
 * Desc: Main for lab 3.
 ==============================================================================
 * Libraries
 =============================================================================*/
#include "BOARD.h"
#include "AD.h"
#include <stdio.h>
#include "Oled.h"
#include <xc.h>

/*==============================================================================
 * Defines/Functions/Globals
 =============================================================================*/
#define MAIN
#define part4
//#define testing
#define part6

#define sample_size 50
char string[40];
void PrintOled(char* s);
void UpdateAccel();
void UpdateMag();
void UpdateGyro();
void Part4();
void Part6();
void wait(unsigned ms);
float ms_hz(int h);

/*==============================================================================
 * Main
 =============================================================================*/
#ifdef MAIN

int main() {
    /*==========================================================================
     * Initialization
     =========================================================================*/
    BOARD_Init();
    OledInit();
    TIMERS_Init();

    printf("\r\nWelcome to Johnson's Lab3\r\n");
    if (!MPU9250_Init() || !AD_Init()) {
        printf("Initialization failed.\r\n");
        while (1);
    }
    printf("Initializtions Success\r\n");
    /*==========================================================================
     * Code
     =========================================================================*/
        Part6();
    while (1) {
        //        sprintf(string, "Acc\nX: %d\nY: %d\nZ: %d", MPU9250_ReadAccelX(), MPU9250_ReadAccelY(), MPU9250_ReadAccelZ());
        //        sprintf(string, "Mag\nX: %d\nY: %d\nZ: %d", MPU9250_ReadMagX(), MPU9250_ReadMagY(), MPU9250_ReadMagZ());
        //        sprintf(string, "Gyr\nX: %d\nY: %d\nZ: %d", MPU9250_ReadGyroX(), MPU9250_ReadGyroY(), MPU9250_ReadGyroZ());
        //        PrintOled(string);

        //        wait(100);
    }
}
#endif

#ifdef part6

void Part6() {
    int i = 0;
    unsigned int t_start;
    unsigned int p = 200; //take data every p/1000 seconds, 1000->1sec hz=1/pE-3
    //wait 2 seconds to settle
    PrintOled("Wait 2 seconds");
    wait(2000);
    static unsigned int timeout = 180; //1hr = 3600seconds
    t_start = TIMERS_GetMilliSeconds();
    for (i = 0; i < (timeout)*(1000 / p); i++) {
        sprintf(string, "Grabbing data %u(s)\ntimer: %d(m) %d(s)", timeout, (i / 60) * p / 1000, (i * p / 1000) % 60);
        PrintOled(string);
        printf("%d,%d,%d,%d,%d,%d\r\n",
                MPU9250_ReadAccelX(), MPU9250_ReadAccelY(), MPU9250_ReadAccelZ(),
                MPU9250_ReadMagX(), MPU9250_ReadMagY(), MPU9250_ReadMagZ());
        wait(p);
    }
}
#endif

#ifdef part4

void Part4() {
    int i = 0;
    unsigned int t_start;
    unsigned int p = 50; //take data every p/1000 seconds, 1000->1sec
    //wait 2 seconds to settle
    PrintOled("Wait 2 seconds");
    wait(2000);
    static unsigned int timeout = 120; //1hr = 3600seconds
    t_start = TIMERS_GetMilliSeconds();
    for (i = 0; i < (timeout)*(1000 / p); i++) {

        sprintf(string, "Grabbing data %u(s)\ntimer: %d(m) %d(s)", timeout, (i / 60) * p / 1000, (i * p / 1000) % 60);
        PrintOled(string);
        //        printf("%d,%d,%d\r\n", MPU9250_ReadGyroX(), MPU9250_ReadGyroY(), MPU9250_ReadGyroZ());
        //        printf("x%d\r\ny%d\r\nz%d\r\n", MPU9250_ReadGyroX(), MPU9250_ReadGyroY(), MPU9250_ReadGyroZ());
        //        printf("%f\r\n", (MPU9250_ReadGyroX()+496.25)/131);
        //        printf("%f\r\n", (MPU9250_ReadGyroY()-244.8)/131);
        //        printf("%f\r\n", (MPU9250_ReadGyroZ()-71.55)/131);
        wait(p);
    }
}
#endif

#ifdef testing

void UpdateAccel() {
    static int i = 0;
    static int x = 0;
    static int y = 0;
    static int z = 0;
    static int X[sample_size] = {0};
    static int Y[sample_size] = {0};
    static int Z[sample_size] = {0};

    x -= X[i];
    y -= Y[i];
    z -= Z[i];
    X[i] = MPU9250_ReadAccelX();
    Y[i] = MPU9250_ReadAccelY();
    Z[i] = MPU9250_ReadAccelZ();
    x += X[i];
    y += Y[i];
    z += Z[i];

    if (++i == sample_size) {

        i = 0;
    }

    sprintf(string, "Accelerometer\nX: %d\nY: %d\nZ: %d", (x / sample_size), (y / sample_size), (z / sample_size));
    PrintOled(string);
}

void UpdateMag() {
    static int i = 0;
    static int x = 0;
    static int y = 0;
    static int z = 0;
    static int X[sample_size] = {0};
    static int Y[sample_size] = {0};
    static int Z[sample_size] = {0};

    x -= X[i];
    y -= Y[i];
    z -= Z[i];
    X[i] = MPU9250_ReadMagX();
    Y[i] = MPU9250_ReadMagY();
    Z[i] = MPU9250_ReadMagZ();
    x += X[i];
    y += Y[i];
    z += Z[i];

    if (++i == sample_size) {

        i = 0;
    }

    sprintf(string, "Magnetometer\nX: %d\nY: %d\nZ: %d", (x / sample_size), (y / sample_size), (z / sample_size));
    PrintOled(string);
}

void UpdateGyro() {
    static int scale = 1;
    static int i = 0;
    static int x = 0;
    static int y = 0;
    static int z = 0;
    static int X[sample_size] = {0};
    static int Y[sample_size] = {0};
    static int Z[sample_size] = {0};

    x -= X[i];
    y -= Y[i];
    z -= Z[i];
    X[i] = MPU9250_ReadGyroX() / scale;
    Y[i] = MPU9250_ReadGyroY() / scale;
    Z[i] = MPU9250_ReadGyroZ() / scale;
    x += X[i];
    y += Y[i];
    z += Z[i];

    if (++i == sample_size) {

        i = 0;
    }

    sprintf(string, "Gyroscope\nX: %d\nY: %d\nZ: %d", (x / sample_size), (y / sample_size), (z / sample_size));
    PrintOled(string);
}
#endif

void PrintOled(char* s) {

    sprintf(string, s);
    OledClear(0);
    OledDrawString(string);
    OledUpdate();

}

void wait(unsigned ms) {
    unsigned int t_start = 0;
    t_start = TIMERS_GetMilliSeconds();

    while (TIMERS_GetMilliSeconds() - t_start < ms);
}

float ms_hz(int h) {
    return (1000 / h);
}