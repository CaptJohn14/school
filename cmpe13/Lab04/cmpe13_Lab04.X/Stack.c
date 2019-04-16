#include "BOARD.h"
#include "Stack.h"

#define STACK_SIZE 20

/* just for referencing 
 struct Stack {
    float stackItems[STACK_SIZE];
    int currentItemIndex;
    uint8_t initialized;
};
 * So:
 *  - stackItems: Contains all the floats that are in the queue in ascending
 *                order.
 *  - currentItemIndex: Contains the index in stackItems of the top of the
 *                      stack.
 *  - intialized: Stores whether or not the array was initialized. FALSE if
 *                it isn't and TRUE if it is.*/

void StackInit(struct Stack *stack) {
    //initializing stack.
    stack ->currentItemIndex = -1; //can use -> since pointer *
    stack ->initialized = TRUE;
}

int StackIsEmpty(const struct Stack *stack) {
    if (stack->currentItemIndex == -1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackIsFull(const struct Stack *stack) {
    if (stack->currentItemIndex == STACK_SIZE - 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackPush(struct Stack *stack, float value) {
    /* fail if full. need to access stack struct.*/
    //    if (stack->currentItemIndex > STACK_SIZE || stack->initialized == FALSE) {
    //        return STANDARD_ERROR;
    //    }
    if (StackIsFull(stack) == FALSE && stack->initialized == TRUE) {
        stack->currentItemIndex++;
        stack->stackItems[stack->currentItemIndex] = value;
    } else {
        return STANDARD_ERROR;
    }
}

int StackPop(struct Stack *stack, float *value) {
    if (StackIsEmpty(stack) == FALSE && stack->initialized == TRUE) {
        *value = stack->stackItems[stack->currentItemIndex];
        stack->currentItemIndex--;
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

int StackGetSize(const struct Stack *stack) {
    if (stack->initialized == TRUE) {
        return stack->currentItemIndex + 1;
    }
    else{
        return SIZE_ERROR;
    }
}