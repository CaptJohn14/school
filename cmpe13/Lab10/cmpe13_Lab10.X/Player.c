//Name: Johnson Le

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Game.h"
#include "Player.h"
#include "UnixBoard.h"

#define ResetPlayer 11111111

uint8_t inventory[INVENTORY_SIZE]; //can hold 4 items. valid items values of 0-255.


// Define how big the player's inventory is.
//#define INVENTORY_SIZE 4

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item) {
//    if(item == ResetPlayer){
//        memset(inventory, 0, sizeof (inventory));
//        return SUCCESS;
//        //clear player's inventory to start. I'm hoping 11111111 isn't being used.
//    }
    //if inventory is null, inventory is empty
    if (inventory[0] == NULL) { //checks if first slot if empty
        inventory[0] = item; //if it is empty, place item into inventory
        return SUCCESS; //so if not empty, moves onto next slot.
    } else if (inventory[1] == NULL) {
        inventory[1] = item;
        return SUCCESS;
    } else if (inventory[2] == NULL) {
        inventory[2] = item;
        return SUCCESS;
    } else if (inventory[3] == NULL) {
        inventory[3] = item;
        return SUCCESS;
    }
    return STANDARD_ERROR; //if all inventory spots are full, then fail.
}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item) {
    if (inventory[0] == item || inventory[1] == item || inventory[2] == item || inventory[3]) {
        return SUCCESS; //so if the item searching for matches with one in player's inventory, success.
    }
    return STANDARD_ERROR;
}
