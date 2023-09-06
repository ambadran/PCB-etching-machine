
#ifndef PID_H
#define PID_H

#define PID_dt = TIMER1LKSDJFLKJ + CALC_FUNC_DELAY

#define CLAMP(value) ((value) <= (PID_MIN_OUTPUT) ? (PID_MIN_OUTPUT) : ( \
                      (value) >= (PID_MAX_OUTPUT) ? (PID_MAX_OUTPUT) : ( (value) )))

#define pid_execute() PID_OUTPUT_FUNC(pid_calc(PID_INPUT_FUNC()))

typedef struct {

  // User Inputs
  float setpoint;

  // Process variables
  float _proportional;
  float _integral;
  float _derivative;
  float _last_error;
  float _last_time;
  float _last_input;
  float _last_output;

} pid_t;

void pid_reset(void);
void pid_setpoint(float setpoint);
void pid_calc(void);
void pid_activate(void);
void pid_deactivate(void);

#endif
