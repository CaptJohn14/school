/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: Rexp.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Mar-2019 21:11:36
 */

/* Include Files */
#include <math.h>
#include "Rexp.h"

/* Function Definitions */

/*
 * function R_exp = Rexp(w)
 *
 *  returns the exponential Rodrigues parameter form of the integration that
 *  keeps R on SO(3). See Park and Chung paper.
 * Arguments    : const double w[3]
 *                double R_exp[9]
 * Return Type  : void
 */
void Rexp(const double w[3], double R_exp[9])
{
  double scale;
  double absxk;
  double t;
  double wnorm;
  double rx[9];
  int i0;
  int i1;
  int R_exp_tmp;
  static const signed char iv0[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  scale = 3.3121686421112381E-170;
  absxk = fabs(w[0]);
  if (absxk > 3.3121686421112381E-170) {
    wnorm = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    wnorm = t * t;
  }

  absxk = fabs(w[1]);
  if (absxk > scale) {
    t = scale / absxk;
    wnorm = 1.0 + wnorm * t * t;
    scale = absxk;
  } else {
    t = absxk / scale;
    wnorm += t * t;
  }

  absxk = fabs(w[2]);
  if (absxk > scale) {
    t = scale / absxk;
    wnorm = 1.0 + wnorm * t * t;
    scale = absxk;
  } else {
    t = absxk / scale;
    wnorm += t * t;
  }

  wnorm = scale * sqrt(wnorm);

  /*  function rx = rcross(r) */
  /*  forms the skew symmetric x-product matrix of a 3x1 vector */
  rx[0] = 0.0;
  rx[3] = -w[2];
  rx[6] = w[1];
  rx[1] = w[2];
  rx[4] = 0.0;
  rx[7] = -w[0];
  rx[2] = -w[1];
  rx[5] = w[0];
  rx[8] = 0.0;
  scale = wnorm / 2.0 / 3.1415926535897931;
  if (scale < 1.0020841800044864E-292) {
    scale = 1.0;
  } else {
    scale *= 3.1415926535897931;
    scale = sin(scale) / scale;
  }

  /*  sinc in matlab has an additional pi term, do not do this in C */
  absxk = scale * cos(wnorm / 2.0);
  scale = scale * scale / 2.0;
  for (i0 = 0; i0 < 3; i0++) {
    for (i1 = 0; i1 < 3; i1++) {
      R_exp_tmp = i0 + 3 * i1;
      R_exp[R_exp_tmp] = ((double)iv0[R_exp_tmp] + absxk * rx[R_exp_tmp]) +
        ((scale * rx[i0] * rx[3 * i1] + scale * rx[i0 + 3] * rx[1 + 3 * i1]) +
         scale * rx[i0 + 6] * rx[2 + 3 * i1]);
    }
  }
}

/*
 * File trailer for Rexp.c
 *
 * [EOF]
 */
