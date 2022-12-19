#ifndef _IR_CONTROL
#define _IR_CONTROL
#include <Arduino.h>
#include <IRremote.h>
#include "constants.hpp"

class SimpleIR {
private:
  unsigned long last_update;

public:
  void init();
  void check(unsigned long now);
};

void SimpleIR::init() {
  IrReceiver.begin(IR_PIN, DISABLE_LED_FEEDBACK);
}

void SimpleIR::check(unsigned long now) {
  if (IrReceiver.decode()) {
    Println(IrReceiver.decodedIRData.decodedRawData, HEX);
    IrReceiver.resume();
  }
  // Println("IR CHECK");
}

#endif
