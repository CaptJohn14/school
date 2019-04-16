//Name: Johnson Le

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <xc.h>
//#include <plib.h>
//#include <stdbool.h>

//CMPE13 Support Library
#include "UnixBOARD.h"
#include "Game.h"
//#include "Player.h"

static void printTitleDesc(void);
static void printExits(void);
static char title[128];
static char desc[128];
static int exitSuccess;
static char directions[128];

// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****

// **** Declare any function prototypes here ****

int main() {

    /******************************** Your custom code goes below here ********************************/
    char input;
    GameInit();
    memset(title, 0, sizeof(title));
    memset(desc, 0, sizeof(desc));
    exitSuccess = FALSE;
    
    if (GameInit() == STANDARD_ERROR) {
        printf("ERROR: GameInit() failed!!!\n");
        FATAL_ERROR();
    }
    
    printTitleDesc();
    while (1) {
        printf("Select direction.");
        printExits();
        input = getchar();
        switch (input) {
            case 'n':
            case 'N':
                exitSuccess = GameGoNorth();
                break;
            case 'e':
            case 'E':
                exitSuccess = GameGoEast();
                break;
            case 's':
            case 'S':
                exitSuccess = GameGoSouth();
                break;
            case 'w':
            case 'W':
                exitSuccess = GameGoWest();
                break;
            case 'q':
            case 'Q':
                printf("Goodbye\n");
                exit(1);
            default:
                printf("You have inputed an invalid character. Please try again.\n");
                break;
        }
        if (exitSuccess == SUCCESS){
            printTitleDesc();
        }else if(exitSuccess == STANDARD_ERROR){
            printf("That direction does not have an exit. Please try again.\n");
        }
    }

    /**************************************************************************************************/
    while (1);
}

void printTitleDesc(){
    memset(title, 0, sizeof(title));
    memset(desc, 0, sizeof(desc));
    GameGetCurrentRoomTitle(title);
    GameGetCurrentRoomDescription(desc);
    printf("Room Title: %s\n", title);
    printf("Room Description: %s\n", desc);
}

void printExits(){
    uint8_t exits = 0x0;
    exits = GameGetCurrentRoomExits();
    memset(directions, 0 , sizeof(directions));
    if (exits & GAME_ROOM_EXIT_NORTH_EXISTS){   //if 0, no exit there.
        sprintf(directions + strlen(directions), "(N)orth, ");
    }
    if (exits & GAME_ROOM_EXIT_EAST_EXISTS){
        sprintf(directions + strlen(directions), "(E)ast, ");
    }
    if (exits & GAME_ROOM_EXIT_SOUTH_EXISTS){
        sprintf(directions + strlen(directions), "(S)outh, ");
    }
    if (exits & GAME_ROOM_EXIT_WEST_EXISTS){
        sprintf(directions + strlen(directions), "(W)est, ");
    }
    printf ("Exits exists at: %s\n", directions);
}
