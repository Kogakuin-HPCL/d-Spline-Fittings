#include "dspline.h"

#ifndef __MY_DSP_QR__
#define __MY_DSP_QR__

#define TRI_RANGE (2 * range + 1)

void dspline_qr_init1(dspline *);
double dspline_qr_incr1(dspline *, int, double);
void dspline_qr_init2(dspline *);
double dspline_qr_incr2(dspline *, int, double);
void dspline_backward(dspline *);

#endif
