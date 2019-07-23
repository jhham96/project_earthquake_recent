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
/*
 * Author: Sung-Han Sim
 */


#include <stdio.h>
#include <stdlib.h>
#include "peakpicking_impl.h"
#include "input.h"
#define WING_SIZE	6
#define NSEGS		10

int main(int argc, char **argv) 
{
	double fn1[NPEAKS], fn2[NPEAKS], fopt[NPEAKS], pxx[2 * WING_SIZE + 1];
	uint8_t nfn1 = NPEAKS, ref_freq_len = NPEAKS, i;
	
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		exit(1);
	}

	peakpicking1_impl(psd, PSD_LEN, DF, fn1, &nfn1);
	peakpicking2_impl(psd, PSD_LEN, DF, ref_freq, ref_freq_len, WING_SIZE, fn2);
	printf("peak(s) found:\n");
	printf("Algorithm 1:");
	if (nfn1 == 0) {
		printf("no peak found\n");
	} else {
		for (i = 0; i < nfn1; i++) printf(" %8.4f Hz,",fn1[i]); 
		printf("\n");
	}
	printf("Algorithm 2:");
	for (i = 0; i < ref_freq_len; i++) printf(" %8.4f Hz,",fn2[i]); printf("\n");
	
	/* curve fitting */
	printf("Algorithm 3:");
	for (i = 0; i < NPEAKS; i++) {
		memcpy(pxx, psd + (uint16_t)(fn2[i]/DF) - WING_SIZE, (2 * WING_SIZE + 1) * sizeof (double));
		curvefitting(pxx, DF, fn2[i], WING_SIZE, NSEGS, fopt + i);
		printf(" %8.4f Hz,", fopt[i]);
	}
	printf("\n");
	return 0;
}
