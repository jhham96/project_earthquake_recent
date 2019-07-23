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
      email.println("안녕 내 이름은 함형이야");
      email.println("이메일이 되면 너무 좋겠구나");
      email.println("Good luck");
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
