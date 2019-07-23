// custom_function.cpp
#include <custom_function.h>

extern int nfreq;
extern int nacc;
extern int fs;

extern double acc_x[];
extern double acc_z[];

extern LiquidCrystal_I2C lcd;

void setup_lcd()
{
  lcd.begin(); //LCD 사용 시작
}

double power(double a, int n)
{
  if (a == 0) { return 0; }

  int i = 0;
  double ret = 1;

  for (i = 0; i < n; i++)
  {
    ret *= a;
  }

  return ret;
}

double root(double a)
{
  double x = 2;

  for (int i = 0; i < 100; i++)
  {
    x = (x + (a / x)) / 2;
  }

  return x;
}

void preprocessing(int _nacc, double *lx, double *lz, double *acc)
{
  double sum = 0;

  // 1. acc = ((10 + lx)^2 + (10 + lz)^2)^(1/2)
  // Serial.print("preprocessing step 1 - ");
  for (int i = 0; i < _nacc; i++) {
    acc[i] = root((10 + lx[i]) * (10 + lx[i]) + (10 + lz[i]) * (10 + lz[i]));
    sum += acc[i];
  }
  // Serial.println("complete");

  // Serial.print("preprocessing step 2 - ");
  // 2. acc = acc - mean(acc)
  double mean = sum / _nacc;
  for (int i = 0; i < _nacc; i++) {
    acc[i] -= mean;
  }
  // Serial.println("complete");

  // 3. Initialize the global variable value with the value of the variable in the header file.
  nfreq = NFFT / 2;
  //A = acc;
}

// 산술 평균 구하기
double mean_f(double* array, int size)
{
  double sum = 0.0;

  for (int i = 0; i < size; i++)
    sum += array[i];

  return sum / size;
}


// 표준 편차 계산 함수
double calStd(double* array, int size, int index, int option)
{

  double sum = 0.0;
  double sd = 0.0;
  double diff;
  double meanValue = mean_f(array, size);

  if (option == 0) {
    for (int i = index; i < size; i++) {
      diff = array[i] - meanValue;
      sum += diff * diff;
    }
    sd = root((sum / (size - index)));
  }
  else if (option == 1) {
    // Option 1 is the case of obtaining the standard deviation with all x and y data.
    // 이때 calStd의 array 변수는 사용하지 않고, header파일에서 선언한 acc_x와 acc_z 변수를 사용한다.

    // re-calculate meanValue
    double sum_for_mean = 0.0;

    for (int i = 0; i < size; i++)
      sum_for_mean = sum_for_mean + acc_x[i] + acc_z[i];

    meanValue = sum_for_mean / (size * 2);

    for (int i = index; i < size; i++) {
      diff = acc_x[i] - meanValue;
      sum += diff * diff;

      diff = acc_z[i] - meanValue;
      sum += diff * diff;
    }
    sd = root((sum / (size * 2)));
  }

  return sd;
}

double calStdPart(double* acc, int div, int start, int finish) {
  double sum = 0;
  double diff;
  double meanValue;

  int count = 0;

  for (int i = nacc / div * start ; i < nacc / div * finish ; i++) {
    sum += acc[i];
    count++;
  }
  meanValue = sum / count;

  sum = 0;
  for (int i = nacc / div * start ; i < nacc / div * finish ; i++) {
    diff = acc[i] - meanValue;
    sum += diff * diff;
  }

  return root(sum / count);
}

int sineOrSeismic(double* acc)
{
  // 총 6개의 표준편차 std 값을 구한다.
  double std[6] = {0, };

  // std1 : 0 ~ 1/2
  std[0] = calStdPart(acc, 2, 0, 1);
  // std2 : 1/2 ~ 1
  std[1] = calStdPart(acc, 2, 1, 2);
  // std3 : 0 ~ 1/4
  std[2] = calStdPart(acc, 4, 0, 1);
  // std4 : 1/4 ~ 2/4
  std[3] = calStdPart(acc, 4, 1, 2);
  // std3 : 2/4 ~ 3/4
  std[4] = calStdPart(acc, 4, 2, 3);
  // std3 : 3/4 ~ 1
  std[5] = calStdPart(acc, 4, 3, 4);


  // find min, max in std[]
  double min, max;
  min = max = std[0];   // min, max를 std[0]으로 초기화
  for(int i = 1 ; i < 6 ; i++) {
    min = min < std[i] ? min : std[i];
    max = max > std[i] ? max : std[i];
  }

  double rate = min / max;
  Serial.print("rate : "); Serial.println(rate);

  if(rate > 0.75)   // min / max의 값이 1에 가까운 경우, 즉 정현파처럼 변화가 없는 경우를 의미
  {
    return 0;
  }
  else    // 반대의 경우로 지진파에 해당한다.
  {
    return 1;
  }
}

