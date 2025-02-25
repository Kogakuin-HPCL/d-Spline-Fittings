#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dspline.h"
#include "dsp_qr.h"

static const double golden = (1.0+2.2360679775)/2.0;

// get R, Q^t * b, and ABIC value

double get_abic1(dspline *dsp, double alpha){

	int i;

	double fst = 0.0;
	double trd = 0.0;
	double snd = 0.0;

	double tmp;

	int nn,dd; 
	int range; //range点づつ更新

	nn = dsp->nn;// 111
	dd = dsp->dd;// 31

	range = dsp->nn / dsp->n[dsp->dim-1];
	//1101 printf("range = %d nn = %d n[dsp->dim-1] = %d\n",range,nn,dsp->n[dsp->dim-1]);

	dsp->alpha[0] = pow(10.0, alpha);

	for(i=0;i<nn;i++) {
		dsp->b[i] = 0.0;
	}

	if(dsp->Z != NULL){
		free(dsp->Z->val);
		free(dsp->Z);
		dsp->Z = NULL;
	}

	dspline_qr_init1(dsp);//Zの値入れ、初期化等

	for(i=0;i<dd;i++){
		tmp = dspline_qr_incr1(dsp,i,dsp->y[i]);
		trd += tmp * tmp;
		//printf("trd=%f\n", trd);
	}

	for(i=0;i<nn;i++){
		fst += log(fabs(dsp->Z->val[i*(TRI_RANGE)]));//TRI_RANGE = (2 * range + 1) range = 1;
	}
	fst *= 2.0;
	
	for(i=1;i<nn-2;i++){
		snd += log(dsp->h[3][i]);
	}
	//printf("ALL fst=%f\n", fst);
	//printf("ALL trd=%f\n\n", trd);
	
	//return(fst - 2 * (nn - 2) * log(dsp->alpha[0]) + (dd - 2) * log(trd));//dd =n nn = nn 二相用は式が間違っていた！！！！！！！！！下が正しい
	return(fst - 2 * (nn - 2) * log(dsp->alpha[0]) + (dd - 2) * log(trd) - 2 * snd);//dd =n nn = nn
}

double get_abic2(dspline *dsp, double alpha, double beta, int r){

	int i;

	double fst = 0.0;
	double trd = 0.0;
	double snd = 0.0;

	double abic;
	double tmp;

	int nn,dd;
	int range;

	nn = dsp->nn;
	dd = dsp->dd;
	range = dsp->nn / dsp->n[dsp->dim-1];
	//printf("range = %d nn = %d n[dsp->dim-1] = %d\n",range,nn,dsp->n[dsp->dim-1]);

	dsp->alpha[0] = pow(10.0, alpha);
	dsp->beta = pow(10.0, beta);
	dsp->gunman = 1.e-4;
	dsp->r = r;

	for(i=0;i<nn;i++){
		dsp->b[i] = 0.0;
	}

	if(dsp->Z != NULL){
		free(dsp->Z->val);
		free(dsp->Z);
		dsp->Z = NULL;
	}

	dspline_qr_init2(dsp);

	for(i=0;i<dd;i++){
			tmp = dspline_qr_incr2(dsp,i,dsp->y[i]);
			trd += tmp * tmp;
	}

	for(i=0;i<nn;i++){
		fst += log(fabs(dsp->Z->val[i*(TRI_RANGE)]));
	}
	fst *= 2.0;

	for(i=1;i<nn-2;i++){
		snd += log(dsp->h[3][i]);
	}

	//abic = fst -2*(r-1)*log(dsp->alpha[0]) -2*log(1.e-4) -2*(nn-2-r)*log(dsp->beta) + (dd-2)*log(trd);
	abic = fst - 2 * (r - 1) * log(dsp->alpha[0]) -2 *log(1.e-4) -2*(nn-2-r)*log(dsp->beta) + (dd-2)*log(trd) - 2 * snd;

	//1029ここにありましたprintf("alpha,%lf,beta,%lf,r,%d,ABIC,%lf\n", pow(10.0, alpha), pow(10.0, beta), r, abic);//計測用
	
	return(abic);
}


double dspline_abic_search2(dspline *dsp, int r){

	int i,j;
	double a[20];

	double abic;
	double min = 1.e+5;
	double a_opt;
	double b_opt;
/*
	a[0] = -2.5;
	for(i=0;i<50;i++){
		a[i] = a[0] + 0.1 * i;
	}*/

	//a[]アルファとβの組み合わせ 19×19通り調べてる
	a[0]=-5;
	a[1]=-4.5;
	a[2]=-4;
	a[3]=-3.5;
	a[4]=-3;
	a[5]=-2;
	a[6]=-1.5;
	a[7]=-1;
	a[8]=-0.5;
	a[9]=0;
	a[10]=0.5;
	a[11]=1;
	a[12]=1.5;
	a[13]=2;
	a[14]=2.5;
	a[15]=3;
	a[16]=3.5;
	a[17]=4;
	a[18]=4.5;
	a[19]=5;

	for(i=0;i<=19;i++){
		//puts("");
		for(j=0;j<=19;j++){
			abic = get_abic2(dsp, a[i], a[j], r);
			if(min > abic){
				min = abic;
				a_opt = a[i];
				b_opt = a[j];
			}
		}
	}
	//printf("end\n");


	min = get_abic2(dsp, a_opt, b_opt, r);

	return(min);
}

