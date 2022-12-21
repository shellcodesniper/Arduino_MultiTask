#ifndef _DISP_CONTROL
#define _DISP_CONTROL
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "constants.hpp"

#define IS_TEST_MODE
// HACK : 디버깅 모드와 같은 의미임. 바로, 메인 시퀀스 진입 하도록

#ifdef IS_TEST_MODE
  #define START_ACTION 1000
#else
  #define START_ACTION 0
#endif

#define FIRST_LINE true
#define SECOND_LINE false

class SimpleLCD {
private:
  bool waiting = false;
  unsigned long wake_up_time;

  unsigned long last_update; // HACK : 마지막 업데이트 시각
  unsigned long current_time; // HACK : 지금시각
  int action = START_ACTION; // HACK :
  int stored_action = -1; // HACK : 저장된 액션 ( 기존으로 돌아가기 위함 )

  char first_line[17] = "                ";
  char second_line[17] = "                ";
  char led_status_str[16] = "LED_O O O O O O";
  char btn_status_str[16] = "BTN_[-:O] [+:O]";
  char cycle_period_str[16] = "             MS"; // 0 ~ 12

  bool stored_is_first = false;
  char stored_string[17] = "                ";

  unsigned int info_count = 0;

  void write_action(bool, char*);
  // NOTE: 10001 ~ 10010 : write action

  void draw_entire_line(bool);
  void remove_line(bool);
  void show_info_animation();
  void main_sequence();
  void wait(unsigned long time); // TYPE : 대기함수


public:
  void check(unsigned long now); // TYPE : 비동기 시뮬
};

void SimpleLCD::remove_line(bool is_first) {
  if (is_first) {
    for (int i = 0; i < 16; i++) {
      first_line[i] = ' ';
    }
  } else {
    for (int i = 0; i < 16; i++) {
      second_line[i] = ' ';
    }
  }
  draw_entire_line(is_first);
}

void SimpleLCD::draw_entire_line(bool is_first) {
  if (is_first) {
    lcd.setCursor(0, 0);
    for (int i = 0; i < 16; i++) {
      if (first_line[i] != 0) {
        lcd.print(first_line[i]);
      } else {
        lcd.print(" ");
      }
    }
  } else {
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++) {
      if (second_line[i] != 0) {
        lcd.print(second_line[i]);
      } else {
        lcd.print(" ");
      }
    }
  }
}

