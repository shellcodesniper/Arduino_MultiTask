#ifndef _CONSTANTS_HPP
#define _CONSTANTS_HPP

#define Print(...) Serial.print(__VA_ARGS__)
#define Println(...) Serial.println(__VA_ARGS__)
#define BTN_YELLOW 2 // TYPE : 1번 버튼
#define BTN_ORANGE 4 // TYPE : 2번 버튼
#define IR_PIN 7 // TYPE : 적외선 버튼
#define LED_1 8 
#define LED_2 9
#define LED_3 10
#define LED_4 11
#define LED_5 12
#define LED_6 13
// NOTE : 적외선 PIN 설정
// macro BTN_ON(X) digitalRead(X) == HIGH
// macro BTN_OFF(X) digitalRead(X) == LOW

int LED_LIST[6] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6 }; // TYPE : 제어 가능한 LED

LiquidCrystal_I2C lcd(0x27, 16, 2);

char DISPLAY_MSG[2][16];

int cursor_row = 0, cursor_col = 0;

const char CYCLE_RESET_STRING[12] = "            ";

// It will be shared ( same thread, don't be worry )

#endif
