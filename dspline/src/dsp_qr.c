#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "crs.h"
#include "dspline.h"

#define TRI_RANGE (2 * range + 1)
//#define TRI_RANGE(n)  ((n) * range + 1)]

void givens(double c, double s, double *x, double *y, int range){
	
	int i;
	double xx,yy;

	for(i=0;i<range;i++){
		xx = x[i]*c + y[i]*s;
		yy =-x[i]*s + y[i]*c;

		x[i] = xx;
		y[i] = yy;
	}
}

void dspline_qr_init1(dspline *dsp){

	double *work;
	double *Z;
	double r,c,s;
	int i,j,k;

	int bnd[2];

	int coo,tmp,tmptmp;

	int *n;
	int nn;
	int range;
	int dim;

	int hoge;
	int jj;

	n = dsp->n;
	nn = dsp->nn;
	dim = dsp->dim;
	range = nn / n[dsp->dim-1];

	if(dsp->Z == NULL){
		dsp->Z = malloc(sizeof(crs));
		dsp->Z->val = (double *)calloc((dsp->nn*TRI_RANGE), sizeof(double));
	}

	Z = dsp->Z->val;

	work=malloc(sizeof(double)*(TRI_RANGE));
	//work=malloc(sizeof(double)*(nn));

	for(i=0;i<nn;i++){
		for(j=0;j<TRI_RANGE;j++){
			work[j] = 0.0;
		}

		//struct D_i
		tmp = i;
		tmptmp = nn;
		for(j=dim-1;j>=0;j--){

			tmptmp /= n[j];
			coo = tmp / tmptmp;
			tmp = tmp % tmptmp;

			work[range - tmptmp] += dsp->h[2][i] * dsp->alpha[j];
			work[range] -= (dsp->h[2][i] + dsp->h[1][i]) * dsp->alpha[j];
			work[range + tmptmp] += dsp->h[1][i] * dsp->alpha[j];
		}
		//1101 printf("%d %f %f %f | ", i+1,work[0], work[1], work[2]);
		//1101 printf(" X= %f %f %f\n",dsp->x[2][i],dsp->x[2][i+1],dsp->x[2][i+2]);

		//Givens
		for(j=-range;j<1;j++) {
			jj = j + range;
			if(work[jj]==0.0){/*printf("cont\n");*/continue;}
			//printf("%d %d %d %d\n",i,j,jj,TRI_RANGE-jj);
			c = Z[(i+j)*(TRI_RANGE)];
			s = work[jj];
			r = sqrt(c*c + s*s);
			c /= r;
			s /= r;
			//hoge = j + TRI_RANGE >= nn ? nn-j : TRI_RANGE ;
			givens(c,s,&Z[(i+j)*(TRI_RANGE)],&work[jj],TRI_RANGE-jj);
		}
		//printf("%f %f %f\n", Z[0], work[1], work[2]);

	}
	free(work);
}

double dspline_qr_incr1(dspline *dsp, int ind, double val){

	int i,j,k;
	double r,c,s;
	double *work;
	double tmp[2];
	double *Z;

	double res;
	int pos;

	int hoge;

	int *n;
	int nn;
	int range;

	n = dsp->n;
	nn = dsp->nn;
	Z = dsp->Z->val;
	range = nn / n[dsp->dim-1];

	res = val;
	pos = dsp->ytof[ind];

	work=malloc(sizeof(double)*nn);

	for(i=0;i<nn;i++){
		work[i] = 0.0;
	}
	work[pos] = 1.0;

	for(j=pos;j<nn;j++){
	
		if(work[j]==0.0){continue;}
		c = Z[j*(TRI_RANGE)];
		s = work[j];
		r = sqrt(c*c + s*s);
		c /= r;
		s /= r;
		hoge = j + TRI_RANGE >= nn ? nn-j : TRI_RANGE ;
		givens(c,s,&Z[j*(TRI_RANGE)],&work[j],hoge);

		tmp[0] = dsp->b[j];
		tmp[1] = res;

		dsp->b[j] = tmp[0]*c + tmp[1]*s;
		res = -tmp[0]*s + tmp[1]*c;
	}

	free(work);
	return(res);
}


