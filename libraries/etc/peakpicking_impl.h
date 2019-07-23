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
/**
 * @author: Soojin Cho, Sung-Han Sim, Jian Li
 **/

#ifndef _PEAKPICKING_IMPL_H
#define _PEAKPICKING_IMPL_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define MAX_VALUE(a, b) (((a) > (b)) ? (a) : (b))
#define MIN_VALUE(a, b) (((a) > (b)) ? (b) : (a))
#define PI 3.14159265358979323846264338327950288419716939937

void peakpicking2_impl(double *psd, uint16_t psd_len, double df, double *ref, uint8_t ref_len, uint8_t wing_size, double *fn)
{
	uint16_t i, j, k, intv_a, intv_b;
	double max, max_tmp;

	for (i = 0; i < ref_len; i++) {
		k = ref[i] / df;
		max = psd[k];
		intv_a = MAX_VALUE(ref[i] / df - wing_size, 0);
		intv_b = MIN_VALUE(ref[i] / df + wing_size, psd_len - 1);

		if (i > 0)
			intv_a = MAX_VALUE(intv_a, ref[i - 1] / df);
		if (i < ref_len - 1)
			intv_b = MIN_VALUE(intv_b, ref[i + 1] / df);

		for (j = intv_a; j <= intv_b; j++) {
			max_tmp = MAX_VALUE(max,psd[j]);
			if (max_tmp != max) k = j;
			max = max_tmp;
		}
		fn[i] = k * df;
	}
}

/* Finds lower N peaks of an array */
void peakpicking1_impl(double *data, int len, double df, double *fn, uint8_t *nfn)
{
	int current = 1, foundPeak = 0, i, buffer = 0, maxpeaks = *nfn;
	int *tempnum;
	double average = 0, stddev = 0, limit, local_max = 0;
	double *temp, currentData[3];

	if (maxpeaks == 0) return;

#ifdef PLATFORM_IMOTE2
	tempnum = (int *)sdcalloc(maxpeaks, sizeof (int));
	temp = (double *)sdcalloc(maxpeaks, sizeof (double));
#else
	tempnum = (int *)malloc(maxpeaks * sizeof (int));
	temp = (double *)malloc(maxpeaks * sizeof (double));
	memset(tempnum, 0, maxpeaks * sizeof (int));
	memset(temp, 0, maxpeaks * sizeof (double));
#endif

	/* Find absolute mean and standard deviation of FFT data */
	for(i = 1; i < len; i++) average += data[i]; average /= len;
	for(i = 1; i < len; i++) stddev += (data[i] - average)*(data[i] - average);
	stddev = sqrt(stddev / (len - 1));

	/* Set upper limit for peak picking with 95% confidence */
	limit = average + 2 * stddev;

	currentData[1] = data[current];
	currentData[2] = data[current + 1];
	current = current + 1;

	/* Find all peaks present in FFT Data */
	while((current < len) && (foundPeak < maxpeaks)) {
		currentData[0] = currentData[1];
		currentData[1] = currentData[2];
		currentData[2] = data[current + 1];

		/* Is current point a peak? */
		if((currentData[1] > currentData[0]) &&
		   (currentData[1] > currentData[2]) &&
		   (currentData[1] > limit) &&
		   (currentData[1] > local_max)) /* If a point is a local peak (larger than adjacent values) and over the preset limit */
		{
			local_max = currentData[1];
			buffer = 0;
		} else {
			if (++buffer == 51 && local_max > 0) { /* If no local peak larger than stored local peak is found until searching 50 next FFT data */
				temp[foundPeak] = local_max; /* Stored as a natural peak */
				tempnum[foundPeak++] = current - 50;
				local_max = 0;
			}
		}
		current++;
	}

	/* natural frequency */
	memset(fn, 0, maxpeaks * sizeof (double));
	*nfn = foundPeak;
	for (i = 0; i < foundPeak; i++) fn[i] = df * (double)(tempnum[i] - 1);

#ifdef PLATFORM_IMOTE2
	sdfree(tempnum);
	sdfree(temp);
#else
	free(tempnum);
	free(temp);
#endif
}

/* optimization to find better peak locations */
double getmax(double *a, uint16_t len)
{
	uint16_t i;
	double max = 0;
	for (i = 0; i < len; i++) {
		max = max > a[i] ? max : a[i];
	}
	return max;
}

double halfpower(double *pxx, uint16_t len, double f_left, double df)
{
	uint16_t i;
	double ph, f1, f2;

	ph = getmax(pxx, len); ph /= 2;
	/* f1 */
	for (i = 0; i < len / 2; i++) if (pxx[i] < ph && ph < pxx[i + 1]) break;
	if (i == len / 2) return 0;
	f1 = f_left + df * i + (ph - pxx[i]) / (pxx[i + 1] - pxx[i]) * df;
	/* f2 */
	for (i = len / 2; i < len - 1; i++) if (pxx[i] > ph && ph > pxx[i + 1]) break;
	if (i == len) return 0;
	f2 = f_left + df * (i + 1) - (ph - pxx[i + 1]) / (pxx[i] - pxx[i + 1]) * df;

	return (f2 - f1) / (f2 + f1);
}

double costfcn(double wn, double mt, double zt, double *pxx, double wleft, double dw, uint8_t wing_size) 
{
	uint8_t i;
	double ferr = 0, w, tmp;

	for (i = 0; i < 2 * wing_size + 1; i++) {
		w = wleft + i * dw;
		tmp = 1/mt/mt*w*w*w*w/((wn*wn-w*w)*(wn*wn-w*w)+(2*zt*wn*w)*(2*zt*wn*w)) - pxx[i];
		ferr += tmp * tmp;
	}
	return sqrt(ferr);
}

void curvefitting(double *pxx, double df, double fpeak, uint8_t wing_size, uint8_t nsegs, double *fopt)
{
	uint8_t i, idx_min = 0;
	double zt, mt, ferr, dfc, minv = 0;

	zt = halfpower(pxx, 2 * wing_size + 1, fpeak - df * wing_size, df);
	mt = 1 / (2 * zt * sqrt(pxx[wing_size]));

	if (pxx[wing_size - 1] < pxx[wing_size + 1]) dfc = df / nsegs;
	else dfc = - df / nsegs;

	for (i = 0; i < nsegs; i++) {
		ferr = costfcn(2 * PI * (fpeak + i * dfc), mt, zt, pxx, 2 * PI * (fpeak - df * wing_size), 2 * PI * df, wing_size);

		if (i == 0 || minv > ferr) {
			minv = ferr;
			idx_min = i;
		}
	}
	*fopt = fpeak + idx_min * dfc; 
	return;
}


#endif /* _PEAKPICKING_IMPL_H */
