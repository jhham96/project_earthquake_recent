#include <mp3.h>      // mp3 library
#include <email.h>    // for email library


char data;

void setup() {
  Serial.begin(9600);
  while(!Serial)
    ;

  setup_email();    // email code setup
  Serial.println("MP3 setup 실시");
  setup_mp3();      // mp3 code for setup
}

void loop() {
  // due로 부터 data처리 방법 받아오기
  if(Serial.available())
  {
    data = Serial.read();
  }
  if(data == 'A') {
    Serial.print("data sended - "); Serial.println(data);

    // 공통적으로 해야하는 것들
    // 1. email 보내기
    loop_email(data);

    // 2. MP3로 음성안내
    Serial.println("MP3 음성안내 시작");
    while(loop_mp3())
      ;

    delay(60000);
    Serial.println("music finish");

    // data 초기화
    data = 0;
  }
  else if(data == 'B') {
    Serial.print("data sended - "); Serial.println(data);

    // 공통적으로 해야하는 것들
    // 1. email 보내기
    loop_email('A');

    // 2. MP3로 음성안내
    Serial.println("MP3 음성안내 시작");
    while(loop_mp3())
      ;

    delay(60000);
    Serial.println("music finish");

    // data 초기화
    data = 0;
  }
  else if(data == 'C') {
    Serial.print("data sended - "); Serial.println(data);

    // 공통적으로 해야하는 것들
    // 1. email 보내기
    loop_email('A');

    // 2. MP3로 음성안내
    Serial.println("MP3 음성안내 시작");
    while(loop_mp3())
      ;

    delay(60000);
    Serial.println("music finish");

    // data 초기화
    data = 0;
  }
  else if(data == 'D') {
    Serial.print("data sended - "); Serial.println(data);

    // 공통적으로 해야하는 것들
    // 1. email 보내기
    loop_email('A');

    // 2. MP3로 음성안내
    Serial.println("MP3 음성안내 시작");
    while(loop_mp3())
      ;

    delay(60000);
    Serial.println("music finish");

    // data 초기화
    data = 0;
  }
  else if(data == 'E') {
    Serial.print("data sended - "); Serial.println(data);

    // 공통적으로 해야하는 것들
    // 1. email 보내기
    loop_email('A');

    // 2. MP3로 음성안내
    Serial.println("MP3 음성안내 시작");
    while(loop_mp3())
      ;

    delay(60000);
    Serial.println("music finish");

    // data 초기화
    data = 0;
  }

}
