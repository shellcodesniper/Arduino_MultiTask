#ifndef _BTN_CONTROL
#define _BTN_CONTROL
#include <Arduino.h>
#include "constants.hpp"
#include "global.hpp"

class SimpleBTN {
private:
  unsigned long yellow_last_update;
  bool btn_yellow_state = 0; // TYPE : Acts -

  unsigned long orange_last_update;
  bool btn_orange_state = 0; // TYPE : Acts +

  void press_action(int);
  bool is_pressed(int pin);

public:
  void init();
  void check(unsigned long now);
};

void SimpleBTN::init() {
}

 

bool SimpleBTN::is_pressed(int pin) {
  return digitalRead(pin) == HIGH;
}

void SimpleBTN::press_action(int action_ammount) {
  Println(String(led_loop_time) + "CHANGED TO" + String(led_loop_time + (action_ammount * 10)));
  if (((int)led_loop_time + (int)(action_ammount * 10)) > 0) {
    led_loop_time += (action_ammount * 10);
  }
}

// TODO : 최소 50ms 이상 이전에 눌렸을경우, + - 증가. (LED 속도)
void SimpleBTN::check(unsigned long now) {
  // Println("BTN CHECK");

  bool yellow_stat = is_pressed(BTN_YELLOW);
  bool orange_stat = is_pressed(BTN_ORANGE);


  if (yellow_stat != btn_yellow_state) {
    if (yellow_stat) { // NOTE : PRESSED
      if (now - yellow_last_update > 50) {
        Println("YELLOW PRESSED");
        yellow_last_update = now;
        btn_yellow_state = yellow_stat;
        press_action(-1);
      }
    } else {
      Println("YELLOW RELEASED");
      yellow_last_update = now;
      btn_yellow_state = yellow_stat;
    }
  }

  if (orange_stat != btn_orange_state) {
    if (orange_stat) { // NOTE : PRESSED
      if (now - orange_last_update > 50) {
        Println("ORANGE PRESSED");
        press_action(+1);
        orange_last_update = now;
        btn_orange_state = orange_stat;
      }
    } else {
      Println("ORANGE RELEASED");
      orange_last_update = now;
      btn_orange_state = orange_stat;
    }
  }

  btn_state_map[0] = btn_yellow_state;
  btn_state_map[1] = btn_orange_state;
}

#endif

