/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IntegrateOpenLoop.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 11-Mar-2019 01:53:34
 */

#ifndef INTEGRATEOPENLOOP_H
#define INTEGRATEOPENLOOP_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "IntegrateOpenLoop_types.h"

/* Function Declarations */
extern void IntegrateOpenLoop(const double Rminus[9], const double gyros[3],
  double deltaT, double Rplus[9]);

#endif

/*
 * File trailer for IntegrateOpenLoop.h
 *
 * [EOF]
 */
