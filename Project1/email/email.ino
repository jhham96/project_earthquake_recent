#include <mp3.h>      // mp3 library
#include <email.h>    // for email library

// #include <LiquidCrystal_I2C.h>     //LiquidCrystal 라이브러리 추가


// LiquidCrystal_I2C lcd(0x27, 16, 2);  //lcd 객체 선언

char data;

void setup() {
  Serial.begin(9600);
  while(!Serial)
    ;

  setup_email();    // email code setup
  Serial.println("MP3 setup 실시");
  setup_mp3();      // mp3 code for setup

  // lcd.begin(); //LCD 사용 시작
  //
  // lcd.clear();
  // lcd.setCursor(0, 0);    // 커서를 0, 0에 가져다 놓아라. (열, 행)
  // lcd.print("1. sensor");     // 출력

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

}
