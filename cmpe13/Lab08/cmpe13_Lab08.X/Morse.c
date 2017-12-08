
// Name: Johnson Le
//Student ID: 1512346 

#include "Morse.h"
#include "BOARD.h"
#include "Tree.h"
#include "Buttons.h"
//copied over the comments from provided Morse.h

/**
 * This library implements functions for decoding Morse code. It builds upon the Tree library to use
 * a binary tree data structure for storing the codepoints for all characters. Decoding is then done
 * through simple tree traversal. Additional functionality relies on a 100Hz clock to check the
 * button states for decoding the input of Morse characters through a physical button (BTN4).
 */

#include <stdint.h>

/**
 * This enum specifies the different possible inputs to MorseDecode().
 */
//typedef enum {
//    MORSE_CHAR_DOT = '.',          /// A Morse DOT.
//    MORSE_CHAR_DASH = '-',         /// A Morse DASH.
//    MORSE_CHAR_END_OF_CHAR = '#',  /// Signifies END_OF_CHAR: that the Morse string is over and the
//                                    // character should be decoded.
//    MORSE_CHAR_DECODE_RESET = '\0' /// Signifies a reset: that the internal state of `MorseDecode()`
//                                    // should be reset.
//} MorseChar;

/**
 * This enum declares constants for all of the possible events that this module can trigger.
 * Specifically there is an event for each Morse code character that can be found as well as a
 * null-event.
 */
//typedef enum {
//	MORSE_EVENT_NONE,
//	MORSE_EVENT_DOT,
//	MORSE_EVENT_DASH,
//	MORSE_EVENT_INTER_LETTER,
//	MORSE_EVENT_INTER_WORD
//} MorseEvent;

/**
 * Define the length of time, in units of .01s, that either button-presses or time between button
 * presses should be.
 */
//typedef enum {
//	// Specify the downtime of the button press.
//	MORSE_EVENT_LENGTH_DOWN_DOT = 25,
//	MORSE_EVENT_LENGTH_DOWN_DASH = 50,
//
//	// Specify the uptime between button presses.
//	MORSE_EVENT_LENGTH_UP_INTER_LETTER = 100,
//	MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT = 200
//} MorseEventLength;

typedef enum {
    WAITING,
    DOT,
    DASH,
    INTER_LETTER
} morsestate;


static morsestate state = WAITING;
static const char tree[63] = {'\0', 'E', 'I', 'S', 'H', '5', '4', 'V', '\0', '3', 'U', 'F', '\0', '\0', '\0', '\0', '2',
    'A', 'R', 'L', '\0', '\0', '\0', '\0', '\0', 'W', 'P', '\0', '\0', 'J', '\0', '1',
    'T', 'N', 'D', 'B', '6', '\0', 'X', '\0', '\0', 'K', 'C', '\0', '\0', 'Y', '\0', '\0',
    'M', 'G', 'Z', '7', '\0', 'Q', '\0', '\0', 'O', '\0', '8', '\0', '\0', '9', '0'};

Node *head;
Node *temp;

//static int i;

/**
 * This function initializes the Morse code decoder. This is primarily the generation of the
 * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
 * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
 * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
 * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
 * also initializes the Buttons library so that MorseCheckEvents() can work properly.
 * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
 */
