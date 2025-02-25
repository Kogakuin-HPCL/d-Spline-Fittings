#include <stdio.h>
#include <stdlib.h>

#include "dspline.h"

void tyu_x_za(int index,int nn,int n,dspline *dsp){
    int i,j;
    for(i=0;i<n;i++){
		dsp->ytof[i] = index;
        printf("%d\n",dsp->ytof[i]);
		dsp->x[1][dsp->ytof[i]] = dsp->x[0][i];
		printf("dsp->x[1][%d] = %f\n",dsp->ytof[i],dsp->x[0][i]);
        dsp->tx[dsp->ytof[i]] = dsp->x[0][i];
        dsp->ty[dsp->ytof[i]] = dsp->y[i];
		index += dsp->h_ten[i]+1;
		//1101 printf("i=%d,n=%d,index=%d\n",i,n,index);
			if(i!=n){
				for(j=1;j <= dsp->h_ten[i]; j++){
					dsp->x[1][dsp->ytof[i]+j] = (dsp->x[0][i]*(dsp->h_ten[i]-j+1) + dsp->x[0][i+1]*j)/(dsp->h_ten[i]+1);
					//1101 printf("i=%d , j=%d , dsp->x[1][%d] = %f\n", i,j,dsp->ytof[i]+j,dsp->x[1][dsp->ytof[i]+j]);
				}
			}
		//1101 printf("NEW ytof[%d] = %d\n",i,dsp->ytof[i]);
	}
	//1101 printf("\n");

	for(i=0;i<nn+2;i++){	//nn=111 i=0~112
		dsp->x[2][i] = 0.0;	//dsp->x[2][0~112] = 0
	}
	for(i=0;i<nn;i++){		//i=0~110
		dsp->x[2][i+1] = dsp->x[1][i];	//dsp->x[2][1~111] = dsp->x[1][0~110]
		//1101 printf("x[1][%d] = %f\n",i,dsp->x[1][i]);	//x軸座標111点
	}
	for(i=0;i<nn+2;i++){
		//1101 printf("x[2][%d] = %f\n",i,dsp->x[2][i]);
	}
}