/*
 * cpsdfunc.c
 *
 *  Created on: Nov 2, 2016
 *      Author: Mindy
 */

#include "dcs.h"
#include "drealft.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <sdmalloc.h>
#include "f2c.h"
//#include <FreeRTOS.h>
//#include <task.h>

#include "Arduino.h"


// +++ header file
//#include <io.h>


static int WINDOW_OPTION = 2;
static float overlap = 0.5;

void forTest() {
	int a = 1;
	a = 1 + a;
	return;
}

void calculate_cpsd(double *input, double *APS, int NFFT, double FS, int length)
{
	double *sum, **adcs, **ACC1, **ACC2;
	int M = (int)(((float)length/NFFT)/overlap)-1;
	int a = NFFT* overlap;
	int i, j, k;
	double mean;
	
	ACC1 = (double**)malloc(M * sizeof (double*));
	ACC2 = (double**)malloc(M * sizeof (double*));
	adcs = (double**)malloc(M * sizeof (double*));
	sum = (double*)malloc(NFFT * sizeof (double));
	//ASSERT(ACC1 && ACC2 && adcs && sum);
	//memset(ACC1, 0, M* sizeof (double));
	//memset(ACC2, 0, M* sizeof (double));
	//memset(adcs, 0, M* sizeof (double));
		
	for (k = 0; k < M; k++) {
		ACC1[k] = (double*)malloc(NFFT * sizeof (double));
		ACC2[k] = (double*)malloc(NFFT * sizeof (double));
		adcs[k] = (double*)malloc(NFFT * sizeof (double));
		//configASSERT(ACC1[k] && ACC2[k] && adcs[k]);
		//memset(ACC1[k], 0, NFFT* sizeof (double));
		//memset(ACC2[k], 0, NFFT* sizeof (double));
		//memset(adcs[k], 0, NFFT* sizeof (double));
	}
	memset(sum, 0, NFFT * sizeof (double));
	
	for (i = 0; i < M; i++)	{
		for (j = 0; j < NFFT; j++)	{
			if (i == 0)	{
				ACC1[i][j] = input[j];
			} else {
				ACC1[i][j] = input[i*a + j];
			}
		}
	}	
	
	do {		
		mean= 0.0;
		for(i=0;i<M;i++){
			for(j=0;j<NFFT;j++)
		mean+=ACC1[i][j];
		}
		mean/=M*NFFT;
		for(i=0;i<M;i++){
		for(j=0;j<NFFT;j++)
		ACC1[i][j]-=mean;}
	} while (0);	
	
	for (i = 0; i < M; i++) {
		for (j = 0; j < NFFT; j++) {
			ACC2[i][j] = ACC1[i][j];
		}
	}

		
	for (i = 0; i < M; i++) {
		dcspectrm(*(ACC1+i)-1, *(ACC2+i)-1, *(adcs+i)-1, NFFT, WINDOW_OPTION);	
	}
	
		
	for (j = 0; j < NFFT; j++){
	   for (i = 0; i < M; i++) {
			sum[j]+=adcs[i][j];		
		}
	}
	   for (j = 0; j < NFFT; j++) {
	   	sum[j]/=M;
	   }

	for (j = 0; j < NFFT / 2; j++) {
		APS[j] = sum[2*j] / FS;   // according to the window funcation,
		                              //conj(x)*(x)/(Nfft*fs)/V, where V is 1/Nfft*sum(wn^2), then conj(x)*(x)/FS*sum(wn^2)
	}
	for (i = 1; i <NFFT/2; i++) {
		APS[i] = APS[i]*2;
	}
	
	for (k = 0; k < M; k++) {
		free(ACC1[k]);
		free(ACC2[k]);
		free(adcs[k]);
	}
	free(ACC1);
	free(ACC2);
	free(adcs);
	free(sum);
}