double findMAXdata()
{
  // acc_x와 acc_z 데이터에서 가장 큰 값을 찾아 반환한다.(절댓값 취해서)
  double max = 0;

  for (int i = 0; i < nacc; i++) {
    // 1. Compare x and y;
    // 2. Compare present max and (x or y);
    if (abs(acc_x[i]) >= abs(acc_z[i])) {
      if (max < abs(acc_x[i])) {
        max = acc_x[i];
      }
    }
    else {
      if (max < abs(acc_z[i])) {
        max = acc_z[i];
      }
    }
  }

  return max;
}

void distinguish_wave(double *sxx, int nfreq)
{
  // Set the thresh
  double thresh = mean_f(sxx, nfreq) + 2 * calStd(sxx, nfreq, 0, 0);
  //double thresh = 1.5 * mean_f(sxx, nfreq) + 3 * calStd(sxx, nfreq, 0, 0);


  // 40Hz 이후에 해당하는 인덱스들
  // 40 / (fs / NFFT) - 1 = i
  // EX> 40 / (256 / 1024) - 1 = 159(160번째)
  int count = 0;
  char check = 0;
  for (int i = 40 / (fs / NFFT) - 1; i < nfreq; i++) {
    if (sxx[i] >= thresh) {
      count++;
    }
    if (count >= 50) {
      check = 1;
      break;
    }
  }
  if (check == 1) {
    // 40Hz가 넘는것이 50개가 넘어가면 지진파가 아니라고 판단.
   Serial.println("발파에 해당");
  }
  else {
    // 그렇지 않는 경우, 세부 지진파 구별 단계로 진행

    check = 0;    // if count over 10, check change zero to one(0->1)
    int count_over_thresh = 0;  // count number of over thresh data value
    // Counts data beyond thresh.
    for (int i = 0; i < nfreq; i++) {
      if (sxx[i] > thresh) {
        count_over_thresh++;
        if (count_over_thresh >= 15) {
          check = 1;
          break;
        }
      }
    }
    if (check == 1) {
      double maxValueOfXandY = findMAXdata();

      digitalWrite(25,LOW);    // 초록 LED 소등
      digitalWrite(26,LOW);    // 초록 LED 소등
      digitalWrite(27,LOW);    // 초록 LED 소등

      if (maxValueOfXandY > 0.264) {
        // 3단계
        Serial.print("3단계\n");
        // 0.1초 간격으로 50번 빨강 LED 점등    (0.3초 * 17 = 5.1초)
        for(int i = 0 ; i < 17; i++) {
          digitalWrite(22,HIGH);
          delay(100);
          digitalWrite(22,LOW);
          digitalWrite(23,HIGH);
          delay(100);
          digitalWrite(23,LOW);
          digitalWrite(24,HIGH);
          delay(100);
          digitalWrite(24,LOW);
        }
      }
      else {
        if (maxValueOfXandY > 0.066) {
          // 2단계
          Serial.print("2단계\n");
          // 0.2초 간격으로 50번 빨강 LED 점등   (0.6초 * 8 = 4.8초)
          for(int i = 0 ; i < 8; i++) {
            digitalWrite(22,HIGH);
            delay(200);
            digitalWrite(22,LOW);
            digitalWrite(23,HIGH);
            delay(200);
            digitalWrite(23,LOW);
            digitalWrite(24,HIGH);
            delay(200);
            digitalWrite(24,LOW);
          }
        }
        else {
          // 1단계
          Serial.print("1단계\n");
          // 0.3초 간격으로 50번 빨강 LED 점등   (0.9초 * 6 = 5.4초)
          for(int i = 0 ; i < 6; i++) {
            digitalWrite(22,HIGH);
            delay(300);
            digitalWrite(22,LOW);
            digitalWrite(23,HIGH);
            delay(300);
            digitalWrite(23,LOW);
            digitalWrite(24,HIGH);
            delay(300);
            digitalWrite(24,LOW);
          }
        }
      }
    }
    else {
      // 10개를 넘지 못하는 경우 = 지진파가 아닌 경우
      Serial.print("Structure\n");
      Serial.write('A');
      lcd.setCursor(5, 0);    // 커서를 5, 0에 가져다 놓아라. (열, 행)
      lcd.print("2st");     // 5, 0에 Hi ^^를 출력해라.
      lcd.setCursor(3, 1);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
      lcd.print("wave step"); // Codingrun을 입력해라.


      // 12:00 이후 수정할 부분
      for(int i = 0 ; i < 16; i++) {     // (0.6초 * 16 = 9.6초)
            digitalWrite(22,HIGH);
            delay(200);
            digitalWrite(22,LOW);
            digitalWrite(23,HIGH);
            delay(200);
            digitalWrite(23,LOW);
            digitalWrite(24,HIGH);
            delay(200);
            digitalWrite(24,LOW);
      }
      // 수정부분


      lcd.clear();            // 글자를 모두 지워라.
    }
  }
}
