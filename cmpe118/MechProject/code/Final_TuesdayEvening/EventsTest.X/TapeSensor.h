/*==============================================================================
 * File Name: TapeSensor.h
 * Coders: Team 14 - Johnson, Jacqueline, M
 * Description: Will provide simpler ways to read TapeSensor
 =============================================================================*/

#ifndef _TapeSensor_H_    
#define _TapeSensor_H_

/*==============================================================================
 * Libraries
 =============================================================================*/

/*==============================================================================
 * Macros and Declarations
 =============================================================================*/
#define N_TapeSensors 4 //set number of tape sensors
#define TAPE_IntoDark 320
#define TAPE_IntoLight 180
#define AD_PORTV_Pin(j) (1<<(0+j))
#define AD_PORTW_Pin(j) (1<<(6+j)) 

typedef enum {
    LIGHT, DARK,
} TAPE_EVENT;

static const char *TAPE_EventNames[] = {
    "LIGHT",
    "DARK",
};

typedef enum {
    TAPE_FrontCenter,
    TAPE_FrontLeft,
    TAPE_FrontRight,
    TAPE_RearCenter,
} TAPE_LOCATION;
//note that LocationNames is not mapped with the enum
static const char *TAPE_LocationNames[] = {
    "TAPE_FrontCenter",
    "TAPE_FrontLeft",
    "TAPE_FrontRight",
    "TAPE_RearCenter",
};

/*==============================================================================
 * Initialize TAPE. Tried adding pins here but didn't work. so its now empty lol
 =============================================================================*/
char TAPE_Init(void);

/*==============================================================================
 * Function: TAPE_Read(void);
 * Returns: 8 bit number.
 * Use: Function will return an 8-bit. Value is dependant on which TAPE SENSOR
 *      is in the dark. If the first is in dark and the rest is in light than it
 *      will return 0b00000001
 =============================================================================*/
unsigned char TAPE_Read(void);

/*==============================================================================
 * Function: TAPE_PrintAnalogAll
 * Use: prints out (: thats it.
 =============================================================================*/
void TAPE_PrintAnalogAll(void);
/*==============================================================================
 * Function: Tape_PrintDigitalAll
 * Use: prints out (: thats it.
 =============================================================================*/
void TAPE_PrintDigitalAll(void);

/*==============================================================================
 * Function: static void TAPE_AnalogRead(void);
 * Use: private function that reads all tape sensors and throws into array
 =============================================================================*/
static void TAPE_AnalogRead(void);

/*==============================================================================
 * Function: printBinary
 * Use: recursion. prints given param in binary
 =============================================================================*/
static void printBinary(char n);

/*==============================================================================
 * Function: void setLED(void)
 * Use: turns on LED based on tape sensor
 =============================================================================*/
static void setLED(void);

#endif 

