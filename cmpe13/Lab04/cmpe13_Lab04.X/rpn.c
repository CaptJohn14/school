// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Stack.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);

// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#ifndef LAB4_TESTING

int main() {
    BOARD_Init();
#else

int their_main(void) {
#endif // LAB4_TESTING
    //    while (token != NULL) {
    //        printf(" %s\n", token);
    //        token = strtok(NULL, s);
    //    }
    /******************************** Your custom code goes below here *******************************/
    printf("Hello there. I am Johnson's personal RPN calculator. \n");
    struct Stack myStack;
    char instr[60]; //instr is input string
    char *token;

    float op1, op2, result, checker;
    const char s[1] = " "; //space is my delimiter? i guess.

    while (1) {
        memset(instr, 0, 60); //clears the array to all null
        StackInit(&myStack);
        printf("%s", "Enter calculation wanted in the form of: (number) (number) (operator)\n");
        fgets(instr, 60, stdin); //stdin = standard input. which is from keyboard
        token = strtok(instr, s); //"parsing" string into separate tokens.

        while (token != NULL) {
            if (atof(token) > 0 || *token == '0') {    //atof() converts to double.
                StackPush(&myStack, atof(token));      //here i am pushing my token that has been converted to a double
            } else if (*token == '+' || *token == '-' || *token == '*' || *token == '/') {
                char operator = *token;           //I'm doing this because it makes my code ALOT shorter. Was like 4 times longer without.
                checker = StackPop(&myStack, &op1);         //checker is checking for errors.
                if (checker == STANDARD_ERROR) {
                    printf("%s", "ERROR: Not enough operands before operator\n");
                    break;
                }
                checker = StackPop(&myStack, &op2);
                if (checker == STANDARD_ERROR) {
                    printf("%s", "ERROR: Not enough operands before operator\n");
                    break;
                }

                if (operator == '+') {               //because i saved operator, code is ALOT shorter (:)
                    result = op1 + op2;
                } else if (operator == '-') {
                    result = op2 - op1;
                } else if (operator == '*') {
                    result = op1 * op2;
                } else if (operator == '/') {
                    result = op2 / op1;
                }

                checker = StackPush(&myStack, result);      //check and push result
                if (checker == STANDARD_ERROR) {
                    printf("%s", "ERROR: No more room on stack\n");
                    break;
                }
            }
            else{
                printf("ERROR: Invalid character in the RPN string\n");
                break;
            }
            token = strtok(NULL, " ");
        }
        if (StackGetSize(&myStack) > 1) {
            printf("ERROR: Invalid RPN calculation: more or less than one item in the stack\n");
        } else {
            StackPop(&myStack, &result);
            printf("Answer: %f\n\n", (double) result);
        }
    }
    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */
int ProcessBackspaces(char *rpn_sentence) {
    return 0;
}
