#include "../src/dspline.h"

#ifndef __DSPLINE_FITTING_H__
#define __DSPLINE_FITTING_H__

int dsp_init(dspline *,int,double);
void dsp_hutou_init(dspline *,int);
//void output_graph(char *,char *);
void write_csv(char *,dspline *);
void write_csv2(char *,dspline *);
dspline *ddspline(double *,double *,int,int);

#endif