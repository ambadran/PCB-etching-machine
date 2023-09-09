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

uint8_t pid_calc(float current_input) {
  
  // Calculating error terms
  float error = pid.setpoint - current_input;
  /* float d_input = */  //TODO: will be used in differential_on_measurement mode
  float d_error = error - pid._last_error;

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

  print_str("Last input: ");
  print_float(pid._last_input);
  print_char('\n');
  print_str("Last Error: ");
  print_float(pid._last_error);
  print_char('\n');
  print_str("Proportional: ");
  print_float(pid._proportional);
  print_char('\n');
  print_str("Integral: ");
  print_float(pid._integral);
  print_char('\n');
  print_str("Derivative: ");
  print_float(pid._derivative);
  print_str("\n\n");

}
