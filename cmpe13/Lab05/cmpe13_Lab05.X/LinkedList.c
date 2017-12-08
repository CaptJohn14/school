// Heap size 1024 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "LinkedList.h"

/**
 * This function starts a new linked list. Given an allocated pointer to data it will return a
 * pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
 * returns NULL otherwise it should return a pointer to this new list item. data can be NULL.
 *
 * @param data The data to be stored in the first ListItem in this new list. Can be any valid 
 *             pointer value.
 * @return A pointer to the malloc()'d ListItem. May be NULL if an error occured.
 */
ListItem *LinkedListNew(char *data) {
    ListItem *new = malloc(sizeof (ListItem)); //creating new
    new->data = data;
    if (new == '\0') {
        return NULL;
    }
    //too make sure no item b4 or after
    new->nextItem = '\0';
    new->previousItem = '\0';
    return new;
}

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead returns it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * @param item The ListItem to remove from the list.
 * @return The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item) {
    char *data = item->data;
    if (item == '\0') {
        return NULL;
    }
    if (item->nextItem != '\0') {
        item->nextItem->previousItem = item->previousItem;
    } else if (item->previousItem != '\0') {
        item->previousItem->nextItem = item->nextItem;
    }
    free(item); //think free clears that spot?
    return data;
}

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
 * @param list An item in the list to be sized.
 * @return The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list) {
    int size = 0;
    //need the start of list and keep moving down until null
    //    ListItem *head = list;
    //    while (head ->previousItem != '\0'){    //this while loop will basically move back until null 
    //        head = head->previousItem;          //which ends on first item in list (head).
    //    }
    ListItem *temp = LinkedListGetFirst(list);
    while (temp != '\0') { //this while will keep moving down list and increment
        size++; //till it reaches null
        temp = temp->nextItem;
    }
    return size;
}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will just return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * @param list An element in a list.
 * @return The first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list) {
    ListItem *head = list;
    while (head->previousItem != '\0') { //this while loop will basically move back until null 
        head = head->previousItem; //which ends on first item in list (head).
    }
    return head;
}

/**
 * This function allocates a new ListItem containing data and inserts it into the list directly
 * after item. It rearranges the pointers of other elements in the list to make this happen. If
 * passed a NULL item, InsertAfter() should still create a new ListItem, just with no previousItem.
 * It returns NULL if it can't malloc() a new ListItem, otherwise it returns a pointer to the new
 * item. The data parameter is also allowed to be NULL.
 *
 * @param item The ListItem that will be before the newly-created ListItem.
 * @param data The data the new ListItem will point to.
 * @return A pointer to the newly-malloc()'d ListItem.
 */
ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    ListItem *temp = LinkedListNew(data);
    if (temp == '\0') {
        return NULL;
    }
    if (item->nextItem == '\0') {
        temp->nextItem = '\0';
        temp->previousItem = item;
        item->nextItem = temp;
    } else {
        temp->previousItem = item;
        temp->nextItem = item->nextItem;
        item->nextItem->previousItem = temp;
        item->nextItem = temp;
    }
    return temp;
}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. It is used
 * within LinkedListSort() for swapping items, but probably isn't too useful otherwise. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * @param firstItem One of the items whose data will be swapped.
 * @param secondItem Another item whose data will be swapped.
 * @return SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    if (firstItem == '\0' || secondItem == '\0') {
        return STANDARD_ERROR;
    } else {
        char *temp1 = firstItem->data; //setting temp1 = to 1st item
        char *temp2 = secondItem->data; //setting temp2 = to 2nd
        firstItem->data = temp2; //set 1st = 2nd
        secondItem->data = temp1; //set 2nd = 1st
        return SUCCESS;
    }
}

/**
 * LinkedListSort() performs a selection sort on list to sort the elements into decending order. It
 * makes no guarantees of the addresses of the list items after sorting, so any ListItem referenced
 * before a call to LinkedListSort() and after may contain different data as only the data pointers
 * for the ListItems in the list are swapped. This function sorts the strings in ascending order
 * first by size (with NULL data pointers counting as 0-length strings) and then alphabetically
 * ascending order. So the list [dog, cat, duck, goat, NULL] will be sorted to [NULL, cat, dog,
 * duck, goat]. LinkedListSort() returns SUCCESS if sorting was possible. If passed a NULL pointer
 * for either argument, it will do nothing and return STANDARD_ERROR.
 *
 * @param list Any element in the list to sort.
 * @return SUCCESS if successful or STANDARD_ERROR is passed NULL pointers.
 */
int LinkedListSort(ListItem *list) {
    //*descending order.*sort strings in ascending order first by size then alphabetically ascending.
    //So the list [dog, cat, duck, goat, NULL] will be sorted to [NULL, cat, dog, duck, goat]
    //prob have to use strcmp()
    if (list == '\0') {
        return STANDARD_ERROR;
    } else {
        ListItem *ifirst, *jsecond; //following the given pseudo code.
        for (ifirst = list; ifirst->nextItem != '\0'; ifirst = ifirst->nextItem) {
            for (jsecond = ifirst->nextItem; jsecond != '\0'; jsecond = jsecond->nextItem) {
                if (ifirst->data == '\0') { //putting this in b/c is infinite looping
                    break; //stops loop if no data  //still infinite looping... kill me
                } else if (jsecond->data == '\0') {
                    LinkedListSwapData(jsecond, ifirst);
                } else {
                    if (strlen(ifirst->data) > strlen(jsecond->data)) { //swap if more
                        LinkedListSwapData(jsecond, ifirst); //chars in first
                    } else if (strlen(ifirst->data) == strlen(jsecond->data)) {
                        if (strcmp(ifirst->data, jsecond->data) > 0) { //if first > second,
                            LinkedListSwapData(jsecond, ifirst); //swap. ie b>a, swap
                        }
                    }
                }
            }
        }
        return SUCCESS;
    }
}

/**
 * LinkedListPrint() prints out the complete list to stdout. This function prints out the given
 * list, starting at the head if the provided pointer is not the head of the list, like "[STRING1,
 * STRING2, ... ]" If LinkedListPrint() is called with a NULL list it does nothing, returning
 * STANDARD_ERROR. If passed a valid pointer, prints the list and returns SUCCESS.
 *
 * @param list Any element in the list to print.
 * @return SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem * list) {
    //*start head. if called with NULL, break and standard error. else passed valid ptr, print and success
    if (list == '\0') {
        return STANDARD_ERROR;
    }
    list = LinkedListGetFirst(list); //grab first item
    ListItem *temp = list;
    printf("[%s", temp->data);
    temp = temp->nextItem;
    while (temp != '\0') { //loop
        printf(", %s", temp->data); //print
        temp = temp->nextItem; //go to next
    }
    printf("]\n");
    return SUCCESS;
}

