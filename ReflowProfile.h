#ifndef ReflowProfile_h
#define ReflowProfile_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum PhaseType 
{
  PREHEAT,
  SOAK,
  REFLOW,
  HOLD,
  COOLING
};

enum MetricType
{
  DURATION,
  ROC
};

typedef union metric_val
{ 
  int duration; 
  int roc;
} Metric_val;

typedef struct metric {
    MetricType type;
    Metric_val value;
} Metric;

typedef struct Phase 
{
  PhaseType type;
  int target_temp;
  Metric target_metric;
};

class ReflowProfile {
  private:
  char * _name;
  Phase ** _phases;

  public:
  ReflowProfile();
  ReflowProfile(char*, Phase**);
  void set_phase(Phase);
  void set_current_phase(Phase);
  Phase * get_phase(PhaseType);
  Phase ** get_phases();
};

#endif