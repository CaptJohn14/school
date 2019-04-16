/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IntegrateOpenLoop.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 11-Mar-2019 01:53:34
 */

/* Include Files */
#include "IntegrateOpenLoop.h"

/* Function Definitions */

/*
 * function [Rplus] = IntegrateOpenLoop(Rminus, gyros, deltaT)
 *
 *  Function to Integrate the gyros to the attitude DCM
 *
 *  Inputs: Previous attitute DCM (Rminus)
 *          Body Fixed Rotation rates ([p;q;r]) in rad/s
 *          Time between samples (deltaT) in seconds
 *
 *  Outputs: New DCM (Rplus)ssssss
 *
 *  Note: This code implements both a forward integration as well as the
 *  matrix exponential version of the integration, change the flag in the
 *  code below to pick between them.
 * Arguments    : const double Rminus[9]
 *                const double gyros[3]
 *                double deltaT
 *                double Rplus[9]
 * Return Type  : void
 */
void IntegrateOpenLoop(const double Rminus[9], const double gyros[3], double
  deltaT, double Rplus[9])
{
  double dv0[9];
  int i0;
  int i1;
  int Rplus_tmp;

  /*  UseMatrixExponential = 0;   % set to zero for forward integration */
  /*  if (UseMatrixExponential) */
  /*      Rplus = Rminus * Rexp(gyros*deltaT); */
  /*  else */
  /*  function rx = rcross(r) */
  /*  forms the skew symmetric x-product matrix of a 3x1 vector */
  dv0[0] = 0.0;
  dv0[3] = -gyros[2];
  dv0[6] = gyros[1];
  dv0[1] = gyros[2];
  dv0[4] = 0.0;
  dv0[7] = -gyros[0];
  dv0[2] = -gyros[1];
  dv0[5] = gyros[0];
  dv0[8] = 0.0;
  for (i0 = 0; i0 < 3; i0++) {
    for (i1 = 0; i1 < 3; i1++) {
      Rplus_tmp = i0 + 3 * i1;
      Rplus[Rplus_tmp] = 0.0;
      Rplus[Rplus_tmp] = (Rminus[i0] * dv0[3 * i1] + Rminus[i0 + 3] * dv0[1 + 3 *
                          i1]) + Rminus[i0 + 6] * dv0[2 + 3 * i1];
    }
  }

  for (i0 = 0; i0 < 9; i0++) {
    Rplus[i0] = Rminus[i0] + Rplus[i0] * deltaT;
  }

  /*  end */
}

/*
 * File trailer for IntegrateOpenLoop.c
 *
 * [EOF]
 */
