#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include "header.h"

SimpleLed LED;
SimpleLCD LCD;
SimpleIR IR;
SimpleBTN BTN;

void system_setup() {
  Serial.begin(2000000);
  // NOTE : 통신 속도를 높이기 위해 2Mbps로 설정
  // IrReceiver.begin(IR_PIN, DISABLE_LED_FEEDBACK);
  IR.init();
  // NOTE : IR 수신기 초기화
  lcd.init();
  // NOTE : LCD 초기화
  lcd.backlight();
  // NOTE : LCD 배경 불 켜기
  lcd.setCursor(0, 0);
  for(int i=0;i<6;i+=1) {
    pinMode(LED_LIST[i], OUTPUT);
  }
  pinMode(BTN_YELLOW, INPUT);
  pinMode(BTN_ORANGE, INPUT);
  // NOTE : I/O init

}

void setup() {
  system_setup();
}

// TODO : 타이머를 이용하여, 주기적으로 각 상태를 체크하여, 해당 상태별 동작을 수행하도록 한다.
// TODO : 간단하지만, 시분할 사용을 아두이노로 구현해보도록함.

const unsigned long START = millis();
unsigned long CUR = millis();

enum CheckState {
  CHECK_IR,
  CHECK_BTN,
  CHECK_LCD,
  CHECK_LED,
  CHECK_STATE_MAX
};




void loop() {

  CUR = millis();
  unsigned int check_state = (CUR - START) % CHECK_STATE_MAX;
  // if (IrReceiver.decode()) {
  //   Println(IrReceiver.decodedIRData.decodedRawData, HEX);
  //   IrReceiver.resume();
  // }
  // Println(CUR - START);

  switch (check_state) {
    case CHECK_IR:
      // Println("CHECK_IR");
      IR.check(CUR);
      break;
    case CHECK_BTN:
      // Println("CHECK_BTN");
      BTN.check(CUR);
      break;
    case CHECK_LCD:
      // Println("CHECK_LCD");
      LCD.check(CUR);
      break;
    case CHECK_LED:
      // Println("CHECK_LED");
      LED.check(CUR);
      break;
    default:
      break;
  }
}
