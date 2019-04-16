
#ifndef _matrix_H    
#define _matrix_H

void MakeIdentity(double result[3][3]);
void MatrixReset(double result[3][3]);
void MatrixCopy(double mat[3][3], double result[3][3]);
void MatrixMultiply(double mat1[3][3], double mat2[3][3], double result[3][3]);
void MatrixScalarMultiply(double f, double mat[3][3], double result[3][3]);
void MatrixAdd(double mat1[3][3], double mat2[3][3], double result[3][3]);
void MatrixTranspose(double mat[3][3], double result[3][3]);
void MatrixMult3to1(double m3[3][3], double m1[3], double result[3]);
void MatrixAdd3by1(double m1[3], double m2[3], double result[3]);
void MatrixCopy3by1(double mat[3], double result[3]);

#endif
