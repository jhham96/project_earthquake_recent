/* ************************************************************************
 * 
 * Unless otherwise noted, the files which comprise the Structural Health
 * Monitoring software toolsuite (SHM) are subject to the following
 * restrictions.
 * 
 * The SHM software is NOT in the public domain.  However, it is freely
 * available without fee for education, research, and non-profit purposes.
 * By obtaining copies of this and other files that comprise the SHM
 * software, you, the Licensee, agree to abide by the following conditions
 * and understandings with respect to the copyrighted software:
 * 
 * 1.  The software is copyrighted in the name of the Board of Trustees
 *     of the University of Illinois (UI), and ownership of the software
 *     remains with the UI.
 * 
 * 2.  Permission to use, copy, and modify this software and its
 *     documentation for education, research, and non-profit purposes is
 *     hereby granted to the Licensee, provided that the copyright
 *     notice, the original author's names and unit identification, and
 *     this permission notice appear on all such copies, and that no
 *     charge be made for such copies.  Any entity desiring permission to
 *     incorporate this software into commercial products should contact:
 * 
 *          Professor Gul A. Agha                 agha@cs.uiuc.edu
 *          University of Illinois at Urbana-Champaign
 *          Department of Computer Science
 *          2104 Siebel Center
 *          201 North Goodwin Avenue
 *          Urbana, Illinois  61801
 *          USA
 * 
 * 3.  Licensee may not use the name, logo, or any other symbol of the UI
 *     nor the names of any of its employees nor any adaptation thereof
 *     in advertising or publicity pertaining to the software without
 *     specific prior written approval of the UI.
 * 
 * 4.  THE UI MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE
 *     SOFTWARE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS
 *     OR IMPLIED WARRANTY.
 * 
 * 5.  The UI shall not be liable for any damages suffered by Licensee
 *     from the use of this software.
 * 
 * 6.  The software was developed under agreements between the UI and the
 *     Federal Government which entitles the Government to certain
 *     rights.
 * 
 * ************************************************************************
 * 
 * Developed by:
 * 
 * Open Systems Lab
 * University of Illinois at Urbana-Champaign
 * Department of Computer Science
 * 201 North Goodwin Avenue
 * Urbana, IL  61801
 * http://osl.cs.uiuc.edu/
 * 
 * Smart Structures Technology Laboratory
 * University of Illinois at Urbana-Champaign
 * Department of Civil and Environmental Engineering
 * 205 North Matthews Avenue
 * Urbana, IL  61801
 * http://sstl.cee.uiuc.edu/
 * 
 * 
 * Send comments to: agha@cs.uiuc.edu
 * 
 * 
 * Copyright (c) 2008
 * The University of Illinois Board of Trustees.
 *      All Rights Reserved.
 * 
 * 
 * Principal Investigators:
 *      Gul A. Agha (agha@cs.uiuc.edu)
 *      B.F. Spencer, Jr. (bfs@uiuc.edu)
 * 
 * This work was supported in part by:
 * 
 * NSF grants CMS 06-00433 and CNS 05-09321 
 * NCASSR grant N00014-04-1-0562
 * DARPA grant F33615-01C-1907
 */

#define _CRT_SECURE_NO_WARNINGS

 /* config parameters */
#define NFFT 1024
#define WINDOW_OPTION 3
#define NORMALIZE 0

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include "f2c.h"
#include "dcs.h"
#include "drealft.h"
#include "cpsd_impl.h"
#include "data9.h"


/* inputs */
double *A, *B;

/* outputs */
doublecomplex *sxx, *sxy;

int nfreq;


FILE *input, *output;

//void read_input()
//{
//    int i;
//    if (fscanf(input, "%d\t%d\t%d\n", &NFFT, &WINDOW_OPTION, &NORMALIZE) != 3) {
//        fprintf(stderr, "could not read config parameters\n");
//        exit(1);
//    }
//    if ((A =  malloc(NFFT * sizeof (double))) == NULL
//    || (B =  malloc(NFFT * sizeof (double))) == NULL
//    || (sxx =  malloc(NFFT * sizeof (doublecomplex))) == NULL
//    || (sxy =  malloc(NFFT * sizeof (doublecomplex))) == NULL
//    ) {
//        fprintf(stderr, "malloc failed\n");
//        exit(1);
//    }
//    for (i = 0; i < NFFT; i++) {
//        if (fscanf(input, "%le\t%le\n", A + i, B + i) != 2) {
//            fprintf(stderr, "input contains < NFFT lines\n");
//            exit(1);
//        }
//    }
//    fprintf(stderr, "read A, B (%d lines)\n", i);
//	nfreq = NFFT / 2 + 1;
//}

void write_output()
{
    int i;
	//fprintf(output, "# sxx(A)\tsxy(A,B)\n");
    fprintf(output, "# sxx(A)\n");
    for (i = 0; i < nfreq; i++) {
		//fprintf(output, "%3.7e + 1i * %3.7e \t %3.7e + 1i * %3.7e\n", abs(sxx[i].r), sxx[i].i, sxy[i].r, sxy[i].i);
		fprintf(output, "%3.7e + 1i * %3.7e \n", abs(sxx[i].r), sxx[i].i);
    }
    fprintf(stderr, "written sxx(A) (%d lines)\n", i);

}

double power(double a, int n)
{
	if (a == 0) { return 0; }

	int i = 0;
	double ret = 1;

	for (i = 0; i < n; i++)
	{
		ret *= a;
	}

	return ret;
}

double root(double a, int n, double precision)
{
	double ret = 1;

	while ((power(ret, n) - a) < -precision || (power(ret, n) - a) > precision)
	{
		ret = ret * (n - 1) / n + a / (n * power(ret, n - 1));
	}

	return ret;
}

void preprocessing(int nacc, double fs, double *acc_x, double *acc_y, double *acc) {
	double sum = 0;
	
	// 1. acc = ((10 + acc_x)^2 + (10 + acc_y)^2)^(1/2)
	for (int i = 0; i < nacc; i++) {
		acc[i] = root((10 + acc_x[i]) * (10 + acc_x[i]) + (10 + acc_y[i]) * (10 + acc_y[i]), 2, 0.00001);
		sum += acc[i];
	}

	// 2. acc = acc - mean(acc)
	double mean = sum / nacc;
	for (int i = 0; i < nacc; i++) {
		acc[i] -= mean;
	}

	// 3. Initialize the global variable value with the value of the variable in the header file.
	nfreq = NFFT / 2 + 1;
	A = acc;
}



int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }
    if ((input = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "error opening input file\n");
        exit(1);
    }
    //read_input();
	sxx = malloc(NFFT * sizeof(doublecomplex));

	double *acc = (double *)malloc(sizeof(double) * nacc);
	preprocessing(nacc, fs, acc_x, acc_y, acc);

    /* compute auto-spectrum for A */
    fprintf(stderr, "starting auto-spectrum computaiton for A\n");
    cpsd_impl(A, A, sxx, (unsigned long)NFFT, WINDOW_OPTION, NORMALIZE);
    fprintf(stderr, "finished computing auto-spectrum for A\n");

    /* compute cross-spectrum for A, B */
    /*fprintf(stderr, "starting cross-spectrum computaiton for A, B\n");
    cpsd_impl(A, B, sxy, (unsigned long)NFFT, WINDOW_OPTION, NORMALIZE);
    fprintf(stderr, "finished computing cross-spectrum for A, B\n")*/;

    if ((output = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "error opening output file\n");
        exit(1);
    }
    write_output();
    return 0;
}
