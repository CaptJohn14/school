////Libraries
//#include "BOARD.h"
//#include "MatrixMath.h"
//#include <math.h>
//#include <xc.h>
//#include <plib.h>
//#include <peripheral/osc.h>
//#include <peripheral/system.h>
//#include <stdint.h>
#include <stdio.h>

/******************************************************************************
 * Matrix - Matrix Operations
 *****************************************************************************/
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int x, y, z;
    for (x = 0; x < 3; x++) { //initializes the matrixes? Exactly how not to sure.
        for (y = 0; y < 3; y++) {
            float r = 0;
            for (z = 0; z < 3; z++) {
                r += mat1[x][z] * mat2[z][y];
            }
            result[x][y] = r;
        }
    }
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int x, y; //these x and y exclusive to this scope of MatrixAdd. shouldn't effect if reusing same variables
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = mat1[x][y] + mat2[x][y];
        }
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int x, y;
    int r; //true if 1, false if 0. 0 is initialized just cause.
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            if (mat1[x][y] == mat2[x][y]) {
                r = 1;
            } else {
                r = 0;
            }
        }
    }
    return r;
}

/******************************************************************************
 * Matrix - Scalar Operations
 *****************************************************************************/
void MatrixScalarMultiply(float f, float mat[3][3], float result[3][3]) {
    int x, y;
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = f * mat[x][y];
        }
    }
}

void MatrixScalarAdd(float f, float mat[3][3], float result[3][3]) {
    int x, y;
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = f + mat [x][y];
        }
    }
}

/******************************************************************************
 * Unary Matrix Operations
 *****************************************************************************/
float MatrixDeterminant(float mat[3][3]) {
    float a, b, c, x, y, z;
    int determinant = 0;
    a = mat[0][0] * mat[1][1] * mat[2][2]; //first diagonal going left down
    b = mat[0][1] * mat[1][2] * mat[2][0]; //second diagonal going left down
    c = mat[0][2] * mat[1][0] * mat[2][1]; //third diagonal going left down
    x = mat[2][0] * mat[1][1] * mat[0][2]; //first diagonal going left up
    y = mat[2][1] * mat[1][2] * mat[0][0]; //second diagonal going left up
    z = mat[2][2] * mat[1][0] * mat[0][1]; //third diagonal going left up
    determinant = (a + b + c)-(x + y + z);
    return determinant;
}

float MatrixTrace(float mat[3][3]) { //wtf is trace. lol
    //wow, that's so easy. trace is first diagonal added together. EASY PEASY
    int trace = mat[0][0] + mat[1][1] + mat[2][2];
    return trace;
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    //the rows are placed into columns and vice columns to rows.
    int x, y;
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = mat[y][x];
        }
    }
}

void MatrixPrint(float mat[3][3]) { //a little confused with matrix printing..
    int x, y;
    printf(" ____________________\n");
    for (x = 0; x < 3; x++) {
        printf("|");
        for (y = 0; y < 3; y++) {
            printf("% .2f |", (double) mat[x][y]);
        }
        printf("\n --------------------\n");
    }
    printf("\n");
}

/******************************************************************************
 * Extra Credit Matrix Operations
 *****************************************************************************/
void MatrixAdjugate(float mat[3][3], float result[3][3]) {
    int x, y;
    float c, d, e, f, g, h, i, j, k;
    c = (mat[1][1] * mat[2][2])-(mat[1][2] * mat[2][1]);
    d = (mat[0][2] * mat[2][1])-(mat[0][1] * mat[2][2]);
    e = (mat[0][1] * mat[1][2])-(mat[0][2] * mat[1][1]);
    f = (mat[1][2] * mat[2][0])-(mat[1][0] * mat[2][2]);
    g = (mat[0][0] * mat[2][2])-(mat[0][2] * mat[2][0]);
    h = (mat[0][2] * mat[1][0])-(mat[0][0] * mat[1][2]);
    i = (mat[1][0] * mat[2][1])-(mat[1][1] * mat[2][0]);
    j = (mat[0][1] * mat[2][0])-(mat[0][0] * mat[2][1]);
    k = (mat[0][0] * mat[1][1])-(mat[0][1] * mat[1][0]);
    float adjugate[3][3] = {
        {c, d, e},
        { f, g, h},
        { i, j, k}
    };
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = adjugate[x][y];
        }
    }
}
//MatrixInverse goes under unary matrix operations but it had to be under
//matrix adjugate to be used.

void MatrixInverse(float mat[3][3], float result[3][3]) {
    // kinda hard. will get back to it
    //back. so we need 1/determinant, so will prob just call that 
    float invdet = 1 / MatrixDeterminant(mat);
    //can either straight up multiply invdet to matrix or call matrixscalarmult
    MatrixAdjugate(mat, result);
    MatrixScalarMultiply(invdet, result, result);
}