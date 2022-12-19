#ifndef _DISP_CONTROL
#define _DISP_CONTROL
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "constants.hpp"

// NOTE : setCursor(COL, ROW) 순서임.
//

class SimpleLCD {
private:
  bool waiting = false;
  unsigned long wake_up_time;

  unsigned long last_update; // HACK : 마지막 업데이트 시각
  unsigned long current_time; // HACK : 지금시각
  int action = 0; // HACK :
  int stored_action = -1; // HACK : 저장된 액션 ( 기존으로 돌아가기 위함 )

  char first_line[17] = "                ";
  char second_line[17] = "                ";
  uint8_t first_line_cursor = 0;
  uint8_t second_line_cursor = 0;

  bool stored_is_first = false;
  char stored_string[17] = "                ";

  void write_action(bool, char*);
  // NOTE: 10001 ~ 10010 : write action


public:
  void show_info_animation();
  void check(unsigned long now); // TYPE : 비동기 시뮬
  void wait(unsigned long time); // TYPE : 대기함수
};

// NOTE : stored action 사용, 종료후 sored_action 으로 돌아감.
void SimpleLCD::write_action(bool is_first, char *input_str) { 
  uint8_t row = (is_first) ? 0 : 1;
  uint8_t cursor = (is_first) ? first_line_cursor : second_line_cursor;
  char mini_batch[4] = "    ";
  switch (action) {
    case 10001:
      // NOTE : 여기서부터 작업 받아서 루프안에 들어온것임.
      // NOTE : 4글자씩 끊어서 출력
      lcd.setCursor(cursor, row);
      strncpy(mini_batch, input_str, 4);
      lcd.printstr(mini_batch);
      if (is_first) {
        first_line_cursor += 4;
      } else {
        second_line_cursor += 4;
      }
      wait(100);
      action = 10002;
      break;
    case 10002:

      lcd.setCursor(cursor, row);
      strncpy(mini_batch, input_str+4, 4);
      lcd.print(mini_batch);
      if (is_first) {
        first_line_cursor += 4;
      } else {
        second_line_cursor += 4;
      }
      wait(100);
      action = 10003;
      break;
    case 10003:

      lcd.setCursor(cursor, row);
      strncpy(mini_batch, input_str+8, 4);
      lcd.print(mini_batch);
      if (is_first) {
        first_line_cursor += 4;
      } else {
        second_line_cursor += 4;
      }
      wait(100);
      action = 10004;
      break;
    case 10004:

      lcd.setCursor(cursor, row);
      strncpy(mini_batch, input_str+12, 4);
      lcd.print(mini_batch);
      if (is_first) {
        first_line_cursor += 4;
      } else {
        second_line_cursor += 4;
      }
      wait(100);
      action = 19000; // HACK : 정리
      break;
    case 19000:
      // HACK : cleanup and return
      action = 0;
      if (stored_action > -1) {
        action = stored_action + 1; // 저장된 stored_action 다음으로.
        stored_action = -1;
      }
      break;

    default:
      // NOTE : 함수에서 호출하여, 아직 action이 없는것일수있음.
      if (stored_action > -1) {
        Println("ERROR : stored_action is not -1");
        return;
      }
      stored_action = action; // 현재 진행중인 작업 저장
      stored_is_first = is_first;
      *stored_string = "                ";

      bool is_available = ((16 - strlen(input_str)) > 0);
      if (is_available) {
        action = 10001; // NOTE : 10001 ~ 10010 : write action 
        strcpy(stored_string, input_str);
        Println("WRITE ACTION START");
        Println("ACTION 시작 가능!");
      } else {
        Print("ACTiON 시작 불가, 크기: ");
        Println(String(strlen(input_str)));
        action = stored_action;
        stored_action = -1;
      }


  }

}


void SimpleLCD::wait(unsigned long time) {
  waiting = true;
  wake_up_time = current_time + time;
  Println("WAITING FOR " + String(time) + "ms");
}



void SimpleLCD::show_info_animation() {
  switch(action) {
    case 0:
      lcd.setCursor(0, 0);
      write_action(true, "Coded by.");
      break;
    case 1:
      // Println("NEXT SHOW INFO");
      break;
    default:
      break;
  }
}

// NOTE : 5초에 한번 업데이트
void SimpleLCD::check(unsigned long now) {
  current_time = now;
  if (waiting) {
    if (now > wake_up_time) {
      waiting = false;
      wake_up_time = now;
    }
  } else {
    if (action < 1000) {
      // NOTE : action < 1000 :: onetime_animation 에게 할당함
      show_info_animation();
    } else if (action < 10000) {
      // NOTE : 정상 동작 ( 1000 ~ 9999 )

    } else if (action < 20000) {
      // NOTE : write or other ( 10001 ~ 19999)
      Println("WRITE ACTION RANGE");
      Println(String(action));
      write_action(stored_is_first, stored_string);
    } else {
      // HACK : ERROR -> GO TO 1000
      action = 1000;
    }
  }

  last_update = now;
}

#endif
