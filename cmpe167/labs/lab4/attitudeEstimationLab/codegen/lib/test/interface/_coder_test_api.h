/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_test_api.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Mar-2019 23:00:27
 */

#ifndef _CODER_TEST_API_H
#define _CODER_TEST_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_test_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern real_T test(real_T w);
extern void test_api(const mxArray * const prhs[1], int32_T nlhs, const mxArray *
                     plhs[1]);
extern void test_atexit(void);
extern void test_initialize(void);
extern void test_terminate(void);
extern void test_xil_terminate(void);

#endif

/*
 * File trailer for _coder_test_api.h
 *
 * [EOF]
 */
