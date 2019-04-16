// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    BOARD_Init();
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // Declare Variables
    float fahr, celsius;
    int lower, upper, step;

    // Initialize Variables
    lower = 0; // lower limit of temperature
    upper = 300; // upper limit
    step = 20; // step size
    fahr = lower;

    // Column Headers for the table
    printf("%7c %3c\n", 'F', 'C');

    // Print out table for Fahrenheit to Celsius
    while (fahr <= upper) {
        celsius = (5.0 / 9.0)*(fahr - 32.0);
        printf("%7.1f %04d\n", (double) fahr, (int) celsius); // did %[width.precision]f
        fahr = fahr + step;
    //ended up changing celsius from double to int because it was printing 0-17 instead -017
    }

    //printing the new line asked by manual. also adding a header for table
    printf("\n");
    printf("%3c %8c\n", 'K', 'F');
            
    //print Kelvin to Fahrenheit conversion table
    float kelv = lower;
    while (kelv <= upper) {
        fahr = (kelv * 9.0 / 5.0 - 459.67);
        printf("%3.3f %5.6f\n", (double) kelv, (double) fahr);
        kelv = kelv + step;
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     **************************************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}
