#include "crs.h"

#ifndef __MY_DSPLINE__
#define __MY_DSPLINE__

typedef enum{//型
	E,
	X
}interval_strategy;

struct _dspline{
	int dim;
	int *n,*nd;
	int nn,dd;
	double minh;
	int *h_ten;

	int **d_ind;
	int *ytof;
	int *used;
	int N_5per;

	interval_strategy itvl;
	int ext;

	double alpha_base;
	double *alpha;
	double beta;
	double gunman;
	int r;

	crs *Z; //ZはCRS形式で作る crs.h
	double *f;
	double *b;
	double **x;
	double *tx;
	double **h;
	double *y;
	double *ty;
	double *fx;
	double *fy;
};

typedef struct _dspline dspline;

#endif
