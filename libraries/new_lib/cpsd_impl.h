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
#ifndef _CPSD_IMPL_H
#define _CPSD_IMPL_H

/* no NesC code in this file! */

#include <assert.h>
#ifdef PLATFORM_IMOTE2
#include "sdram.h"
#endif

char cpsd_impl(double *in1, double *in2, doublecomplex *out, unsigned long n_fft,
    int window_option, char normalize)
{
    double *DataCSD1, *DataCSD2, *out_double;
    unsigned long i;

#ifdef PLATFORM_IMOTE2
    DataCSD1 = (double *)sdmalloc(n_fft*sizeof(double));
    DataCSD2 = (double *)sdmalloc(n_fft*sizeof(double));
    out_double = (double *)sdmalloc(n_fft*sizeof(double));
#else
    DataCSD1 = (double *)malloc(n_fft*sizeof(double));
    DataCSD2 = (double *)malloc(n_fft*sizeof(double));
    out_double = (double *)malloc(n_fft*sizeof(double));
#endif
	assert(DataCSD1 && DataCSD2 && out_double);

    /* cannot do computation in place - will corrupt in1 and in2 */
    for (i = 0; i < n_fft; i++) {
        DataCSD1[i] = in1[i];
    }
    for (i = 0; i < n_fft; i++) {
        DataCSD2[i] = in2[i];
    }

    /* normalize: mean = 0 */
    if (normalize) {
        double mean1, mean2;
        mean1 = mean2 = 0.0;
        for (i = 0; i < n_fft; i++) {
            mean1 += DataCSD1[i];
            mean2 += DataCSD2[i];
        }
        mean1 /= n_fft;
        mean2 /= n_fft;
        for (i = 0; i < n_fft; i++) {
            DataCSD1[i] -= mean1;
            DataCSD2[i] -= mean2;
        }
    }

    dcspectrm(DataCSD1 - 1, DataCSD2 - 1, out_double - 1, n_fft, window_option);
    for (i = 0; i < n_fft; i++) {
        out_double[i] = out_double[i] / n_fft * 2;
    }
  
  	out[0].r = out_double[0];
	out[0].i = 0;
	out[n_fft / 2].r = out_double[1];
	out[n_fft / 2].i = 0;
	for (i = 1; i < n_fft / 2; i++) {
		out[i].r = out_double[2 * i];
		out[i].i = out_double[2 * i + 1];
	}

#ifdef PLATFORM_IMOTE2
    sdfree(DataCSD1);
    sdfree(DataCSD2);
	sdfree(out_double);
#else
    free(DataCSD1);
    free(DataCSD2);
	free(out_double);
#endif
    return 0;
}

#endif /* _CPSD_IMPL_H */
