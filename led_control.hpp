#ifndef _LED_CONTROL_HPP
#define _LED_CONTROL_HPP
#include <Arduino.h>
#include "constants.hpp"
#include "global.hpp"

#define LED_COUNT 6

class SimpleLed {
private:
  unsigned long last_update;
  bool led_state_map[LED_COUNT] = {0,0,0,0,0,0};
  int target_state = 1;
  
public:
  void reverse(int);
  void check(unsigned long now);
};

void SimpleLed::reverse(int idx) {
  if (led_state_map[idx] == 1) {
    digitalWrite(LED_LIST[idx], LOW);
    led_state_map[idx] = 0;
  } else {
    digitalWrite(LED_LIST[idx], HIGH);
    led_state_map[idx] = 1;
  }
}

// TODO : 50ms 마다, LED를 순차적으로 켜고 끔.

void SimpleLed::check(unsigned long now) {
  // Println("LED CHECK");
  
  if (now - last_update > led_loop_time) {
    // Println("LED 변경");
    int target_state_count = 0;
    bool is_changed = false;
    for (int i = 0; i < LED_COUNT; i++) {
      if (led_state_map[i] != target_state) {
        if (!is_changed) {
          reverse(i);
          is_changed = true;
        }
      } else {
        target_state_count += 1;
      }
      if (target_state_count == LED_COUNT) {
        target_state = !target_state;
      }
    }
    last_update = now;

  }
}



#endif