void r_search2(dspline *dsp){

	double *abic;
	double min = 1.e+5;//10^5
	int r, r_opt;

	abic = malloc(sizeof(double) * dsp->nn);//補間した数＋実データ数	
	//printf("ABIC\n");
	for(r=dsp->N_5per; r < dsp->nn - dsp->N_5per; r++){ //dsp->N_5per = 5 r 5<全データ数-5 +1づつ
		abic[r] = dspline_abic_search2(dsp, r);//Rのポイントを探すために全て回す。
		//printf("r,%d,ABIC,%lf\n", r, abic[r]);
	}

	for(r=dsp->N_5per;r<dsp->nn-dsp->N_5per;r++){
		if(min > abic[r]){
			min = abic[r];
			r_opt = r;
		}
	}
	
	printf("Turning Point R = %d\n abic = %f\n", r_opt,abic[r_opt]);

	dspline_abic_search2(dsp, r_opt);//決まったRで再度回し、最適なαβを見つける

	free(abic);
}

void NOT_GOLDEN(dspline *dsp){

	double *abic;
	double min = 1.e+5;//10^5
	double a,ABIC;
	double opt_a = 0.0;

	//1029ここにありましたprintf("ABIC\n");
	for(a= -5; a <= 5; a+=0.001){ //dsp->N_5per = 5 r 5<全データ数-5 +1づつ
		ABIC = get_abic1(dsp, a);
		//1029ここにありましたprintf("alpha,%lf,ABIC,%lf\n", pow(10.0, a), ABIC);
		if(min > ABIC){
			min = ABIC;
			opt_a = a;
		}
	}

	//1029ここにありましたprintf("best alpha = %lf\n", pow(10.0, opt_a));


	get_abic1(dsp, opt_a);//決まったRで再度回し、最適なαβを見つける

	printf("alpha = %f",opt_a);

	free(abic);

}

// golden section search
// return next searching point
int abic_golden(double *key_ext, double *key_int, double *val_ext, double *val_int){
	int flag;
	flag = val_int[0]>val_int[1]?0:1;

	key_ext[flag] = key_int[flag];
	val_ext[flag] = val_int[flag];

	key_int[flag] = key_int[!flag];
	val_int[flag] = val_int[!flag];

	key_int[!flag] = (1.0*flag+golden*!flag)/(1.0+golden)*fabs(key_ext[1]-key_ext[0])+key_ext[0];

	return(!flag);
}

// ABIC golden section search
void dspline_abic_search1(dspline *dsp){

	int flag = 0;
	//const double golden=(1.0+sqrt(5.0))/2.0;
	double itv=1.0;

	double key_ext[2];
	double key_int[2];
	double val_ext[2];
	double val_int[2];

	key_ext[0] = -5.0;
	key_ext[1] = 5.0;

	key_int[0] = 1.0/(1.0+golden)*fabs(key_ext[1]-key_ext[0])+key_ext[0];
	key_int[1] = golden/(1.0+golden)*fabs(key_ext[1]-key_ext[0])+key_ext[0];

	val_ext[0] = get_abic1(dsp,key_ext[0]);
	val_int[0] = get_abic1(dsp,key_int[0]);
	val_int[1] = get_abic1(dsp,key_int[1]);
	val_ext[1] = get_abic1(dsp,key_ext[1]);

	printf("alpha=%lf ABIC=%lf\n", pow(10.0, key_ext[0]), val_ext[0]);
	printf("alpha=%lf ABIC=%lf\n", pow(10.0, key_int[0]), val_int[0]);
	printf("alpha=%lf ABIC=%lf\n", pow(10.0, key_int[1]), val_int[1]);
	printf("alpha=%lf ABIC=%lf\n\n", pow(10.0, key_ext[1]), val_ext[1]); 
	
	while(itv>0.0001){//今までは0.01だった
		flag = abic_golden(key_ext, key_int, val_ext, val_int);
		val_int[flag] = get_abic1(dsp, key_int[flag]);
		itv = fabs((key_int[1]-key_int[0])/key_int[0]);
		printf("itv = %f", itv);
		printf("key_int = %f alpha=%lf ABIC=%lf\n",key_int[flag], pow(10.0,key_int[flag]), val_int[flag]);
	}
	/*key_int[0] = -1;
	val_int[flag] = get_abic1(dsp, key_int[0]);
	printf("key_int = %f alpha=%lf ABIC=%lf\n",key_int[0], pow(10.0,key_int[0]), val_int[0]);
*/
}



