/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IntegrateOpenLoop_api.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 11-Mar-2019 01:53:34
 */

#ifndef _CODER_INTEGRATEOPENLOOP_API_H
#define _CODER_INTEGRATEOPENLOOP_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_IntegrateOpenLoop_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void IntegrateOpenLoop(real_T Rminus[9], real_T gyros[3], real_T deltaT,
  real_T Rplus[9]);
extern void IntegrateOpenLoop_api(const mxArray * const prhs[3], int32_T nlhs,
  const mxArray *plhs[1]);
extern void IntegrateOpenLoop_atexit(void);
extern void IntegrateOpenLoop_initialize(void);
extern void IntegrateOpenLoop_terminate(void);
extern void IntegrateOpenLoop_xil_terminate(void);

#endif

/*
 * File trailer for _coder_IntegrateOpenLoop_api.h
 *
 * [EOF]
 */
