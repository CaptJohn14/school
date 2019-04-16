/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_Rexp_api.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Mar-2019 21:11:36
 */

#ifndef _CODER_REXP_API_H
#define _CODER_REXP_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_Rexp_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void Rexp(real_T w[3], real_T R_exp[9]);
extern void Rexp_api(const mxArray * const prhs[1], int32_T nlhs, const mxArray *
                     plhs[1]);
extern void Rexp_atexit(void);
extern void Rexp_initialize(void);
extern void Rexp_terminate(void);
extern void Rexp_xil_terminate(void);

#endif

/*
 * File trailer for _coder_Rexp_api.h
 *
 * [EOF]
 */
