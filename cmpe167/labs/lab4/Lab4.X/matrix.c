#include <math.h>
#include "matrix.h"


/*==============================================================================
 * Make result identity matrix
 =============================================================================*/
void MakeIdentity(double result[3][3]) {
    static int i = 0;
    static int j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j) {
                result[i][j] = 1;
            } else {
                result[i][j] = 0;
            }
        }
    }
}

/*==============================================================================
 * reset matrix to all 0
 =============================================================================*/
void MatrixReset(double result[3][3]) {
    static int i = 0;
    static int j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = 0;
        }
    }
}

/*==============================================================================
 * Matrix copy
 =============================================================================*/
void MatrixCopy(double mat[3][3], double result[3][3]) {
    static int i = 0;
    static int j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat[i][j];
        }
    }
}
void MatrixCopy3by1(double mat[3], double result[3]){
    int i = 0;
    for(i=0; i<3; i++){
        result[i] = mat[i];
    }
}
/*==============================================================================
 * Matrix-Matrix Multiply. Provide result array
 =============================================================================*/
void MatrixMultiply(double mat1[3][3], double mat2[3][3], double result[3][3]) {
    int x, y, z;
    for (x = 0; x < 3; x++) { //initializes the matrixes? Exactly how not to sure.
        for (y = 0; y < 3; y++) {
            double r = 0;
            for (z = 0; z < 3; z++) {
                r += mat1[x][z] * mat2[z][y];
            }
            result[x][y] = r;
        }
    }
}

/*==============================================================================
 * Matrix Scalar Multiply. 
 =============================================================================*/
void MatrixScalarMultiply(double f, double mat[3][3], double result[3][3]) {
    int x, y;
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = f * mat[x][y];
        }
    }
}

/*==============================================================================
 * Matrix-Matrix Addition
 =============================================================================*/
void MatrixAdd(double mat1[3][3], double mat2[3][3], double result[3][3]) {
    int x, y; //these x and y exclusive to this scope of MatrixAdd. shouldn't effect if reusing same variables
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = mat1[x][y] + mat2[x][y];
        }
    }
}

/*==============================================================================
 * Matrix-Transpose
 =============================================================================*/
void MatrixTranspose(double mat[3][3], double result[3][3]) {
    //the rows are placed into columns and vice columns to rows.
    int x, y;
    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            result[x][y] = mat[y][x];
        }
    }
}
/*==============================================================================
 * 3x3Matrix multiply 3x1Matrix
 =============================================================================*/
void MatrixMult3to1(double m3[3][3], double m1[3], double result[3]){
    int i = 0;
    for(i=0; i<3; i++){
        result[i] = m1[0]*m3[i][0] + m1[1]*m3[i][1] + m1[2]*m3[i][2];
    }
}
/*==============================================================================
 * 3x1Matrix add 3x1Matrix
 =============================================================================*/
void MatrixAdd3by1(double m1[3], double m2[3], double result[3]){
    
}