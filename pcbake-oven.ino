/*
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * Button pin to analog pin A0

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <Adafruit_MAX31865.h>
#include "./pcbake-oven.h"

float temp = 0.0f;
int dtime = 0;

// pass in the CS pin
Adafruit_MAX31865 thermo = Adafruit_MAX31865(3);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/************ Main header *************/
//LiquidLine header(0, 0, "   PCBake Oven   ");
/************ Info lines **************/
//LiquidLine version_banner(0, 1, "<    ", VERSION, "    >");
LiquidLine temp_banner(0, 1, temp + " C");
LiquidLine timeline(0, 0, dtime + "s")
/************ Menu screens ************/
LiquidScreen main_screen(timeline, temp_banner);
/************ Main menu ***************/
LiquidMenu menu(lcd, main_screen);

void setup() {
  thermo.begin(MAX31865_3WIRE);
  lcd.begin(16, 2);
  menu.update();
  Serial.begin(9600);
}

void loop() {
  dtime = millis() / 1000;
  temp = thermo.temperature(RRES, RREF);
  menu.update();
  delay(100);
}

/*
void log_fault() {
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    thermo.clearFault();
  }
}

KeyInput check_input() {
  int val = analogRead(A0);

  if (val >= 0 && val <= 50) {
    lcd.setCursor(15, 1);
    lcd.write((byte)5);
    delay(250);
    lcd.setCursor(15, 1);
    lcd.write((byte)4);
    swap_screens(KeyInput::RIGHT);
    screen_idx++;
    return KeyInput::RIGHT;
  } else if (val >= 50 && val <= 150) {
    return KeyInput::UP;
  } else if (val >= 15 && val <= 300) {
    return KeyInput::DOWN;
  } else if (val >= 300 && val <= 500) {
    lcd.setCursor(0, 1);
    lcd.write((byte)3);
    delay(250);
    lcd.setCursor(0, 1);
    lcd.write((byte)2);
    swap_screens(KeyInput::LEFT);
    screen_idx--;
    return KeyInput::LEFT;
  } else if (val >= 500 && val <= 750) {
    return KeyInput::SELECT;
  }
}

void swap_screens(KeyInput dir) {
  for(uint8_t i = 0; i < 16; i++) {
    dir == KeyInput::RIGHT ? lcd.scrollDisplayLeft() : lcd.scrollDisplayRight();
    delay(25);
  }
}
*/
