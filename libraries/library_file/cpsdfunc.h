/*
 * cpsdfunc.h
 *
 *  Created on: Nov 2, 2016
 *      Author: Mindy
 */



#ifndef CPSDFUNC_H_
#define CPSDFUNC_H_

#include "Arduino.h"

void forTest();
void calculate_cpsd(double *input, double *APS, int NFFT, double FS, int length);

#endif