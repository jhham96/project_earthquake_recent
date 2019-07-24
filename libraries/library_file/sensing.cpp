#include <sensing.h>

// variable
extern int count;
extern int startp;

extern double acc_x[];
extern double acc_z[];

extern LiquidCrystal_I2C lcd;

#define DIV 15987

// software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
//Adafruit_LIS3DH lis = Adafruit_LIS3DH();

float avg_x_first, avg_z_first; // 설치환경에 따라 달라질 수 있는 초기 평균이 여기에 저장된다

float avg_x = 0, avg_z = 0;

int inPin = 4;      // INT1 input


void sensor_setup() {
  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(9600);
  Serial.println("LIS3DH test!");

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");

  lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  count = 0;

  reading_for_avg();  // 초기 평균값 확인


  // LED 설정
  // 빨강 LED
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(24,OUTPUT);

  // 초록 LED
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);

  // 노랑 LED
  pinMode(28,OUTPUT);
  pinMode(29,OUTPUT);
  pinMode(30,OUTPUT);

  pinMode(inPin, INPUT);    // INT : DRDY (데이터를 실제로 읽어서 저장할 때 사용되는 interrupt 레지스터)
                            // OR trigger 체크하는 레지스터
}

void reading_for_avg() {
  Serial.print("\n평균작업 시작 - ");
  avg_x_first = avg_z_first = 0;    // 0으로 초기화

  count = 0;
  while(count <= 100) {
    lis.read();

    // 이전 데이터와 읽어온 데이터가 다를 때만 데이터를 저장하도록 한다.
    if(count == 0) {  // 0은 아래서 인덱스 에러가 발생하지 않도록 특별 처리
      acc_x[count] = lis.x_g;
      avg_x_first += acc_x[count];

      acc_z[count] = lis.z_g;
      avg_z_first += acc_z[count];
      count++;
    }
    else if(acc_x[count - 1] != lis.x_g && acc_z[count - 1] != lis.z_g) {
      acc_x[count] = lis.x_g;
      avg_x_first += acc_x[count];

      acc_z[count] = lis.z_g;
      avg_z_first += acc_z[count];
      count++;
    }
  }

  // 평균 계산(나누기)
  avg_x_first /= 100;
  avg_z_first /= 100;

  count = 0;  // 다음에 쓰는 곳에서 제대로 쓸수 있도록 초기화
  Serial.println("complete");
}

void checkStart()
{
  double x, z;
  Serial.println("===-=-=-=-대기상태=-=-=-==-=-");
  lcd.clear();
  lcd.setCursor(0, 0);    // 커서위치(열, 행)
  lcd.print("idle state");

  while(1) {
    lis.read();
    x = lis.x_g;
    z = lis.z_g;
    if( ((lis.x_g - avg_x_first) / DIV > 0.035) || ((lis.z_g - avg_z_first) / DIV > 0.035) ) {
      Serial.print((x - avg_x_first) / DIV); Serial.print("////"); Serial.println((z - avg_z_first) / DIV);
      Serial.println("thresh over");
      lcd.clear();
      lcd.setCursor(0, 0);    // 커서위치(열, 행)
      lcd.print("thresh over");

      startp = 1;
      break;
    }
    delay(12000 / NUM_DATA);
  }
}


void sensor_reading() {

  if (count == 0) {
    avg_x = 0;
    avg_z = 0;

    // start collecting data after 3 sec
    lcd.clear();
    lcd.setCursor(0, 0);    // 커서위치(열, 행)
    lcd.print("3");
    delay(995);
    lcd.setCursor(4, 0);    // 커서위치(열, 행)
    lcd.print("2");
    delay(995);
    lcd.setCursor(8, 0);    // 커서위치(열, 행)
    lcd.print("1");
    delay(995);

    Serial.println("start");

    lcd.clear();
    lcd.setCursor(1, 0);    // 커서위치(열, 행)
    lcd.print("start!!");
  }

  lis.read();

  // 이전 데이터와 읽어온 데이터가 다를 때만 데이터를 저장하도록 한다.
  if(count == 0) {  // 0은 아래서 인덱스 에러가 발생하지 않도록 특별 처리
    acc_x[count] = lis.x_g;
    avg_x += acc_x[count];

    acc_z[count] = lis.z_g;
    avg_z += acc_z[count];
    count++;
  }
  else if(acc_x[count - 1] != lis.x_g && acc_z[count - 1] != lis.z_g) {
    acc_x[count] = lis.x_g;
    avg_x += acc_x[count];

    acc_z[count] = lis.z_g;
    avg_z += acc_z[count];
    count++;
  }
  delay(12000 / NUM_DATA);


  if (count >= NUM_DATA) {
    avg_x /= NUM_DATA;
    avg_z /= NUM_DATA;

    // Serial.print("\navg_x : "); Serial.println(avg_x);
    // Serial.print("avg_z : "); Serial.println(avg_z);


    // Serial.print("평균으로 빼기 - ");
    for(int i = 0 ; i < NUM_DATA ; i++) {
      acc_x[i] = (acc_x[i] - avg_x) / DIV;
      acc_z[i] = (acc_z[i] - avg_z) / DIV;
    }
    // Serial.println("complete");


    // // 센서값 출력(debug)
    // Serial.println ("==============================");
    // Serial.println ("X: ");
    // char temp[20];
    // char buffer[20];
    // for (int i = 0; i < NUM_DATA; i++) {
    //   dtostrf(acc_x[i], 8, 6, temp);
    //   sprintf(buffer, "%s", temp);
    //   Serial.println(buffer);
    // }
    // Serial.println ("==============================");

    // Serial.println ("==============================");
    // Serial.println("Y: ");
    // for (int i = 0; i < NUM_DATA; i++) {
    //   dtostrf(acc_y[i], 8, 6, temp);
    //   sprintf(buffer, "%s", temp);
    //   Serial.println(buffer);
    // }
    // Serial.println ("==============================");

    // Serial.println ("==============================");
    // Serial.println("Z: ");
    // for (int i = 0; i < NUM_DATA; i++) {
    //   dtostrf(acc_z[i], 8, 6, temp);
    //   sprintf(buffer, "%s", temp);
    //   Serial.println(buffer);
    // }
    // Serial.println ("==============================");
  }
}
