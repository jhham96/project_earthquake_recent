// custom_function.h
#include <Arduino.h>


#define NFFT 1024

double power(double a, int n);
double root(double a);
void preprocessing(int _nacc, double *lx, double *lz, double *acc);
double mean_f(double* array, int size);
double calStd(double* array, int size, int index, int option);
double calStdPart(double* acc, int div, int start, int finish);
int sineOrSeismic(double* acc);
double findMAXdata();
void distinguish_wave(double *sxx, int nfreq);
