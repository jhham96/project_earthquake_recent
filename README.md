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
