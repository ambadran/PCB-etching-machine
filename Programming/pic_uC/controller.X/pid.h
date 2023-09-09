
#ifndef PID_H
#define PID_H

#define PID_dt TIMER1_M_OVERFLOW_MS
#define PID_dt_inv (1/PID_dt)

#define CLAMP(value) (((uint8_t)value) <= (PID_MIN_OUTPUT) ? (PID_MIN_OUTPUT) : ( \
                      ((uint8_t)value) >= (PID_MAX_OUTPUT) ? (PID_MAX_OUTPUT) : ( (value) )))

#define pid_execute() PID_OUTPUT_FUNC(pid_calc(PID_INPUT_FUNC()))

typedef struct {

  // User Inputs
  float setpoint;

  // Process variables
  float _proportional;
  float _integral;
  float _derivative;
  float _last_error;
  float _last_input;

} pid_t;

uint8_t pid_execute_flag = 0;
uint8_t pid_report_show = 0;
uint8_t pid_report_shown = 1;

void pid_reset(void);
void pid_setpoint(float setpoint);
uint8_t pid_calc(float current_input);
void pid_activate(void);
void pid_deactivate(void);
void pid_report(void);

#endif
