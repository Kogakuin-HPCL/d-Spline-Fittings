#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dspline_fitting.h"
//時間計測用ライブラリ
#include <time.h>
//

enum option{
	option1,
	option2,
};

int get_num(char *name);
void open_file(double *x, double *y, char *name_x, char *name_y, int n);
void open_file2(double *x, double *y, char *name, int n);
int compare(const void *a, const void *b);

typedef struct {
	double x;
	double y;
} Data;

int main(int argc,char **argv){

	int n;
	double *x , *y ;
	enum option op1, op2;
	op1 = option1;
	op2 = option2;

	if (argc < 2) {
		printf("Please enter data file path.\n");
	}
	else if (argc == 2) {
		char data[256];

		strcpy(data, argv[1]);
		n = get_num(data);
		x = (double *)malloc(sizeof(double) * n );
		y = (double *)malloc(sizeof(double) * n );
		open_file2(x, y, data, n);
		dspline *result = ddspline(x,y,n,op1);

		free(x);
		free(y);
	}
	else if (argv[3] == NULL) {

		if ((strcmp(argv[2],"option1") != 0 && strcmp(argv[2],"option2") != 0) && (fopen(argv[2], "r"))) {

			// x配列とy配列が別々のファイルだがオプション指定がない場合

			char data_x[256];	//	ファイル名を入れるための箱
			char data_y[256];

			strcpy(data_x,argv[1]);	//	argv[1]に入ってるファイル名をdata_xにコピーしている
			strcpy(data_y,argv[2]);	
			n = get_num(data_x);
			x = (double *)malloc(sizeof(double) * n );
			y = (double *)malloc(sizeof(double) * n );
			open_file(x , y, data_x, data_y, n);
			dspline *result = ddspline(x,y,n,op1);

			free(x);
			free(y);
		}
		else {

			char data[256];

			strcpy(data, argv[1]);
			n = get_num(data);
			x = (double *)malloc(sizeof(double) * n );
			y = (double *)malloc(sizeof(double) * n );
			open_file2(x, y, data, n);
			if (strcmp(argv[2],"option1") == 0){
				dspline *result = ddspline(x,y,n,op1);
			}
			else if(strcmp(argv[2],"option2") == 0){
				dspline *result = ddspline(x,y,n,op2);
			}
			else {
				printf("The enable option is either 'option1' or 'option2'.\n");
			}

			free(x);
			free(y);
		}
	}
	else {
		char data_x[256];	//	ファイル名を入れるための箱
		char data_y[256];

		strcpy(data_x,argv[1]);	//	argv[1]に入ってるファイル名をdata_xにコピーしている
		strcpy(data_y,argv[2]);	
		n = get_num(data_x);
		x = (double *)malloc(sizeof(double) * n );
		y = (double *)malloc(sizeof(double) * n );
		open_file(x , y, data_x, data_y, n);

		if (strcmp(argv[3],"option1") == 0){
			dspline *result = ddspline(x,y,n,op1);
		}
		else if(strcmp(argv[3],"option2") == 0){
			dspline *result = ddspline(x,y,n,op2);
		}
		else {
			printf("The enable option is either 'option1' or 'option2'.\n");
		}

		free(x);
		free(y);
	}

	printf("tame taken = %f [s]\n", ((double)clock()) / CLOCKS_PER_SEC); //時間計測

	return 0;
}

int get_num(char *name) {
	FILE *fp;
    int num = 0;
    char ret[256];
	
	fp = fopen(name , "r");
	if (fp == NULL){
		printf("ERROR! fp=NULL!\n");
		return 1;
	}

	while(fgets(ret, sizeof(ret), fp) != NULL){
		num++;
	}

	fclose (fp);

	//end_time = time(NULL);

	//printf("time:%ld\n",end_time - start_time);
	return num;
}

void open_file(double *x, double *y, char *name_x, char *name_y, int n){
	FILE *fp_x, *fp_y;
	char *token;
	char line[256];

	Data *data = (Data*)malloc(n * sizeof(Data));

	fp_x = fopen(name_x , "r");
	if (fp_x == NULL){
		printf("ERROR\n");
		free(data);
	}

	fp_y = fopen(name_y, "r");
	if (fp_y == NULL){
		printf("ERROR\n");
		free(data);
		fclose(fp_x);
	}

	for (int i =  0; i < n; i++) {
		if (fgets( line, sizeof(line), fp_x) != NULL) {
			token = strtok( line, " ,");
			if (token != NULL){
				data[i].x = atof(token);
			}
		}
		else break;
	}

	for (int i =  0; i < n; i++) {
		if (fgets( line, sizeof(line), fp_y) != NULL) {
			token = strtok( line, " ,");
			if (token != NULL){
				data[i].y = atof(token);
			}
		}
		else break;
	}


	fclose(fp_x);
	fclose(fp_y);

	qsort(data, n, sizeof(Data), compare);

	for(int i = 0; i < n; i++){
		x[i] = data[i].x;
		y[i] = data[i].y;
	}

	free(data);
}

void open_file2(double *x, double *y, char *name, int n){
	FILE *fp;
	char *token;
	char line[256];

	Data *data = (Data*)malloc(n * sizeof(Data));

	fp = fopen(name , "r");
	if (fp == NULL){
		printf("ERROR\n");
		free(data);
	}

	for (int i =  0; i < n; i++) {
		if (fgets( line, sizeof(line), fp) != NULL) {
			token = strtok( line, " ,");
			if (token != NULL){
				data[i].x = atof( token);
			}
			token = strtok( NULL, " ,");
			if (token != NULL) {
				data[i].y = atof( token);
			}
		}
		else break;
	}

	fclose(fp);

	qsort(data, n, sizeof(Data), compare);

	for(int i = 0; i < n; i++){
		x[i] = data[i].x;
		y[i] = data[i].y;
	}

	free(data);
}

int compare(const void *a, const void *b){
	double diff = ((Data*)a)->x - ((Data*)b)->x;
	return (diff > 0) - (diff < 0);
}

