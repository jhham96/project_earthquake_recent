# project_earthquake_recent

* update :
1. 새로운 트리거 시스템
* 이후에 작업할 내용을 위해 백업


* findmaxdata + 2 things update : 
바꾼점 
1. findmaxdata 최신화
2. INT port 번호 최신화 (4 -> 6)
3. 포트 번호 스위치(8 <-> 9)


* mp3 볼륨 수정 :
1. mp3 볼륨 수정
2. 이메일 data sended 출력문 조정
3. LCD 출력 부분 수정(커서위치 조정)
4. A 받을 수 있도록 딜레이 추가
   + sensor 부분 코드에선 custom_funcition에서 노래가 끝날때까지 코드가 잠시 중단되도록 delay 60초 추가
   
   
* sensing.cpp 외2건 수정 :
1. sensing.cpp
 - 이전 데이터와 다를 경우에만 센서 데이터를 저장하도록 한다.
2. adafruit_lis3dh.cpp
- 16mg => 32mg로 증가
3. custom_function.cpp
- count_over_thresh 값 볼 수 있도록 수정


>> custom_function.cpp 수정:
1. custom_function.cpp
: count_over_thresh 값 출력 구문 수정(버그)

>> custom_function.cpp 등 3건 수정:
1. custom_function.cpp
- thresh 식에서 계수를 2에서 1.5로 조정

2. sensor.iso
- 뭐가 바뀐거징...

3. adafruit_lis3dh.cpp
- 16mg -> 32mg 잘못처리함... 버그 수정
(32mg 잘 될거임)



>> 수정사항
1. 다시 16mg으로 감소
2. 트리거 부분 초기화
   (thresh 값 0.04 로 조정)
   Q. Interupt 방법을 쓰지 않고, delay도 넣지 않았는데, 어떻게 13초만에 데이터를 다 읽었는지....



>> 수정사항
1. 트리거 새로운 알고리즘 구성
(sensing.h - reading_for_avg 함수 추가)

1) 스위치로 전원이 들어오면, 맨처음 100개의 데이터를 읽어온다
2) 100개에서 평균을 잰다
3) 평균을 저장해, 나중에 트리거로 사용한다
if( ((acc_x - avg_x) / DIV) > 0.035)


2. calSTD에서 thresh 계수 1.5에서 1로 수정

3. count_thresh 기준값을 15에서 8로 수정


>> 수정사항
1. count_thresh 기준값을 8에서 10으로 수정


>> 수정사항
1. 16mg 활성화


>> 수정사항
1. 요구사항을 위한  x, y, z 순수값 데이터 가져오기위한 수정
요구사항 : 
#. x를 중력 방향
#. y를 중력 방향
#. z를 중력방향

2.  checkstart 비활성화
