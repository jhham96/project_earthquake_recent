// full 시연 버전

extern "C" {
  #include <cpsdfunc.h>
  #include <dcs.h>
  #include <dfour1.h>
  #include <drealft.h>
  #include <f2c.h>
  #include <nr_utils.h>
}

// for .cpp file
#include <sensing.h>
#include <custom_function.h>

int count;

double acc_x[NUM_DATA] = {0, };
double acc_z[NUM_DATA] = {0, };

int nfreq;
int nacc = NUM_DATA;
int fs = 100;
int startp;

double* acc;
double* sxx;


void setup(void) {
  sensor_setup();
  count = 0;
  startp = 0;

  acc = (double *)malloc(sizeof(double) * nacc);
  sxx = (double *)malloc(sizeof(double) * NFFT / 2);

  digitalWrite(25,HIGH);    // 초록 LED 계속 점등
  digitalWrite(26,HIGH);    // 초록 LED 계속 점등
  digitalWrite(27,HIGH);    // 초록 LED 계속 점등
}

void loop() {
  if(startp == 0) {
    checkStart();
    Serial.print("1. sensor reading - ");
  }

  sensor_reading();

  if(count >= NUM_DATA) {

    Serial.println("complete");
    Serial.print("2. 선형추세 제거 - ");

    preprocessing(nacc, acc_x, acc_z, acc);  // 선형추세 제거
    Serial.println("complete");

    // detrend를 거친 acc값 출력        debug
//    char temp1[20];
//    char buffer1[20];
//    for(int i = 0 ; i < NFFT / 2 ; i++) {
//      dtostrf(acc[i], 8, 6, temp1);
//      sprintf(buffer1, "%s", temp1);
//      Serial.println(buffer1);
//    }

    Serial.print("3. sine or seismic - ");
    int wave = sineOrSeismic(acc);  // 지진파인지, 정현파인지 구별

    if (wave == 1) {
      // 지진파라고 생각되는 경우
      Serial.println("complete");

      Serial.print("4. calculate cpsd - ");

      calculate_cpsd(acc, sxx, NFFT, fs, nacc);
      Serial.println("complete");

      //////// processing finish //////////////

      // notification

      Serial.println("5. waveform classification - ");

      distinguish_wave(sxx, nfreq);
    }
    else if (wave == 0) {
      // 정현파인 경우
      Serial.println("sinusoidal wave or random wave");

      digitalWrite(25,LOW);    // 초록 LED 소등
      digitalWrite(26,LOW);    // 초록 LED 소등
      digitalWrite(27,LOW);    // 초록 LED 소등
      for(int i = 0 ; i < 17; i++) {
          digitalWrite(28,HIGH);  // 노랑 LED 점등
          delay(100);
          digitalWrite(28,LOW);
          digitalWrite(29,HIGH);
          delay(100);
          digitalWrite(29,LOW);
          digitalWrite(30,HIGH);
          delay(100);
          digitalWrite(30,LOW);
      }
    }

    // 다시 실행을 위한 초기화
    count = 0;
    startp = 0;
    digitalWrite(25,HIGH);    // 초록 LED 계속 점등
    digitalWrite(26,HIGH);    // 초록 LED 계속 점등
    digitalWrite(27,HIGH);    // 초록 LED 계속 점등

    Serial.println("start again in 3 seconds!");

    delay(3000);    // 3초 후 다시 시작
  }
}