int MorseInit(void) {
    ButtonsInit();
    head = TreeCreate(6, tree);
    //printf("%c", head->rightChild->rightChild->data);
    //printf(head);
    //    for (i=0; i <75 ; i++){
    //        printf(head);
    //    }
    if (head != NULL) {
        temp = head;
        state = WAITING;
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
 * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
 * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
 * decoding and return the decoded character. During that call to MorseDecode() the return value
 * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
 * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
 * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
 * then the internal state should be reset and STANDARD_ERROR should be returned.
 * 
 * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
 * 
 * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
 *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
 *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
 *         or `in` isn't a valid member of the MorseChar enum.
 */
char MorseDecode(MorseChar in) {
    char data;
    //    if (head == NULL) {
    //        return STANDARD_ERROR;
    //    }
    switch (in) {
        case MORSE_CHAR_DOT:
            printf("in decode char dot\n");
            if (temp->leftChild == NULL) {
                temp = head; //resets
                return STANDARD_ERROR;
            } else {
                temp = temp ->leftChild; //traverse left
                return SUCCESS;
            }
            break;
        case MORSE_CHAR_DASH:
            printf("in decode char dash\n");
            if (temp->rightChild == NULL) {
                temp = head; //resets
                return STANDARD_ERROR;
            } else {
                temp = temp->rightChild; //traverse right
                return SUCCESS;
            }
            break;
        case MORSE_CHAR_END_OF_CHAR:
            printf("in decode end\n");
            data = temp->data; //grabs data and returns. resets tree.
            temp = head;
            return data;
            break;
        case MORSE_CHAR_DECODE_RESET:
            printf("in decode reset\n");
            temp = head; //reset tree.
            return SUCCESS;
            break;
        default:
            printf("decode broke or some shiznit\n");
            return STANDARD_ERROR;
    }
}

/**
 * This function calls ButtonsCheckEvents() once per call and returns which, if any,
 * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
 * events in its input and should be called at 100Hz so that the timing works. The
 * length that BTN4 needs to be held down for a dot is >= 0.25s and < 0.50s with a dash being a button
 * down event for >= 0.5s. The button uptime various between dots/dashes (>= .5s), letters
 * (>= 1s), and words (>= 2s).
 *
 * @note This function assumes that the buttons are all unpressed at startup, so that the first
 *       event it will see is a BUTTON_EVENT_*DOWN.
 *
 * So pressing the button for 0.1s, releasing it for 0.1s, pressing it for 0.3s, and then waiting
 * will decode the string '.-' (A). It will trigger the following order of events:
 * 9 MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, 39 MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, 69
 * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_INTER_WORDs.
 * 
 * @return The MorseEvent that occurred.
 */


static int btncounter = 0;

MorseEvent MorseCheckEvents(void) {
    btncounter++;
    ButtonEventFlags event = ButtonsCheckEvents();
    switch (state) {
        case WAITING:
            if (event == BUTTON_EVENT_4DOWN) { //button 4 is pressed
                state = DOT; //go to DOT state
                event = BUTTON_EVENT_NONE;
                btncounter = 0; //reset how long button pressed
                //            } else {
                //                state = WAITING; // go to WAITING state
            }
            //    printf("waiting\n");
            break;
        case DOT:
            if (btncounter >= MORSE_EVENT_LENGTH_DOWN_DASH) {//check this
                state = DASH; //go to DASH state
            }
            if (event == BUTTON_EVENT_4UP) {
                state = INTER_LETTER; //go to INTER_LETTER state
                event = BUTTON_EVENT_NONE;
                btncounter = 0; //reset how long button pressed
                return MORSE_EVENT_DOT;
            }
            printf("dot\n");
            break;
        case DASH:
            if (event == BUTTON_EVENT_4UP) {
                state = INTER_LETTER; //go to INTER_LETTER state
                event = BUTTON_EVENT_NONE;
                btncounter = 0; //reset how long button pressed
                return MORSE_EVENT_DASH;
            }
            printf("dash\n");
            break;
        case INTER_LETTER:
            if (event == BUTTON_EVENT_4DOWN) {
                event = BUTTON_EVENT_NONE;
                printf("inter_letter\n");
                if (btncounter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER) {
                    state = DOT; //go back to DOT
                    btncounter = 0; //reset how long button pressed
                    printf("inter_letter22222222\n");
                    return MORSE_EVENT_INTER_LETTER; //inter letter event returned
                } else { //not sure if else.
                    state = DOT; //go back to DOT
                    btncounter = 0; //reset how long button pressed
                    return MORSE_EVENT_NONE;
                }
            }
            if (btncounter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT) { //c>=inter letter timeout
                state = WAITING; //go back to waiting
                printf("waiting\n");
                return MORSE_EVENT_INTER_WORD; //inter word event
            }

            break;
    }
    return MORSE_EVENT_NONE;
}