// NOTE : stored action 사용, 종료후 sored_action 으로 돌아감.
void SimpleLCD::write_action(bool is_first, char *input_str) { 
  uint8_t row = (is_first) ? 0 : 1;
  switch (action) {
    // NOTE : 4글자씩 끊어서 출력
    case 10001:
      strncpy((is_first ? first_line : second_line), input_str, 4);
      draw_entire_line(is_first);
      wait(100);
      action = 10002;
      break;
    case 10002:
      strncpy((is_first ? first_line : second_line), input_str, 8);
      draw_entire_line(is_first);
      wait(100);
      action = 10003;
      break;
    case 10003:
      strncpy((is_first ? first_line : second_line), input_str, 12);
      draw_entire_line(is_first);
      wait(100);
      action = 10004;
      break;
    case 10004:
      strncpy((is_first ? first_line : second_line), input_str, 17);
      lcd.printstr((is_first ? first_line : second_line));
      draw_entire_line(is_first);
      wait(100);
      action = 19000; // HACK : 정리
      break;
    case 19000:
      // HACK : cleanup and return
      action = 0;
      if (stored_action > -1) {
        action = stored_action + 1;
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
      char reset_line[17] = { ' ' * 16 };
      if (is_first) {
        strncpy(first_line, reset_line, 16); // NOTE : reset
      } else {
        strncpy(second_line, reset_line, 16); // NOTE : reset
      }

      bool is_available = ((16 - strlen(input_str)) > 0);
      if (is_available) {
        action = 10001; // NOTE : 10001 ~ 10010 : write action 
        strcpy(stored_string, input_str);
        // Println("WRITE ACTION START");
        // Println("ACTION 시작 가능!");
      } else {
        // Print("ACTiON 시작 불가, 크기: ");
        // Println(String(strlen(input_str)));
        action = stored_action;
        stored_action = -1;
      }


  }

}


void SimpleLCD::wait(unsigned long time) {
  current_time = millis();
  waiting = true;
  wake_up_time = current_time + time;
  // Println("WAITING FOR " + String(time) + "ms");
}



void SimpleLCD::show_info_animation() {
  switch(action) {
    case 0:
      action = 1;
      break;
    case 1:
      write_action(true, "BOOT UP ..");
      break;
    case 2:
      action = 3;
      wait(1000);
      break;
    case 3:
      write_action(FIRST_LINE, "Coded by.");
      break;
    case 4:
      action = 5;
      wait(1000);
      break;
    case 5:
      write_action(SECOND_LINE, "Dogyun, Kim");
      break;
    case 6:
      action = 7;
      wait(2000);
      // INFO : REMOVE LINE CODE
      break;
    case 7:
      remove_line(SECOND_LINE);
      write_action(FIRST_LINE, "github.com/");
      break;
    case 8:
      action = 9;
      wait(500);
      break;
    case 9:
      write_action(SECOND_LINE, "shellcodesniper");
      break;
    case 10:
      action = 11;
      wait(2000);
      // INFO : REMOVE LINE CODE
      break;
    case 11:
      remove_line(SECOND_LINE);
      write_action(FIRST_LINE, "STARTING...");
      break;
    case 12:
      action = 13;
      wait(500);
      break;
    case 13:
      write_action(FIRST_LINE, "MAIN_SEQUENCE..");
      break;
    case 14:
      action = 15;
      wait(2000);
      break;
    case 15:
      remove_line(FIRST_LINE);
      remove_line(SECOND_LINE);
      action = 17;
      wait(1000);

    default:
      Println("ERROR : show_info_animation() : action can not be " + String(action));
      action = 1000; // NOTE : GO TO MAIN SEQUENCE. loop over.
      break;
  }
}

void SimpleLCD::main_sequence() {
  // if (action == 1005) {
  //   Println("ERROR : main_sequence() : action can not be 1005");
  // }
  // switch (action) {
  //   case(1005):
  //     Println("SWITCH TEST");
  //     break;
  //   default:
  //     break;
  // }
  if (action == 1900) { // NOTE : 반복 제어문 똑 떼어옴.
    Println("INFO_COUNT : " + String(info_count));
    info_count += 1;
    action = 1002;
    if (info_count > 4) {
      action = 1002;
      info_count = 0;
    }
    if (info_count >= 2) {
      action = 1100; // NOTE : 현재 주기
    }
    // NOTE : 루프처럼 돌아가도록 해주는곳.
  }

  switch(action) {
    case 1000:
      // action = 1001;
      stored_action = -1;
      write_action(FIRST_LINE, "MAIN SEQUENCE");
      break;
    case 1001:
      action = 1002;
      wait(1000);
      break;
    case 1002:
      action = 1003;
      remove_line(FIRST_LINE);
      remove_line(SECOND_LINE);
      break;
    case 1003:
      action = 1004;
      wait(1000);
      break;

    // NOTE: LED STATUS O F O F O F
    case 1004:
      // NOTE : LED 0 : 6, 1 : 8 [6 + 2 * N번째]

      for(int i = 0; i < LED_COUNT; i++) {
        led_status_str[(4 + 2 * i)] = led_state_map[i] ? 'O' : 'X';
      }
      write_action(FIRST_LINE, led_status_str);
      break;

    case(1005):
      action = 1006;
      wait(10);
      break;

    case 1006:
      btn_status_str[7] = btn_state_map[0] ? 'O' : 'X';
      btn_status_str[13] = btn_state_map[1] ? 'O' : 'X';
      write_action(SECOND_LINE, btn_status_str);
      break;

    case 1007:
      action = 1900; // NOTE : 반복 제어문으로
      wait(100);
      break;

    case 1100:
      // TODO : 지우기.
      action = 1101;
      remove_line(FIRST_LINE);
      remove_line(SECOND_LINE);
      wait(100);
      break;

    case 1101:
      write_action(FIRST_LINE, "CYCLE PERIOD");
      break;

    case 1102:
      strncpy(cycle_period_str, CYCLE_RESET_STRING, 12);
      int idx = 12;
      unsigned int cycle_period = led_loop_time;
      while (cycle_period > 0) {
        cycle_period_str[idx] = (cycle_period % 10) + '0'; // HACK : ASCII CODE
        cycle_period /= 10;
        idx--;
      }
      write_action(SECOND_LINE, cycle_period_str);
      break;

    case 1103:
      action = 1900; // NOTE : 반복 제어문으로
      wait(100);
      break;




    default:
      Println("ERROR : main_sequence() : action can not be " + String(action));
      remove_line(FIRST_LINE);
      remove_line(SECOND_LINE);
      action = 1000;

      // NOTE : 이 main_sequence 는 루프처럼 동작할것임.
      // NOTE : 에러상황 발생시 action을 1000으로 초기화하고, 다시 시작.
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
    // Println("ACTION, STORED_ACTION" + String(action) + ", " + String(stored_action));
    if (action < 1000) {
      // NOTE : action < 1000 :: onetime_animation 에게 할당함
      show_info_animation();
    } else if (action < 10000) {
      // NOTE: 1000 ~ 9999 :: main_sequence 에게 할당함
      // Println("ACTION, STORED_ACTION" + String(action) + ", " + String(stored_action));
      main_sequence();
    } else if (action < 20000) {
      // NOTE : write or other ( 10001 ~ 19999)
      // Println("WRITE ACTION RANGE");
      // Println(String(action));
      write_action(stored_is_first, stored_string);
    } else {
      // HACK : ERROR -> GO TO 1000
      action = 1000;
    }
  }

  last_update = now;
}

#endif
