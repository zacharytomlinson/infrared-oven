#ifndef UI_h
#define UI_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum class FloatBias 
{ 
  LEFT,
  RIGHT 
};

uint8_t center_float(uint8_t _buflen, FloatBias _bias) {
  auto frame = [&]() {
    return (16 >> 1) - (_buflen >> 1);
  };
  if (_buflen % 2 > 0) {
    if (_bias == FloatBias::LEFT) return frame() - 1;
  }
  return frame();
};

auto cellular_row = [&](char ** content, uint8_t len) {
  uint8_t agg_len = 0;
  for(int i = 0; i < len; i++) agg_len += strlen(content[i]);
};
