/*
 * File: kinem2_new5.c
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 18-Aug-2016 22:21:45
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kinem2_new5.h"
#include "norm.h"

/* Function Definitions */

/*
 * Arguments    : double px_a
 *                double py_a
 *                double *q1
 *                double *q2
 *                double *q3
 *                double *q4
 *                double *q5
 * Return Type  : void
 */
void kinem2_new5(double px_a, double py_a, double *q1, double *q2, double *q3,
                 double *q4, double *q5)
{
  double sinq4;
  double cosq4;
  double sinq5;
  double P2_a[3];
  double Xr_a[3];
  int k;
  static const unsigned char P0_a[3] = { 0U, 0U, 130U };

  double Yr_a[3];
  double B;
  double b_B;
  double b_P2_a[3];
  double b_Xr_a;
  double b_Yr_a;
  double T[9];
  double P2x_r;
  double P1x_r;
  double P1z_r;
  double c_Xr_a[3];
  double b_P1x_r[3];
  double c_P1x_r[3];
  double P1_a_2[3];
  int i0;
  double cosq2new;
  double cosq3minusq2;
  double q2new;
  double d0;
  double b_P1_a_2;
  double delta;
  unsigned char point_data[3];
  static const unsigned char uv0[3] = { 0U, 0U, 130U };

  double Lambda;
  double d_Xr_a[3];
  double b_px_a[3];
  double c_P1_a_2[9];
  double cosq5;
  boolean_T flag;
  sinq4 = 0.0;
  cosq4 = 0.0;
  sinq5 = 0.0;

  /* inverse cinematics alternative */
  P2_a[0] = px_a - 5.2491592393165018E-31;
  P2_a[1] = py_a - 8.572527594031473E-15;
  P2_a[2] = 215.0;
  *q1 = atan((px_a - 5.2491592393165018E-31) / (py_a - 8.572527594031473E-15));

  /* subsidiary coordinate system */
  /* P0 is a centre */
  for (k = 0; k < 3; k++) {
    Xr_a[k] = P2_a[k] - (double)P0_a[k];
  }

  Yr_a[0] = 0.0 - 130.0 * Xr_a[1];
  Yr_a[1] = 130.0 * Xr_a[0];
  Yr_a[2] = 0.0 * Xr_a[1] - 0.0 * Xr_a[0];
  B = norm(Xr_a);
  b_B = norm(Yr_a);

  /* calculations in new _r system */
  for (k = 0; k < 3; k++) {
    b_Xr_a = Xr_a[k] / B;
    b_Yr_a = Yr_a[k] / b_B;
    b_P2_a[k] = P2_a[k] - (double)P0_a[k];
    T[k] = b_Xr_a;
    T[3 + k] = b_Yr_a;
    Xr_a[k] = b_Xr_a;
    Yr_a[k] = b_Yr_a;
  }

  P2x_r = norm(b_P2_a);
  P1x_r = ((P2x_r * P2x_r + 22500.0) - 18225.0) / (2.0 * P2x_r);
  P1z_r = sqrt(22500.0 - P1x_r * P1x_r);
  c_Xr_a[0] = Xr_a[1] * Yr_a[2] - Xr_a[2] * Yr_a[1];
  c_Xr_a[1] = Xr_a[2] * Yr_a[0] - Xr_a[0] * Yr_a[2];
  c_Xr_a[2] = Xr_a[0] * Yr_a[1] - Xr_a[1] * Yr_a[0];
  b_P1x_r[0] = P1x_r;
  b_P1x_r[1] = 0.0;
  b_P1x_r[2] = P1z_r;
  c_P1x_r[0] = P1x_r;
  c_P1x_r[1] = 0.0;
  c_P1x_r[2] = -P1z_r;
  for (k = 0; k < 3; k++) {
    T[6 + k] = c_Xr_a[k];
    b_B = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      b_B += T[k + 3 * i0] * b_P1x_r[i0];
    }

    Xr_a[k] = (double)P0_a[k] + b_B;
    b_B = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      b_B += T[k + 3 * i0] * c_P1x_r[i0];
    }

    P1_a_2[k] = (double)P0_a[k] + b_B;
  }

  if (Xr_a[2] > P1_a_2[2]) {
    for (k = 0; k < 3; k++) {
      P1_a_2[k] = Xr_a[k];
    }
  }

  /* ?????????? ???? */
  cosq2new = P1_a_2[1] / (150.0 * cos(*q1));
  cosq3minusq2 = ((py_a - 8.572527594031473E-15) - P1_a_2[1]) / (-135.0 * cos
    (*q1));
  if ((P1_a_2[2] - 130.0) / 150.0 > 0.0) {
    q2new = acos(cosq2new);
  } else {
    q2new = -acos(cosq2new);
  }

  *q2 = q2new;
  if ((215.0 - P1_a_2[2]) / 135.0 > 0.0) {
    d0 = acos(cosq3minusq2);
  } else {
    d0 = -acos(cosq3minusq2);
  }

  *q3 = d0 + q2new;
  for (k = 0; k < 3; k++) {
    P1_a_2[k] = P2_a[k] - P1_a_2[k];
  }

  B = norm(P1_a_2);

  /* if OP2 || X1 than use P1 instead of O */
  b_B = 0.0;
  for (k = 0; k < 3; k++) {
    b_P1_a_2 = P1_a_2[k] / B;
    b_B += P2_a[k] * b_P1_a_2;
    P1_a_2[k] = b_P1_a_2;
  }

  if (fabs(b_B) > 0.01) {
    /* dot(P2_a,X1_a) or cross(P2_a,X1_a) */
    delta = 0.0;
    point_data[0] = 0;
  } else {
    delta = -130.0 * P1_a_2[2];
    for (k = 0; k < 3; k++) {
      point_data[k] = uv0[k];
    }
  }

  /* ????? ?? ?????????? ??????, ????? P2 ????????? ?????? ??????????? ??? ? */
  /* ??? ?????????????? ?????????????? ???????  abs(dot(P2_a-P0_a,X1_a))>0.01 */
  /* ????? ????????? ?????? ?1=?0 ? (??????? ???????) ?1=X0; Z1=Y0; ??? ?1=?0; Z1=?0; */
  b_B = 0.0;
  for (k = 0; k < 3; k++) {
    b_B += P2_a[k] * P1_a_2[k];
  }

  Lambda = b_B + delta;

  /* you can use p2x_a*X1_a(1)+p2y_a*X1_a(2)+p2z_a*X1_a(3) instead of dot(P2_a,X1_a) */
  for (k = 0; k < 3; k++) {
    Xr_a[k] = P2_a[k] - (P1_a_2[k] * Lambda + (double)point_data[k]);
  }

  B = norm(Xr_a);
  for (k = 0; k < 3; k++) {
    Xr_a[k] /= B;
  }

  /* inverse transfer matrix  */
  /* T=[X_ Y_ Z_]^(-1);  */
  /* we can use transposition instead of inversing */
  /* because matrix T is orthogonal  */
  d_Xr_a[0] = Xr_a[1] * P1_a_2[2] - Xr_a[2] * P1_a_2[1];
  d_Xr_a[1] = Xr_a[2] * P1_a_2[0] - Xr_a[0] * P1_a_2[2];
  d_Xr_a[2] = Xr_a[0] * P1_a_2[1] - Xr_a[1] * P1_a_2[0];
  b_px_a[0] = px_a;
  b_px_a[1] = py_a;
  b_px_a[2] = 75.0;
  for (k = 0; k < 3; k++) {
    c_P1_a_2[3 * k] = P1_a_2[k];
    c_P1_a_2[1 + 3 * k] = d_Xr_a[k];
    c_P1_a_2[2 + 3 * k] = Xr_a[k];
    b_P2_a[k] = b_px_a[k] - P2_a[k];
  }

  for (k = 0; k < 3; k++) {
    Yr_a[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      Yr_a[k] += c_P1_a_2[k + 3 * i0] * b_P2_a[i0];
    }
  }

  /* |q4|<=pi/2 --> cos(q4)>=0; */
  /* we know that */
  /* cosq5==P2P_n(1)/d4 ==cosalpha1*cosbeta1; (1) */
  cosq5 = Yr_a[0] / 140.0;

  /* <-(1) */
  flag = true;
  if (fabs(Yr_a[0] - 140.0) < 1.0E-6) {
    /* first exclusion */
    flag = false;
  } else {
    if (fabs(Yr_a[0] + 140.0) < 1.0E-6) {
      /* second exclusion */
      flag = false;
    }
  }

  if ((fabs(Yr_a[2]) < 1.0E-6) && flag) {
    /* third exclusion */
    /* we know that */
    /* cosq4==0; sinq4==+-pi/2 */
    /* cosq5==P2P_n(1)/d4  */
    /* sinq5*sinq4==P2P_n(2)/d4     */
    /* additionally, 0<=q5<=pi now, that means sinq5>=0 */
    sinq5 = sqrt(1.0 - cosq5 * cosq5);
    sinq4 = Yr_a[1] / (140.0 * sinq5);
  } else {
    if (flag) {
      /* we know that */
      /* sinq5*sinq4==P2P_n(2)/d4 ==sinalpha1*cosbeta1; (2) */
      /* sinq5*cosq4==P2P_n(3)/d4 == sinbeta1; (3) */
      /* cos(q4)>=0; (4) */
      cosq4 = fabs(Yr_a[2] / (140.0 * sqrt(1.0 - cosq5 * cosq5)));

      /* <-(3) & (4) */
      /* or use sin(acos(cosq5)) instead of sqrt(1-cosq5^2) */
      sinq5 = Yr_a[2] / (140.0 * cosq4);

      /* <-(3) */
      sinq4 = Yr_a[1] / (140.0 * sinq5);

      /* <-(2)    */
    }
  }

  if (sinq4 > 0.0) {
    *q4 = acos(cosq4);
  } else {
    *q4 = -acos(cosq4);
  }

  if (sinq5 > 0.0) {
    *q5 = acos(cosq5);
  } else {
    *q5 = -acos(cosq5);
  }

  /* you can use q4=acos(cosq4)*sign(sinq4) and q5=acos(cosq5)*sign(sinq5) instead */
  /* you can use q6=acos(cosq6)*sign(sinq6) instead */
}

/*
 * File trailer for kinem2_new5.c
 *
 * [EOF]
 */
