//Name: Johnson

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Game.h"
#include "Player.h"
#include "UnixBoard.h"

#define ResetPlayer 11111111

typedef enum {
    //NONE, //0 direction. stay still?
    NORTH,
    EAST,
    SOUTH,
    WEST
    //OHHHH, I get what its saying now. SOOOOO exit[NORTH] = 0, means there is no exit NORTH.
    //the NORTH represents the index of the address... basic. so at address NORTH, is 0 means no exit
} direction;

typedef struct {
    char title[GAME_MAX_ROOM_TITLE_LENGTH]; //holds title of room
    char desc[GAME_MAX_ROOM_DESC_LENGTH]; //holds description of room
    int exit[4]; // holds all 4 possible exits for the current room. // 
    char itemreqs[4]; //items held in room. max is 4.
    char loot[4]; // items in the room
    int currentRoom;
    //int move[4]; //represents the direction to go.
    //char file[];
} structgame;

static structgame room;
static FILE *file;

static int GameLoadRoom(int roomNumber);
static void GameOpenNextRoomFile(int roomNumber);
static int Decrypt(int x);
static char roomDir[32]; //will hold the directory for the file wanted.

//******************************************************************************

//// The initial room that Game should initialize to.
//#define STARTING_ROOM 32
//
////the base key for decrypting files
//
//#define DECRYPTION_BASE_KEY 122
//
//// These variable describe the maximum string length of the room title and description respectively.
//// Note that they don't account for the trailing '\0' character implicit with C-style strings.
//#define GAME_MAX_ROOM_TITLE_LENGTH 21
//#define GAME_MAX_ROOM_DESC_LENGTH 255
//
///**
// * This enum defines flags for checking the return values of GetCurrentRoomExits(). Usage is as
// * follows:
// *
// * if (GetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS) {
// *   // The current room has a west exit.
// * }
// *
// * @see GetCurrentRoomExits
// */
//typedef enum {
//    GAME_ROOM_EXIT_WEST_EXISTS  = 0b0001,
//    GAME_ROOM_EXIT_SOUTH_EXISTS = 0b0010,
//    GAME_ROOM_EXIT_EAST_EXISTS  = 0b0100,
//    GAME_ROOM_EXIT_NORTH_EXISTS = 0b1000
//} GameRoomExitFlags;

