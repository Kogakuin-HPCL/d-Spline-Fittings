#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dsp_set.h"
#include "dsp_abic.h"
#include "dsp_ind.h"
#include "dspline.h" //dspの構造体
#include "idou.h"
#include "../include/dspline_fitting.h"

int dsp_init(dspline *dsp, int n, double alpha){

	int tmp,i;
	dsp->itvl = E;
	dsp->Z = NULL;
	
	dspline_setdims(dsp, 1); //dsp_set.c [dsp->dim,nd,n,alpha,d_ind]を0~1でマロック、領域確保

	dsp->alpha[0] = alpha;	//alpha=0を入れる
	dsp->nd[0] = n; //nd = N
	dsp->dd = n; //dd = N
	dsp->n[1] = 3;//基本標本店の数 3→1□□2□□3 4→1□□□2□□□3

	for(i=0;i<n-1;i++){
		dsp->h_ten[i] = (int)(dsp->h[0][i] / dsp->minh) * (dsp->n[1]-1);//最小補間点数 //切り捨て
		//1101 printf("H = %f hokan_ten = %d\n",dsp->h[0][i],dsp->h_ten[i]);
		dsp->h_ten[n] += dsp->h_ten[i];//補間点の数値を集めている
	}

	//1101 printf("dsp->h_ten[n]=%d\n",dsp->h_ten[n]);
	dsp->n[0] = dsp->h_ten[n] + n;//n = N
	//1101 printf("dsp->n[0]=%d\n",dsp->n[0]);
	
	dsp->nn = dsp->n[0]; //nn = 
	dspline_setvecs(dsp); //dsp_set.c
	dspline_set_ytof(dsp); //dsp_ind.c POS ytof[] → 実データがどこに入っているか

}

void dsp_hutou_init(dspline *dsp, int n){	//minhを求める関数

	int i, j, k;
	int count = 0;

	dsp->minh = INFINITY; //初期値
	
	for(i=0;i<n-1;i++){
		dsp->h[0][i] = fabs(dsp->x[0][i+1] - dsp->x[0][i]);//h[0][i]に元データ間の距離を代入
		if(dsp->minh > dsp->h[0][i]){dsp->minh = dsp->h[0][i];}
		//1101 printf("H = %f\n",dsp->h[0][i]);
	}

	//dsp->minh = dsp->minh * 3;
	//1101 printf("min H = %f\n",dsp->minh);//実データの最小距離
}


