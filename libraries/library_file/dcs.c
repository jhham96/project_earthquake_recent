#include <math.h>
#include "nr_utils.h"
#include "drealft.h"
//#include <FreeRTOS.h>
//#include <task.h>
#define WINDOW(j,a,b) (1.0-SQR((((j)-1)-(a))*(b)))

void dcspectrm(double *p1, double *p2, double *p3, unsigned long m, int window_option)
/* p1 and p2 need to be different. If Auto spectrum is to be calculated, you need to have 2 arrays p1 and p2.
 you cannot use cspectrm(p1, p1, p3,...) This misuse will fft p1 twice.  conj(p1)x(p2)
*/
{
  double facp, facm, w, scale;
	int i1;
	facm  = m/2;
	facp = 2.0/m;
	scale = 0.0;
  
	if (window_option == 1) { // welch
		for (i1 = 1; i1 <= m; i1++) {
			w = WINDOW(i1, facm, facp);
			p1[i1] *= w;
			p2[i1] *= w;
			scale += w*w;	// scaling for windowing funtion.  window.'*window/nfft. the last /nfft is executed at line 31.
		 }
	 }
	 else if (window_option == 2) { // hamming
		for (i1 = 1; i1 <= m; i1++) {
			w = 0.54-0.46*cos(2.0*3.14159265358979299*(i1-1.0)/(m-1.0));
			p1[i1] *= w;
			p2[i1] *= w;
			scale += w*w;	// scaling for windowing funtion.  window.'*window/nfft. the last /nfft is executed at line 31.
		 }
	}
	else if (window_option == 3) { // hanning
		for (i1 = 1; i1 <= m; i1++) {
			w = 0.5*(1.0-cos(2.0*3.14159265358979299*(i1-1.0)/(m-1.0)));
			p1[i1] *= w;
			p2[i1] *= w;
			scale += w*w;	// scaling for windowing funtion.  window.'*window/nfft. the last /nfft is executed at line 31.
		 }
	} else {
		scale = m;
	}
	scale = 1.0/scale;  /*the last /m: cross spectrum need to be devided by nfft. definition X*Y'/nfft. 1/(window'*window/nfft)/nfft */
	drealft(p1,m,1);
	drealft(p2,m,1);
	p3[1] = p2[1]*p1[1]*scale;
	p3[2] = p2[2]*p1[2]*scale;
	for (i1 = 3; i1 <= m; i1 = i1 + 2) {
		p3[i1] = (p2[i1]*p1[i1]+p2[i1+1]*p1[i1+1])*scale;
		p3[i1+1] = (-p2[i1]*p1[i1+1]+p2[i1+1]*p1[i1])*scale;
	}
}

#undef WINDOW
