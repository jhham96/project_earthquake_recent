#include <email.h>

PhpocEmail email;

void setup_email() {
  // initialize PHPoC [WiFi] Shield:
  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);
  //Phpoc.begin();
  Serial.println("email setup finish");
}


void loop_email(char data) {

  switch (data) {
    case 'A':
      // A: 건물지진이다.
      Serial.println("Sending email to gmail relay server");

      // [login using your private password]
      // Google may block sign-in attempts from some apps or devices that do not use
      // modern security standards.
      // Change your settings to allow less secure apps to access your account.
      // https://www.google.com/settings/security/lesssecureapps

      // [login using app password]
      // 1. turn on 2-step verification
      // 2. create app password
      // 3. apply app password as your login password

      // setup outgoing relay server - gmail.com:
      email.setOutgoingServer("smtp.gmail.com", 587);
      email.setOutgoingLogin("parkkisun93", "ks681170");

      // setup From/To/Subject:
      email.setFrom("parkkisun93@gmail.com", "함지훈");
      email.setTo("aha6323@naver.com", "박기선");
      // email.setTo("haba1216@naver.com", "박준영");
      email.setSubject("Mail from PHPoC Shield for Arduino");

      // write email message
      email.beginMessage();
      email.println("긴급 지진 경보입니다.\n\n메일로 대응 강령을 발송해 드리니 반드시 숙지하고 행동하시기 바랍니다.\n
                    1. 몸을 낮추고 천장에서 떨어지는 물체에 대비해 머리를 보호하세요\n
                    2. 주변 고중량 장비의 움직임으로 인한 부상에 주의하세요\n
                    3. 흔들림이 멈추면 지진방재연구센터 바로 옆 운동장으로 대피하세요\n
                    4. 사용자의 위치에서 가장 가까운 옥외야외대피소는 북서쪽 800m 근방의 서남초등학교 입니다.\n");
      email.endMessage();

      // send email:
      if(email.send() > 0)
       Serial.println("Email send ok");
      else
       Serial.println("Email send failed");

      data = 0;

      break;
    case 'B':
      // B: 정현파이다.
      break;
    case 'C':
      // C: 지진 1단계
      break;
    case 'D':
      // D: 지진 2단계
      break;
    case 'E':
      // E: 지진 3단계
      break;
    default:
      ;
  }

}
