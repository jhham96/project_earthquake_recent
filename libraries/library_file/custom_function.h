// custom_function.h
#include <Arduino.h>
#include <avr/dtostrf.h>

// LCD 출력을 위한 라이브러리
#include <LiquidCrystal_I2C.h>     //LiquidCrystal 라이브러리 추가

#define NFFT 1024

void setup_lcd();
double power(double a, int n);
double root(double a);
void preprocessing(int _nacc, double *lx, double *lz, double *acc);
double mean_f(double* array, int size);
double calStd(double* array, int size, int index, int option);
double calStdPart(double* acc, int div, int start, int finish);
int sineOrSeismic(double* acc);
double findMAXdata();
void distinguish_wave(double *sxx, int nfreq);
