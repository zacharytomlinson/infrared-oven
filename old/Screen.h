#include <stdint.h>

#define Screen_h
#define UIFRAMES 5

const uint8_t DELTA[8] = {0b00000,0b00100,0b00100,0b01010,0b01010,0b10001,0b11111};
const uint8_t DEGREE[8] = {0b11100,0b10100,0b10100,0b11100,0b00000,0b00000,0b00000};
const uint8_t L_CARET[8] = {0b00000,0b00001,0b00011,0b00101,0b00011,0b00001,0b00000};
const uint8_t L_CARET_SEL[8] = {0b11111,0b11110,0b11100,0b11010,0b11100,0b11110,0b11111};
const uint8_t R_CARET[8] = {0b00000,0b10000,0b11000,0b10100,0b11000,0b10000,0b00000};
const uint8_t R_CARET_SEL[8] = {0b11111,0b01111,0b00111,0b01011,0b00111,0b01111,0b11111};

const uint8_t CHARACTER_SET[8][6] = {DELTA, DEGREE, L_CARET, L_CARET_SEL, R_CARET, R_CARET_SEL}; 

uint8_t screen_idx = 0;

enum class FloatBias { LEFT, RIGHT };

uint8_t center_float(uint8_t, FloatBias);