dspline *ddspline(double *x, double *y, int len, int flag){
    int i, j=0, k, n=0, nn, hy, hh;	//nはNの数
	int index = 0;
	int NUM;		//モデルの変更の数字　1 or 2
	double tmp = 0;
	double alpha = 0.0;
	double minh;


	NUM = flag;	//	モデルの変更の数字　1 or 2 を NUM 入れてる
	n = len;

	dspline *dsp; //dspine.h 構造体
	dsp = (dspline*)malloc(sizeof(dspline));//構造体をdspにmalloc
	
	dsp->x = (double**)malloc(sizeof(double *));
	dsp->h = (double**)malloc(sizeof(double *));
	dsp->y = (double*)calloc(n, sizeof(double));

	dsp->x[0] = (double*)malloc(sizeof(double) * 1000);
	dsp->h[0] = (double*)malloc(sizeof(double) * n);
	dsp->h_ten = (int*)malloc(sizeof(double) * n);

    for(i = 0; i < n; i++) {
        dsp->x[0][i] = x[i];
        dsp->y[i] = y[i];
    }

	dsp_hutou_init(dsp,n);	//minhを求める関数
	dsp_init(dsp, n, alpha); //上部イニシャライズ

	//---端っこ5％づつは二相になってないのでは。rの計算量短縮---
	dsp->N_5per = dsp->nn*0.05;// n 実データ数(36) * 0.05 = 1.8
	//基本は5％だが、場合によっては10％ぐらいにしないと引っかかるかも？
	if(dsp->N_5per<5){
		dsp->N_5per = 5;
	}
	//dsp->N_5per = 0;//全体探索 計測用
	//1101 printf("dsp->N_5per = %d\n",dsp->N_5per);
	//------------------------------------------
	
	minh = dsp->minh;//最小のH 0.25
	nn = dsp->nn; //補間後の合計点数 111
	hy = dsp->n[1];//標本点の数 3
	//1101 printf("nn = %d\n",dsp->nn);

	dsp->x[1] = (double*)malloc(sizeof(double) * nn);
	dsp->x[2] = (double*)malloc(sizeof(double) * (nn+2));//両端の0部分+2
	dsp->tx = (double*)malloc(sizeof(double) * nn);
	dsp->ty = (double*)malloc(sizeof(double) * nn);
	dsp->fx = (double*)malloc(sizeof(double) * nn);
	dsp->fy = (double*)malloc(sizeof(double) * nn);
	dsp->h[1] = (double*)malloc(sizeof(double) * nn);
	dsp->h[2] = (double*)malloc(sizeof(double) * nn);
	dsp->h[3] = (double*)malloc(sizeof(double) * (nn-1));

	//-------------------makeX＆実データがどこに入っているか。ytofのフラグを更新-----------------------
	//ytof、本当はind.cに実装したかったけどできんかった。

	tyu_x_za(index,nn,n,dsp);

	//------------------------------------------------
	
	//-------------------make h-----------------------

	for(i=0;i<nn-1;i++){	//i=0~109
		dsp->h[3][i] = dsp->x[1][i+1] - dsp->x[1][i];	//111点あるx座標同士の差をh[3][0~109]に格納
		//1101 printf("h[3][i] = %f\n",dsp->h[3][i]);
	}
	//1101 printf("\n");

	for(i=0;i<nn;i++){
		dsp->h[1][i] = fabs(dsp->x[2][i] - dsp->x[2][i+1]);
		dsp->h[2][i] = fabs(dsp->x[2][i+1] - dsp->x[2][i+2]);
		if(i==0){dsp->h[1][i] = fabs(dsp->x[2][i+1] - dsp->x[2][i+2]); dsp->h[2][i] = 0;}
		if(i==nn-1){dsp->h[2][i] = fabs(dsp->x[2][i] - dsp->x[2][i+1]); dsp->h[1][i] = 0;}
		//1101 printf("\nh1 = %f h2= %f\n",dsp->h[1][i],dsp->h[2][i]);
	}
	//------------------------------------------------
	if(NUM == 0){
		dspline_abic_search1(dsp);//一層問題 dsp_abic.c
		//NOT_GOLDEN(dsp);//ABIC全体探索 計測用
	}
	else if(NUM == 1){
		r_search2(dsp);//二相問題
	}
	//1101 printf("\n");
	dspline_backward(dsp);

	for (i = 0; i<dsp->nn; i++) {//test
		//1101 printf("%f\n",dsp->f[i]);
	}

	//ここからグラフ作成
	/*FILE *dfp = fopen("kansuu.txt","w");
	if ( dfp == NULL ) {
		printf("Error opening file!\n");
	}

	for (double x = 0; x <= 10; x += 0.1) { 
		double y; 
		if (x < 5) { 
			y = 0.5 * x + 1.5; 
		} 
		else { 
			y = (x - 7) * (x - 7); 
		} 
		fprintf(dfp, "%lf %lf\n", x, y); 
	}

	fclose(dfp);*/


	/*for(i=0; i<nn; i++){
		printf("%f,%f\n",dsp->x[1][i],dsp->f[i]);
	}

	printf("\n");

	for(i=0; i<n; i++){
		printf("%f,%f\n",dsp->x[0][i],dsp->y[i]);
	}*/

	/*for(i=0; i<nn; i++){
		printf("%f\n",dsp->ty[i]);
	}*/

	//write_csv("default.csv", dsp);
	//write_csv2("fitting.csv", dsp);

	FILE *fp1 = fopen("default.csv", "w");
	if (fp1 == NULL){
		printf("can't open file\n");
		return NULL;
	}

	for (i = 0; i < dsp->dd; i++) {
		fprintf(fp1, "%f %f\n", dsp->x[0][i], dsp->y[i]); 
	}

	fclose(fp1);

	FILE *fp2 = fopen("fitting.csv", "w");
	if (fp2 == NULL){
		printf("can't open file\n");
		return NULL;
	}

	for (i = 0; i < dsp->nn; i++) {
		fprintf(fp2, "%f %f\n", dsp->x[1][i], dsp->f[i]); 
	}

	fclose(fp2);

	for (i = 0; i < dsp->nn; i++) {
		dsp->fx[i] = dsp->x[1][i];
		dsp->fy[i] = dsp->f[i];
	}

	//output_graph("default.csv","fitting.csv");

//void output_graph(char *filename1, char *filename2){	
	FILE *gp;
		gp = popen("gnuplot -persist", "w" );
		//fprintf(gp, "set datafile separator ','\n");
		fprintf(gp, "set terminal png\n");
		fprintf(gp, "set output 'result.png'\n");
		//fprintf(gp, "set xrange[0:0.12]\n");
		//fprintf(gp, "set yrange[2000:20000]\n");
		//fprintf(gp, "set xrange[1:12]\n");
		//fprintf(gp, "set xlabel 'Month'\n");
		//fprintf(gp, "set ylabel 'Temperature'\n");
		// fprintf(gp, "set yrange[-2:10]\n");
		//fprintf(gp, "plot 'default.csv' using 1:2 with points title 'Observation Points'\n");
		
		//fprintf(gp, "plot 'default.csv' using 1:2 with points title 'Observation Points', 'fitting.csv' using 1:2 with lines lw 2 lc rgb 'black' title 'd-Spline', 'data.txt' using 1:2 with points lc rgb 'red' pt 7 ps 0.2 title 'trueline points'\n");
		fprintf(gp, "plot 'default.csv' using 1:2 with points title 'Observation Points', 'fitting.csv' using 1:2 with lines lw 2 lc rgb 'black' title 'd-Spline'\n");
		//fprintf(gp, "plot 'fitting.csv' using 1:2 with lines lw 2 lc rgb 'black' title '2024', \'2000.csv' using 1:2 with lines lw 2 lc rgb 'red' title '2000', \'1980.csv' using 1:2 with lines lw 2 lc rgb 'blue' title '1980', \'1960.csv' using 1:2 with lines lw 2 lc rgb 'green' title '1960'\n");

		
		//fprintf(gp, "exit\n");
	pclose(gp);
//}
	return dsp;
}
