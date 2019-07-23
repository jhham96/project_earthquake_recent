#include <sensing.h>

// variable
extern int count;
extern int startp;

extern double acc_x[];
extern double acc_z[];

#define DIV 15987


// software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
//Adafruit_LIS3DH lis = Adafruit_LIS3DH();

float avg_x = 0, avg_z = 0;   // 평균을 구하기 위한 변수

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

void checkStart()
{
  // float x1, z1, x2, z2;   // 첫번째, 두번째 비교 데이터

  lis.setTrigger(true);   // trigger 모드로 전환
  while(1) {
    if(digitalRead(inPin) == 1) {    // 트리거 체크 레지스터
      Serial.println("Thresh over");
      startp=1;
      break;
    }
  lis.setTrigger(false);  // drdy 모드로 전환
  }

  // x1 = lis.x_g / DIV;
  // z1 = lis.z_g / DIV;
  //
  // Serial.println("===-=-=-=-대기상태=-=-=-==-=-");
  // while(1) {     // 1이되었을
  //   lis.read();
  //   x2 = lis.x_g / DIV;
  //   z2 = lis.z_g / DIV;
  //   if((abs(x1 - x2) > 0.035) || (abs(z1 - z2) > 0.035)) {    // 0.045
  //     Serial.print(abs(x1 - x2)); Serial.print("////"); Serial.println(abs(z1 - z2));
  //     Serial.println("Thresh over");
  //     startp = 1;
  //     break;
  //   } else {
  //     x1 = x2;
  //     z1 = z2;
  //   }
  //   delay(12000 / NUM_DATA);
  // }
}


void sensor_reading() {

  if (count == 0) {
    avg_x = 0;
    avg_z = 0;

    // start collecting data after 3 sec
    delay(3000);

    Serial.println("start");
  }
  if(digitalRead(inPin) == 1) {    // 1일때만 유의미한 데이터를 받아온다. 0이면 쓰레기값
    lis.read();

    // 이전 데이터와 읽어온 데이터가 다를 때만 데이터를 저장하도록 한다.
    if(acc_x[count - 1] != lis.x_g && acc_z[count - 1] != lis.z_g) {
      acc_x[count] = lis.x_g;
      avg_x += acc_x[count];

      acc_z[count] = lis.z_g;
      avg_z += acc_z[count];
      count++;
    }
  }

  if (count >= NUM_DATA) {
    avg_x /= NUM_DATA;
    avg_z /= NUM_DATA;

    Serial.print("\navg_x : "); Serial.println(avg_x);
    Serial.print("avg_z : "); Serial.println(avg_z);


    Serial.print("평균으로 빼기 - ");
    for(int i = 0 ; i < NUM_DATA ; i++) {
      acc_x[i] = (acc_x[i] - avg_x) / DIV;
      acc_z[i] = (acc_z[i] - avg_z) / DIV;
    }
    Serial.println("complete");


    // 센서값 출력(debug)
    Serial.println ("==============================");
    Serial.println ("X: ");
    char temp[20];
    char buffer[20];
    for (int i = 0; i < NUM_DATA; i++) {
      dtostrf(acc_x[i], 8, 6, temp);
      sprintf(buffer, "%s", temp);
      Serial.println(buffer);
    }
    Serial.println ("==============================");

    // Serial.println ("==============================");
    // Serial.println("Y: ");
    // for (int i = 0; i < NUM_DATA; i++) {
    //   dtostrf(acc_y[i], 8, 6, temp);
    //   sprintf(buffer, "%s", temp);
    //   Serial.println(buffer);
    // }
    // Serial.println ("==============================");

    Serial.println ("==============================");
    Serial.println("Z: ");
    for (int i = 0; i < NUM_DATA; i++) {
      dtostrf(acc_z[i], 8, 6, temp);
      sprintf(buffer, "%s", temp);
      Serial.println(buffer);
    }
    Serial.println ("==============================");
  }
}
