#include <stdio.h>
#include <stdlib.h>
#include "dspline.h"

void dspline_set_d_ind(dspline *dsp){

	int i,j;
	int *nd,*n;
	int tmp;

	nd = dsp->nd;
	n = dsp->n;

	switch(dsp->itvl){
		case E:
			for(i=0;i<dsp->dim;i++){
				for(j=0;j<nd[i];j++){
					tmp = (int) ( j * (((n[i]) - 1.0) / (nd[i] - 1.0)));
					dsp->d_ind[i][j] = tmp;
					//1101 printf("d_ind[%d][%d] = %d\n",i,j,dsp->d_ind[i][j]);
				}
			}
			break;

		case X:
			for(i=0;i<dsp->dim;i++){
				for(j=0;j<nd[i];j++){
					tmp = (int) (dsp->x[j][i]-dsp->x[j][0]) / (dsp->x[j][nd[i]-1]-dsp->x[j][0]) * (n[j] - 1);
					dsp->d_ind[i][j] = tmp;
				}
			}
	}
}

void dspline_set_ytof(dspline *dsp){
	
	int i,j;

	int *nd,*n;
	int dd,nn;

	nd = dsp->nd; //0 ndの先頭ポインタ nd[0] = 31
	n = dsp->n; //0 nの先頭ポインタ 
	dd = dsp->dd; //31
	nn = dsp->nn;//

	int tmp,tmptmp,tmptmptmp,coo;

	dspline_set_d_ind(dsp);//直上

	for(i=0;i<dd;i++){
		tmp = i;
		tmptmp = dd;//31
		tmptmptmp = nn;//111

		for(j=dsp->dim-1;j>=0;j--){
			tmptmp /= nd[j];	//1
			tmptmptmp /= n[j];	//1
			coo = tmp / tmptmp;
			tmp = tmp % tmptmp;
			//1101 printf("%d %d\n",tmp,coo);
			dsp->ytof[i] += dsp->d_ind[j][coo] * tmptmptmp;
			//1101 printf("dsp->ytof[i] = %d\n",dsp->ytof[i]);
		}
	}
}