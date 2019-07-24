#ifndef _SENSING_H
#define _SENSING_H

#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <avr/pgmspace.h>
#include <avr/dtostrf.h>

#include <LiquidCrystal_I2C.h>     //LiquidCrystal 라이브러리 추가


// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>
#include <SPI.h>

// Used for software SPI
#define LIS3DH_CLK 10
#define LIS3DH_MOSI 9
#define LIS3DH_MISO 8
// Used for hardware & software SPI
#define LIS3DH_CS 7

#define NUM_DATA 1200

void sensor_setup();
void reading_for_avg();
void sensor_reading();
void checkStart();   // 데이터 저장을 시작하기 위해 기준 thresh을 넘는지 확인하는 함수(넘으면 1을, 아니면 함수안의 while문을 looping 한다.)

#endif
