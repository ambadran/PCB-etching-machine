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
void pid_setpoint(float setpoint) {
  pid.setpoint = setpoint;
}

void pid_calc(void) {
 
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
