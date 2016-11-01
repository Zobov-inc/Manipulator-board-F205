/*
 * File: main.h
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 18-Aug-2016 22:21:45
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "kinem2_new5_types.h"

#include "rt_nonfinite.h"
#include "kinem2_new5.h"
#include "kinem2_new5_terminate.h"
#include "kinem2_new5_initialize.h"

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_kinem2_new5(int16_t px_a, int16_t py_a, double q[])
{
  double q4;

  /* Initialize function 'kinem2_new5' input arguments. */
  /* Call the entry-point 'kinem2_new5'. */
  kinem2_new5(px_a, py_a, q, q+1, q+2, &q4, q+3);
}

#endif

/*
 * File trailer for main.h
 *
 * [EOF]
 */
