/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: test.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Mar-2019 23:00:27
 */

/* Include Files */
#include <math.h>
#include "test.h"

/* Function Definitions */

/*
 * Arguments    : double w
 * Return Type  : double
 */
double test(double w)
{
  double testret;
  if (fabs(w) < 1.0020841800044864E-292) {
    testret = 1.0;
  } else {
    testret = 3.1415926535897931 * w;
    testret = sin(testret) / testret;
  }

  return testret;
}

/*
 * File trailer for test.c
 *
 * [EOF]
 */
