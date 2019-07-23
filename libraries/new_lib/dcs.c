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
 * 	 B.F. Spencer, Jr. (bfs@uiuc.edu)
 * 
 * This work was supported in part by:
 * 
 * NSF grants CMS 06-00433 and CNS 05-09321 
 * NCASSR grant N00014-04-1-0562
 * DARPA grant F33615-01C-1907
 */

#include <math.h>
#include "nr_utils.h"
#include "drealft.h"

#define WINDOW(j,a,b) (1.0-SQR((((j)-1)-(a))*(b)))

void dcspectrm(double p1[], double p2[], double p3[], unsigned long m, int window_option)
/* p1 and p2 need to be different. If Auto spectrum is to be calculated, you need to have 2 arrays p1 and p2.
 you cannot use cspectrm(p1, p1, p3,...) This misuse will fft p1 twice.  conj(p1)x(p2)
*/
{

    double facp, facm, w,scale;
	int i1;
	facm  = m/2;
	facp = 2.0/m;
	scale = 0.0;
	if (window_option==1){/*welch*/
		for (i1 = 1;i1<=m;i1++){
			w = WINDOW(i1,facm,facp);
			p1[i1] *=w;
			p2[i1] *=w;
			scale +=w*w;	/*scaling for windowing funtion.  window.'*window/nfft. the last /nfft is executed at line 31. */
		 }
	 }
	 else if (window_option == 2){/*hamming*/
		for (i1 = 1;i1<=m;i1++){
			w = 0.54-0.46*cos(2.0*3.14159265358979299*(i1-1.0)/(m-1.0));
			p1[i1] *=w;
			p2[i1] *=w;
			scale +=w*w;	/*scaling for windowing funtion.  window.'*window/nfft. the last /nfft is executed at line 31. */
		 }
	}
	else if (window_option ==3){/* hanning */
		for (i1 = 1;i1<=m;i1++){
			w = 0.5*(1.0-cos(2.0*3.14159265358979299*(i1-1.0)/(m-1.0)));
			p1[i1] *=w;
			p2[i1] *=w;
			scale +=w*w;	/* scaling for windowing funtion.  window.'*window/nfft. the last /nfft is executed at line 31. */
		 }
	}
	 else{
		scale = m;
	}
	scale = 1.0/scale;  /*the last /m: cross spectrum need to be devided by nfft. definition X*Y'/nfft. 1/(window'*window/nfft)/nfft */
	drealft(p1,m,1);
	drealft(p2,m,1);
	p3[1] = p2[1]*p1[1]*scale;
	p3[2] = p2[2]*p1[2]*scale;
	for(i1 =3;i1<=m;i1 = i1+2){
		p3[i1]=(p2[i1]*p1[i1]+p2[i1+1]*p1[i1+1])*scale;
		p3[i1+1]=(-p2[i1]*p1[i1+1]+p2[i1+1]*p1[i1])*scale;
	}
	/* scaling for windowing funtion. window.'*window/nfft
	
	  X*Y'/nfft
	*/
}

#undef WINDOW
