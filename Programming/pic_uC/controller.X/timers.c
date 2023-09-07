/*
 * File:   timers.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:01 PM
 */


#include "includes.h"

void timer1_init(void) {

  // Read 16-bit values in one operation
  T1RD16 = 1;   

  // TMR1 Prescalar
  T1_PRESCALAR = T1_PRE_1;

  // TMR1 Clock Source -> Fosc/4
  TMR1CS = 0;

  // TMR1 Oscillator Shut off
  T1OSCEN = 0;

}

void timer1_ISR(void) {


  if ((++timer1_overflow_counter) == TARGET_TIMER1_OVERFLOW) {

    RB0 = !RB0;
    pid_execute();
    timer1_overflow_counter = 0;
    pid_report_shown = 0;

  }

  TMR1 = 5550;  // preloading value to 5ms

}

void timer2_init(void) {

  // Pre-scalar Value
  T2_PRESCALAR = T2_PRE_1;
  
  // Post-scalar Value
  T2_POSTSCALAR = T2_POST_1;

}
