/*
 * File:   pid.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:02 PM
 */


#include "includes.h"

pid_t pid;

void pid_reset(void) {
  // Zero Out all values
  memset(&pid, 0.0, sizeof(pid_t)); 

  pid._integral = CLAMP(0.0);
}

//TODO: try doing it in the terminal directly
void pid_setpoint(float setpoint) { pid.setpoint = setpoint; }

double pid_calc(double current_input) {
  
  // Calculating error terms
  double error = pid.setpoint - current_input;
  /* float d_input = */  //TODO: will be used in differential_on_measurement mode
  double d_error = error - pid._last_error;

  // Calculating proportional term
  pid._proportional = Kp*error;

  // Calculating integral term
  pid._integral += Ki*error*PID_dt;
  pid._integral = CLAMP(pid._integral);  // avoid integral windup

  // Calculating derivative term
  pid._derivative = Kd*d_error*PID_dt_inv;

  // Saving current values for next iteration
  pid._last_error = error;
  pid._last_input = current_input;

  return CLAMP(pid._proportional+pid._integral+pid._derivative);

}

void pid_activate(void) {

  // Resetting values
  pid_reset();

  // Turning TMR1 on and starting interrupts calling pid_execute
  TMR1ON = 1;

}

void pid_deactivate(void) {

  // Turing TMR1 off and stopping interrupts calling pid_execute
  TMR1ON = 0;

  // Turning Power Output Off
  PID_OUTPUT_FUNC(0);
}

void pid_report(void) {

  print_str("Current PID values: ");
  print_char('\n');
  print_double(pid._last_input);
  print_char('\n');
  print_double(pid._proportional);
  print_char('\n');
  print_double(pid._integral);
  print_char('\n');
  print_double(pid._derivative);
  print_char('\n');

}
