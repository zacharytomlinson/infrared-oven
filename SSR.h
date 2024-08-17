#ifndef SSR_h
#define SSR_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SSR {
  private:
  bool state;
  uint8_t GPIO_pin;

  public:
  SSR(int8_t);
  bool toggle();
  bool current_state();
  void set_state(bool);
};

#endif