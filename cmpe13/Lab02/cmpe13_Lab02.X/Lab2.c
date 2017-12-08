// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
double Round(double operand);

// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void) {
    BOARD_Init();
#else

int their_main(void) {
#endif // LAB2_TESTING

    //downgrading compiler, couldn't reach subroutines unless they are above
    //so si defined these functions above my code.

    /********************************************************************************
     * Define the Absolute Value function here.
     ********************************************************************************/
    double AbsoluteValue(double x) {
        if (x < 0) {
            x = -x;
        }
        return x;
    }

    /*********************************************************************************
     * Define the Fahrenheit to Celsius function here.
     ********************************************************************************/
    double FahrenheitToCelsius(double x) {
        x = (x - 32) * 5 / 9;
        return x;
    }

    /*********************************************************************************
     * Define the Celsius to Fahrenheit function here.
     ********************************************************************************/
    double CelsiusToFahrenheit(double x) {
        x = (x * 9 / 5) + 32;
        return x;
    }

    /********************************************************************************
     * Define the Average function here.
     *******************************************************************************/
    double Average(double x, double y) {
        x = (x + y) / 2;
        return x;
    }

    /*********************************************************************************
     * Define the Tangent function that takes input in degrees (Note: Your tangent 
     * function relies on a call from the tangent function of math.h which uses 
     * radians).
     ********************************************************************************/
    double Tangent(double x) {
        x = M_PI * (x / 100);
        x = tan(x);
        return x;
    }

    /*********************************************************************************
     * Define the Round function here.
     * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
     * this function.
     ********************************************************************************/
    double Round(double x) {
        int round; //int does not have decimal.
        if (x >= 0) {
            x = x + 0.5;
        } else if (x < 0) {
            x = x - 0.5;
        }
        round = x;
        return round;
    }
    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    printf("%s", "Hello! I am Johnson's personal Calculator Program"); //hello
    while (1) {
        printf("%s", "\nHow may I assist you? Choose an operation: *, /, +, -, v, a, c, f, t, r\n");
        printf("%s", "\nOperation Chosen: ");
        char o = 0; //o = operation 
        char z = 0; //to escape scanf?
        double op1; //op1 for operand
        double op2;
        double result;

        scanf("%c%c", &o, &z); // get user input for operation
        if (o == 'a' || o == 'c' || o == 'f' || o == 't' || o == 'r') {
            printf("%s", "Enter operand: ");
            scanf("%f%c", &op1, &z); //%lf auto to double if needed
        } else if (o == '*' || o == '/' || o == '+' || o == '-' || o == 'v') {
            //these are the operations that require 2 operands
            printf("%s", "Enter first operand:  ");
            scanf("%lf%c", &op1, &z);
            printf("%s", "Enter second operand: ");
            scanf("%lf%c", &op2, &z);
        } else {
            o = 0;
        }

        if (o == 0) {
            printf("%s", "\nYou have inputed and invalid operator. Try again!!");
        } else if (o != 0) {
            printf("%s", "\nHere's the result: ");
        }

        if (o == '+') {
            result = op1 + op2;
            printf("%f %c %f %c %f", op1, o, op2, '=', result);
        } else if (o == '-') {
            result = op1 - op2;
            printf("%f %c %f %c %f", op1, o, op2, '=', result);
        } else if (o == '*') {
            result = op1 * op2;
            printf("%f %c %f %c %f", op1, o, op2, '=', result);
        } else if (o == '/') {
            result = op1 / op2;
            printf("%f %c %f %c %f", op1, o, op2, '=', result);
        } else if (o == 'v') {
            result = Average(op1, op2);
            printf("%s %f %c %f %s %f", "The average of ", op1, '&', op2, "is", result);
        } else if (o == 'a') {
            result = AbsoluteValue(op1);
            printf("%s %f %s %f", "The absolute value of ", op1, "is", result);
        } else if (o == 'c') {
            result = FahrenheitToCelsius(op1);
            printf("%f %s %f %s", op1, "Fahrenheit is ", result, "Celsius!");
        } else if (o == 'f') {
            result = CelsiusToFahrenheit(op1);
            printf("%f %s %f %s", op1, "Celsius is ", result, "Fahrenheit!");
        } else if (o == 't') {
            result = Tangent(op1);
            printf("%s %f %s %f", "The tangent of ", op1, "is", result);
        } else if (o == 'r') {
            result = Round(op1);
            printf("%s %f %s %f", "Rounding ", op1, "is", result);
        }
    }
    //    //now for printing the outcome
    //    if (o == 0) {
    //        printf("%s", "\nYou have inputed and invalid operator");
    //    } else if (o == '*' || o == '/' || o == '+' || o == '-' || o == 'v') {
    //        printf("%s%f %c %f %c %f", "\nHere's the result: ", op1, o, op2, '=', result);
    //    } else {
    //        printf("%s%f", "\nHere's the result: ", result);
    //    }

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/
    while (1);
}
