#include "ReflowProfile.h"
#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

ReflowProfile::ReflowProfile()  {
  _name = "Default";
  _phases = new Phase*[5];
  _phases[0] = &Phase { PhaseType::PREHEAT, 150, { MetricType::ROC, { .roc = 3 }}};
  _phases[1] = &Phase { PhaseType::SOAK, 200, { MetricType::DURATION, { .duration = 60 }}};
  _phases[2] = &Phase { PhaseType::REFLOW, 245, { MetricType::ROC, { .roc = 3 }}};
  _phases[3] = &Phase { PhaseType::HOLD, 245, { MetricType::DURATION, { .duration = 25 }}};
  _phases[4] = &Phase { PhaseType::COOLING, 50, { MetricType::ROC, { .roc = 3 }}};
}

ReflowProfile::ReflowProfile(char * name, Phase ** phases) {
  _name = name;
  _phases = phases;
}


Phase ** ReflowProfile::get_phases() {
  return _phases;
}


void ReflowProfile::set_phase(Phase phase) {
  for(int i = 0; i < 5; i++) {
    if(_phases[i]->type == phase.type) {
      _phases[i] = &phase;
      return;
    }
  }
}

Phase* ReflowProfile::get_phase(PhaseType type) {
  switch(type) {
    case PhaseType::PREHEAT:
      return ReflowProfile::_phases[0];
    case PhaseType::SOAK:
      return ReflowProfile::_phases[1];
    case PhaseType::REFLOW:
      return ReflowProfile::_phases[2];
    case PhaseType::HOLD:
      return ReflowProfile::_phases[3];
    case PhaseType::COOLING:
      return ReflowProfile::_phases[4];
  }
};