void dspline_qr_init2(dspline *dsp)
{
	double *work;
	double *Z;
	double r,c,s;
	int i,j,k;

	int bnd[2];

	int coo,tmp,tmptmp;

	int *n;
	int nn;
	int range;
	int dim;

	int hoge;
	int jj;

	int dpoint; //

	dpoint = dsp->r;
	n = dsp->n;
	nn = dsp->nn;
	dim = dsp->dim;
	range = nn / n[dsp->dim-1];

	//dsp->Z = alloc_DNS(dsp->nn,TRI_RANGE);
	if(dsp->Z == NULL){
		dsp->Z = malloc(sizeof(crs));
		dsp->Z->val = (double *)calloc((dsp->nn*TRI_RANGE), sizeof(double));
	}
	
	Z = dsp->Z->val;

	work=malloc(sizeof(double)*(TRI_RANGE));
	//work=malloc(sizeof(double)*(nn));

	for(i=0;i<nn;i++){

		for(j=0;j<TRI_RANGE;j++){
			work[j] = 0.0;
		}

		tmp = i;
		tmptmp = nn;
		for(j=dim-1;j>=0;j--){
			
			tmptmp /= n[j];
			coo = tmp / tmptmp;
			tmp = tmp % tmptmp;

			if(i<dpoint){
			work[range - tmptmp] += dsp->h[2][i] * dsp->alpha[j];
			work[range] -= (dsp->h[2][i] + dsp->h[1][i]) * dsp->alpha[j];
			work[range + tmptmp] += dsp->h[1][i] * dsp->alpha[j];
			}

			if(i==dpoint){
			work[range - tmptmp] += dsp->h[2][i] * dsp->gunman;
			work[range] -= (dsp->h[2][i] + dsp->h[1][i]) * dsp->gunman;
			work[range + tmptmp] += dsp->h[1][i] * dsp->gunman;
			}

			if(i>dpoint){
			work[range - tmptmp] += dsp->h[2][i] * dsp->beta;
			work[range] -= (dsp->h[2][i] + dsp->h[1][i]) * dsp->beta;
			work[range + tmptmp] += dsp->h[1][i] * dsp->beta;
			}
			
		}

	//printf("%d %f %f %f\n ", i+1,work[0], work[1], work[2]);

		for(j=-range;j<1;j++){
			jj = j + range;
			if(work[jj]==0.0){/*printf("cont\n");*/continue;}
			//printf("%d %d %d %d\n",i,j,jj,TRI_RANGE-jj);
			c = Z[(i+j)*(TRI_RANGE)];
			s = work[jj];
			r = sqrt(c*c + s*s);
			c /= r;
			s /= r;
			//hoge = j + TRI_RANGE >= nn ? nn-j : TRI_RANGE ;
			givens(c,s,&Z[(i+j)*(TRI_RANGE)],&work[jj],TRI_RANGE-jj);
		}
	}
	free(work);
}

double dspline_qr_incr2(dspline *dsp, int ind, double val)
{
	int i,j,k;
	double r,c,s;
	double *work;
	double tmp[2];
	double *Z;

	double res;
	int pos;

	int hoge;

	int *n;
	int nn;
	int range;
	n = dsp->n;
	nn = dsp->nn;
	Z = dsp->Z->val;
	range = nn / n[dsp->dim-1];
	
	//res=dsp->y[ind];
	res = val;
	pos = dsp->ytof[ind];

	work=malloc(sizeof(double)*nn);

	for(i=0;i<nn;i++){
		work[i] = 0.0;
	}
	work[pos] = 1.0;

	for(j=pos;j<nn;j++){
		
		if(work[j]==0.0){continue;}
		c = Z[j*(TRI_RANGE)];
		s = work[j];
		r = sqrt(c*c + s*s);
		c /= r;
		s /= r;
		hoge = j + TRI_RANGE >= nn ? nn-j : TRI_RANGE ;
		givens(c,s,&Z[j*(TRI_RANGE)],&work[j],hoge);

		tmp[0] = dsp->b[j];
		tmp[1] = res;

		dsp->b[j] = tmp[0]*c + tmp[1]*s;
		res = -tmp[0]*s + tmp[1]*c;
	}
	//res=res;
	free(work);
	return(res);
}



void dspline_backward(dspline *dsp){

	int i,j; 

	double *Z,*f,*b;
	int *n;

	int js;
	int nn;
	int range;

	int je;

	double tmp;

	Z = dsp->Z->val;
	f = dsp->f;
	b = dsp->b;
	n = dsp->n;
	nn = dsp->nn;

	range = 2 * nn / n[dsp->dim-1] + 1;
	//1101 printf("range = %d\n",range);
	for(int oo=0;oo<nn;oo++){
		//1101 printf("b[%d] = %f\n",oo,b[oo]);
	}

	for(i=nn-1;i>=0;i--){
		tmp = b[i];
		//j = nn-1;
		//while(j>i)
		je = i + range > nn ? nn - i : range;
		//js = i+1;
		for(j=1;j<je;j++){
			tmp -= Z[i*range+j] * f[i+j];
		}
		f[i] = tmp / Z[i*range];
	}
}