//******************************************************************************

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void) {
    GameLoadRoom(STARTING_ROOM); //set current room to starting room and load it.
    return STANDARD_ERROR;
}

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void) {
    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS) { //0 will mean no match.
        GameLoadRoom(room.exit[NORTH]); //load new room.
        return SUCCESS; //return success if current room has exit and new room able to load.
    }
    return STANDARD_ERROR;
    //GameGo*() functions all open the next room file, process the data, and load the room struct
    //with the current room.)
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void) {
    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS) {
        GameLoadRoom(room.exit[EAST]);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void) {
    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS) {
        GameLoadRoom(room.exit[SOUTH]);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void) {
    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS) {
        GameLoadRoom(room.exit[NORTH]);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

//FILE *fopen(const char *filename, const char *mode)
//fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
//don't use switch statement. use sprintf() to get filename.)

//GameGo*() functions all open the next room file, process the data, and load the room struct
//with the current room.)
//files only used within GameGo*(). Files open one at a time only
//all fopen() failed ---->>>> FATAL_ERROR()

//I guess I could... combine GameOpenNextRoomFile and GameLoadRoom into one later if I want.
//GameOpenNextRoomFile is pretty short.

void GameOpenNextRoomFile(int roomNumber) {
    memset(roomDir, 0, sizeof (roomDir)); //clearing string containing directory for new one.
    sprintf(roomDir, "RoomFiles/room%d.txt", roomNumber); //put directory into string to be loaded.
    file = fopen(roomDir, "rb"); //open file with roomFile directory. 
    //apparently, use "rb" for binary files.
    if (file == NULL) { //checks if grabbing file is a success.
        printf("ERROR: Grabbing file failed.\n"); //error message for myself.
        FATAL_ERROR(); //exits
    }
}

int GameLoadRoom(int roomNumber) {
//    uint8_t temp = 0; //will be used to store fgetc data
    uint8_t itemsNeeded = 0; //holds value for how many items needed 
    uint8_t lengthTitle = 0;
    uint8_t lengthDesc = 0;
    uint8_t itemsHeld = 0;
    int i;
    int skipVersion = FALSE; //flag for whether or not to skip over this version.
    //clears ******************************************************************************************
    memset(room.title, 0, sizeof (room.title)); //clear title for new room's title
    memset(room.desc, 0, sizeof (room.desc)); //clear title for new room's description
    memset(room.exit, 0, sizeof (room.exit)); //clear current room's possible exits.
    //*************************************************************************************************
    room.currentRoom = roomNumber;
    GameOpenNextRoomFile(roomNumber);
    
    //decrypt title
    lengthTitle = Decrypt(fgetc(file));
    for (i = 0; i < lengthTitle; i++) { 
        room.title[i] = Decrypt(fgetc(file)); 
    }
    //while loop checks if correct version and grabs the data. if not, repeat.
    while (1) {
        itemsNeeded = Decrypt(fgetc(file)); //grabs amount of items required.
        //check if end of file. should never really happen though.
        if(itemsNeeded == NULL){
            printf("ERROR: reached end of file!\n");
        }
        //checks if item requirements matches players inventory.
        if(itemsNeeded != 0){
            for (i = 0; i < itemsNeeded; i++) {
                room.itemreqs[i] = Decrypt(fgetc(file));
            }
            for (i = 0; i < itemsNeeded; i++) {
                if (FindInInventory(room.itemreqs[i]) == STANDARD_ERROR);
                //items don't match.
                skipVersion = TRUE;
            }
        }else {
            //this is if itemsNeeded == 0, so correct version and don't skip
            skipVersion = FALSE;
        }
        //if match, then don't skip
        if (skipVersion == FALSE) {
            //Get description.
            lengthDesc = Decrypt(fgetc(file)); 
            for (i = 0; i < lengthDesc; i++) { 
                room.desc[i] = Decrypt(fgetc(file));
            }
            //checks the items contained in room
            itemsHeld = Decrypt(fgetc(file)); 
            for (i = 0; i < itemsHeld; i++) { 
                room.loot[i] = Decrypt(fgetc(file));
            }
            //adds items contained in room into player's inventory.
            for (i = 0; i < itemsHeld; i++) {
                AddToInventory(room.loot[i]);
            }
            //grabs the exits for the room
            for (i = 0; i < 4; i++) {
                room.exit[i] = Decrypt(fgetc(file)); //grabs exits.
            }
            fclose(file);
            return SUCCESS;
            break;
        } else if (skipVersion == TRUE) {
            //need to skip over description, items in room, and exits
            //skip description
            lengthDesc = Decrypt(fgetc(file)); //grabs length of description
            fseek(file, lengthDesc, SEEK_CUR); //skips over that length
            //skip items contained
            itemsHeld = Decrypt(fgetc(file)); //grabs amnt of items held
            fseek(file, itemsHeld, SEEK_CUR); //skips over items
            //skip exits
            fseek(file, 4, SEEK_CUR); //skips over the exits.
            //go back to the while.
        }
    }
    return STANDARD_ERROR;
}

int Decrypt(int x) {
    return (DECRYPTION_BASE_KEY + room.currentRoom)^x;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title) {
    //strcpy(title, game.title);  //coppies current room title.
    strncpy(title, room.title, GAME_MAX_ROOM_TITLE_LENGTH + 1); //copy room title with that length
    if (title == NULL) { //if null, then return 0.

        return 0;
    }
    return strlen(title); //return the length of the string stored in title. will be +1 for the null.
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc) {
    strncpy(desc, room.desc, GAME_MAX_ROOM_DESC_LENGTH + 1); //copy desc with that length
    if (desc == NULL) { //if null, return 0.

        return 0;
    }
    return strlen(desc); //will be this value +1 to account for null terminating char.
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void) {
    uint8_t exits = 0x0;
    if (room.exit[NORTH] != 0) { //if not 0, then room has exit at north.
        exits |= GAME_ROOM_EXIT_NORTH_EXISTS; //toggles bit for NORTH EXIT. which is 0b1000
    }
    if (room.exit[EAST]) {
        exits |= GAME_ROOM_EXIT_EAST_EXISTS; //0b0100
    }
    if (room.exit[SOUTH]) { //0b0010
        exits |= GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    if (room.exit[WEST]) { //0b0001
        exits |= GAME_ROOM_EXIT_WEST_EXISTS;
    }
    return exits;
}
