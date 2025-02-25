#include "dspline.h"
#include "dsp_qr.h"

#ifndef __MY_DSP_ABIC__
#define __MY_DSP_ABIC__

void r_search2(dspline *dsp);
void NOT_GOLDEN(dspline *dsp);
double dspline_abic_search2(dspline *, int);
double get_abic2(dspline *, double, double, int);

void dspline_abic_search1(dspline *);
double get_abic1(dspline *, double);

int abic_golden(double *, double *, double *, double *);

#endif
