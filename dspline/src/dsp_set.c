#include <stdlib.h>
#include "dsp_qr.h"

void dspline_setdims(dspline *dsp, int dim){	//次元数による領域確保

	dsp->dim = dim;	//dsp->dimに引数のdimを入れる
	dsp->nd = malloc(sizeof(int) * dim);
	dsp->n = malloc(sizeof(int) * dim);
	dsp->alpha = malloc(sizeof(double) * dim);
	dsp->d_ind = malloc(sizeof(int *) * dim);
}

void dspline_setvecs(dspline *dsp){
	int i;
	
	for(i=0;i<dsp->dim;i++){
		dsp->d_ind[i] = malloc(sizeof(int) * dsp->nd[i]);
	}
	dsp->ytof = calloc(dsp->dd, sizeof(int));
	dsp->used = calloc(dsp->dd, sizeof(int));
	dsp->f = calloc(dsp->nn, sizeof(double));
	dsp->b = calloc(dsp->nn, sizeof(double));
	//dsp->y = calloc(dsp->dd, sizeof(double));//カロック=0クリア
}

void dspline_destroy(dspline *dsp){

	int i;

	free(dsp->y);
	free(dsp->b);
	free(dsp->f);

	free(dsp->used);
	free(dsp->ytof);

	for(i=0;i<dsp->dim;i++){
		free(dsp->d_ind[i]);
	}
	free(dsp->d_ind);

	free(dsp->alpha);
	free(dsp->n);
	free(dsp->nd);
	free(dsp->h_ten);

}
