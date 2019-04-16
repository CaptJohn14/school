// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "MatrixMath.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main() {
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    //making test matrices. 
    float matrix1[3][3] = {
        {0, 1, 0},
        { 1, 0, 1},
        { 0, 1, 0}
    };
    float matrix2[3][3] = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 0, 1}
    };
    float matrix3[3][3] = {
        {-1, 1, -1},
        {1, -1, 1},
        {-1, 1, -1}
    };
    float matrix4[3][3] = {
        {2, 1, 2},
        {1, 2, 1},
        {2, 1, 2}
    };
    float matrix5[3][3] = {
        {2, 2, 2},
        {-2, -2, -2},
        {2, 2, 2}
    };
    float matrix6[3][3] = {
        {1, 2, 3},
        {9, 8, 7},
        {5, 4, 6}
    };
    float matrix7[3][3] = {
        {2, 1, 0},
        {1, 0, 0},
        {2, 1, 1}
    };
    float matrix8[3][3] = {
        {0, 1, 2},
        {0, 0, 1},
        {1, 1, 2}
    };
    float result1[3][3] = {//just a matrix to place the result into
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float result2[3][3] = {//just a matrix to place the result into
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    int passed;
    int total = 0; //total numbers of functions passed 2/2 tests.
    //**************************************************************************
    //Matrix Multiply test
    //**************************************************************************
    passed = 0;
    MatrixMultiply(matrix1, matrix5, result1);
    float multcheck1[3][3] = {//gonna be used to compare to result. this should be answer
        {-2, -2, -2},
        {4, 4, 4},
        {-2, -2, -2}
    }; //from matrix.reshish.com    a online matrix calculator

    MatrixMultiply(matrix1, matrix2, result2);
    float multcheck2[3][3] = {
        {0, 1, 0},
        {2, 0, 2},
        {0, 1, 0}
    };
    if (MatrixEquals(multcheck1, result1) == 1) {
        passed++;
    }
    if (MatrixEquals(multcheck2, result2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixMultiply()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixMultiply()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixMultiply()\n");
    }

    //**************************************************************************
    //Matrix Add test
    //**************************************************************************
    passed = 0;
    MatrixAdd(matrix1, matrix2, result1);
    float addcheck1[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    MatrixAdd(matrix3, matrix5, result2);
    float addcheck2[3][3] = {
        {1, 3, 1},
        {-1, -3, -1},
        {1, 3, 1}
    };
    if (MatrixEquals(addcheck1, result1) == 1) {
        passed++;
    }
    if (MatrixEquals(addcheck2, result2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixAdd()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixAdd()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixAdd()\n");
    }
    //**************************************************************************
    //Matrix equals test
    //**************************************************************************
    passed = 0;
    if (MatrixEquals(matrix1, matrix1) == 1) {
        passed++;
    }
    if (MatrixEquals(matrix2, matrix2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixEquals()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixEquals()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixEquals()\n");
    }
    //**************************************************************************
    //Matrix scalar multiplication test
    //**************************************************************************
    passed = 0;
    float scalar1 = 2;
    float scalar2 = 10;
    MatrixScalarMultiply(scalar1, matrix4, result1);
    float smultcheck1[3][3] = {
        {4, 2, 4},
        {2, 4, 2},
        {4, 2, 4}
    };
    MatrixScalarMultiply(scalar2, matrix5, result2);
    float smultcheck2[3][3] = {
        {20, 20, 20},
        {-20, -20, -20},
        {20, 20, 20}
    };

    if (MatrixEquals(smultcheck1, result1) == 1) {
        passed++;
    }
    if (MatrixEquals(smultcheck2, result2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixScalarMultiply()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixScalarMultiply()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixScalarMultiply()\n");
    }
    //**************************************************************************
    //Matrix Scalar Add test
    //**************************************************************************
    passed = 0;
    MatrixScalarAdd(scalar1, matrix5, result1);
    float saddcheck1[3][3] = {
        {4, 4, 4},
        {0, 0, 0},
        {4, 4, 4}
    };
    MatrixScalarAdd(scalar2, matrix5, result2);
    float saddcheck2[3][3] = {
        {12, 12, 12},
        {8, -8, 8},
        {12, 12, 12}
    };

    if (MatrixEquals(saddcheck1, result1) == 1) {
        passed++;
    }
    if (MatrixEquals(saddcheck2, result2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixScalarAdd()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixScalarAdd()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixScalarAdd()\n");
    }
    //**************************************************************************
    //Matrix Determinant test
    //**************************************************************************
    passed = 0;
    if (MatrixDeterminant(matrix6) == -30) {
        passed++;
    }
    if (MatrixDeterminant(matrix3) == 0) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixDeterminant()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixDeterminant()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixDeterminant()\n");
    }

    //**************************************************************************
    //Matrix Trace test
    //**************************************************************************
    passed = 0;
    if (MatrixTrace(matrix1) == 0) {
        passed++;
    }
    if (MatrixTrace(matrix2) == 3) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixTrace()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixTrace()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixTrace()\n");
    }

    //**************************************************************************
    //Matrix Transpose test
    //**************************************************************************
    passed = 0;
    MatrixTranspose(matrix1, result1);
    float checktrans1[3][3] = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    MatrixTranspose(matrix5, result2);
    float checktrans2[3][3] = {
        {2, -2, 2},
        {2, -2, 2},
        {2, -2, 2}
    };
    if (MatrixEquals(checktrans1, result1) == 1) {
        passed++;
    }
    if (MatrixEquals(checktrans2, result2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixTranspose()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixTranspose()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixTranspose()\n");
    }
    //**************************************************************************
    //Matrix Adjugate test
    //**************************************************************************
    passed = 0;
    MatrixAdjugate(matrix1, result1);
    float checkadj1[3][3] = {
        {-1, 0, 1},
        {0, 0, 0},
        {1, 0, -1}
    };
    MatrixAdjugate(matrix2, result2);
    float checkadj2[3][3] = {
        {1, 0, -1},
        {0, 0, 0},
        {-1, 0, 1}
    };
    if (MatrixEquals(checkadj1, result1) == 1) {
        passed++;
    }
    if (MatrixEquals(checkadj2, result2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixAdjugate()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixAdjugate()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixAdjugate()\n");
    }
    //**************************************************************************
    //Matrix Inverse test
    //**************************************************************************
    passed = 0;
    MatrixInverse(matrix7, result1);
    float checkinv1[3][3] = {
        {0, 1, 0},
        {1, -2, 0},
        {-1, 0, 1}
    };
    MatrixInverse(matrix8, result2);
    float checkinv2[3][3] = {
        {-1, 0, 1},
        {1, -2, 0},
        {0, 1, 0}
    };
    if (MatrixEquals(checkinv1, result1) == 1) {
        passed++;
    }
    if (MatrixEquals(checkinv2, result2) == 1) {
        passed++;
    }
    if (passed == 2) {
        printf("%s", "Passed (2/2): MatrixInverse()\n");
        total++;
    } else if (passed == 1) {
        printf("%s", "Passed (1,2): MatrixInverse()\n");
    } else {
        printf("%s", "Passed (0,2): MatrixInverse()\n");
    }

    //**************************************************************************
    //Matrix Printing and Print test
    //**************************************************************************
    double percentage = (total * 10); //for the percentage of passed functions

    printf("%s", "-------------------------------------\n");
    printf("%d %s%.1f%s\n", total, "out of 10 functions passed (", percentage, "%)");

    MatrixPrint(matrix1);
    float checkprint[3][3] = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    printf("Expected output of MatrixPrint(): \n");
    MatrixPrint(checkprint);

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}

