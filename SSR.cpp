#include "SSR.h"
#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

SSR::SSR(int8_t gpio_pin) { 
  SSR::GPIO_pin = gpio_pin;
  pinMode(gpio_pin, OUTPUT);
}

bool SSR::toggle() {
  digitalWrite(SSR::GPIO_pin, !digitalRead(SSR::GPIO_pin));
  return current_state();
}

bool SSR::current_state() {
  return digitalRead(SSR::GPIO_pin);
}

void SSR::set_state(bool state) {
  digitalWrite(SSR::GPIO_pin, state);
}