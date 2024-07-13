#include "Screen.h"
#include <stdint.h>

uint8_t center_float(uint8_t _buflen, FloatBias _bias) {
  auto frame = [&]() { return (16>>1)-(_buflen>>1); };
  if(_buflen%2>0) {
    if(_bias == FloatBias::LEFT) return frame()-1;
    return frame()+1;
  }
  return frame();
};