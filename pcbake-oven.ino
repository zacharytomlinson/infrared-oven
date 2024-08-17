#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <Adafruit_MAX31865.h>
#include <QuickPID.h>
#include "./ReflowProfile.h"
#include "./SSR.h"

/******* THERMOCOUPLE VARIABLES ********/
#define MAX_RREF 430.0
#define MAX_RRES 100.0

/****** KEYPAD INPUTS *******/
enum class KeyInput 
{ 
  RIGHT,
  LEFT,
  UP,
  DOWN,
  SELECT,
  NONE 
};

/****** SCREEN ALIGNMENT LAMBDA ********/
auto center_float = [&](uint8_t _buflen) {
  return (16 >> 1) - (_buflen >> 1);
};

/***** PID AUTOTUNE VARIABLES *****/
byte ATuneModeRemember     = 2;
#define aTuneStep            1000
#define aTuneNoise           1
#define aTuneStartValue      1000;
unsigned int aTuneLookBack = 2;

/********* PID VARIABLES *********/
#define PID_REF_FRAME    500
#define PID_DEBOUNCE     50
float Output, Setpoint = 100, Kp = 2, Ki = 5, Kd = 1, temp = 0.0f, Input = 0.0f;
char * tst_state       = "Off";
int t_current          = 0;
unsigned long frameStartTime, nextSwitchTime;

/******** UI JUNK **********/
LiquidLine main_banner(center_float(11), 0, "PCBake Oven");
LiquidLine ver_banner(center_float(6), 1, "v1.0.1");
LiquidLine tst_banner(center_float(9), 0, "Test Mode");
LiquidLine conf_banner(0, 0, "Reflow Profile 1");

LiquidLine temp_line(0, 0, Input, " C");
LiquidLine time_line(0, 1, t_current, "s");
LiquidLine composite_line(0, 1, Input, " C ", t_current, "s");
LiquidLine test_line(0, 1, tst_state);

LiquidScreen splash_screen(main_banner, ver_banner);
LiquidScreen main_screen(time_line, temp_line);
LiquidScreen test_screen(tst_banner, test_line);
LiquidScreen profile_screen(conf_banner, composite_line);

/******** DEPENDENCY INITIALIZATION **********/
QuickPID oven = QuickPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, QuickPID::pMode::pOnError,
                         QuickPID::dMode::dOnMeas, QuickPID::iAwMode::iAwClamp, QuickPID::Action::direct);
Adafruit_MAX31865 thermo = Adafruit_MAX31865(3);
SSR relay = SSR(2);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
ReflowProfile reflow = ReflowProfile::ReflowProfile();
LiquidMenu menu(lcd, splash_screen, main_screen, test_screen, profile_screen);

void setup() {
  thermo.begin(MAX31865_3WIRE);
  lcd.begin(16, 2);
  menu.update();
  Serial.begin(57600);
  oven.SetOutputLimits(0, PID_REF_FRAME);
  oven.SetSampleTimeUs(PID_REF_FRAME * 1000);
  oven.SetMode(QuickPID::Control::automatic);
  delay(1500);
  menu.next_screen();
}

void loop() {
  t_current = millis() / 1000;
  Input = thermo.calculateTemperature(thermo.readRTD(), MAX_RRES, MAX_RREF);
  menu.update();

  switch(check_input()) {
    case KeyInput::RIGHT:
      menu.previous_screen();
      break;
    case KeyInput::LEFT:
      menu.next_screen();
      break;
    case KeyInput::SELECT:
      if(menu.get_currentScreen() == &test_screen) DispatchTest();
      if(menu.get_currentScreen() == &profile_screen) DispatchReflowCycle();
    default:
      break;
  }

  delay(100);
}

void DispatchTest() {
  int tst = 0;
  while (check_input() != KeyInput::DOWN) {
    if(tst % 5 == 0) {
      relay.toggle();
      if(relay.current_state()) {
        strcpy(tst_state, "On");
      } else {
        strcpy(tst_state, "Off");
      }
      menu.update();
    }
    tst++;
    delay(1000);
  }

  strcpy(tst_state, "Off");
  if(relay.current_state()) {
    relay.toggle();
  }
  menu.update();

  return;
}

void DispatchReflowCycle() {
  Phase * current_phase = reflow.get_phase(PhaseType::PREHEAT);
  Setpoint = current_phase->target_temp;
  int start_time = millis() / 1000, slope;
  frameStartTime = millis();
  bool pid_started = true;

  /*
  if(current_phase->target_metric.type == MetricType::DURATION) {
    if(current_phase->type == PhaseType::PREHEAT) {
      slope = (current_phase->target_temp - Input) / current_phase->target_metric.value.duration;
    } else {
      slope = (current_phase->target_temp - reflow.get_phase(current_phase->type - 1)->target_temp) / current_phase->target_metric.value.duration;
    }
  } else {
    slope = current_phase->target_metric.value.roc; 
  }
  */

  while (pid_started) {
    unsigned long now = millis();
    t_current = now / 1000 - start_time;
    Input = thermo.calculateTemperature(thermo.readRTD(), MAX_RRES, MAX_RREF); 
    menu.update();

    if (oven.Compute()) frameStartTime = now;
    if (!relay.current_state() && Output > (now - frameStartTime)) {
      if (now > nextSwitchTime) {
        nextSwitchTime = now + PID_DEBOUNCE;
        relay.toggle();
        Serial.println("OVEN ON");
      }
    } else if (relay.current_state() && Output < (now - frameStartTime)) {
      if (now > nextSwitchTime) {
        nextSwitchTime = now + PID_DEBOUNCE;
        relay.toggle();
        Serial.println("OVEN OFF");
      }
    }
  }
}

KeyInput check_input() {
  int val = analogRead(A0);
  if (val >= 0 && val <= 50) {
    return KeyInput::RIGHT;
  } else if (val >= 50 && val <= 150) {
    return KeyInput::UP;
  } else if (val >= 15 && val <= 300) {
    return KeyInput::DOWN;
  } else if (val >= 300 && val <= 500) {
    return KeyInput::LEFT;
  } else if (val >= 500 && val <= 750) {
    return KeyInput::SELECT;
  } else {
    return KeyInput::NONE;
  }
}
