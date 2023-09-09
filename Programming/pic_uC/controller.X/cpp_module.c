/*
 * File:   cpp_module.c
 * Author: mr-atom
 *
 * Created on August 24, 2023, 12:35 PM
 */

#include "includes.h"

float duty_cycle_constant;

void cpp1_init(void) {

  // setting CCP module to PWM mode
  CCP1CONbits.CCP1M= 0b1100;
 
  // setting RC2 to be output
  CCP1_TRIS = 0;
  TRISC2 = 0;

  // Timer2 ON
  TMR2ON = 1;

}

void cpp1_freq(long int frequency) {

  // Resetting duty cycle to Zero, as change in frequency means duty_cycle_constant will change
  // Thus, if left unchanged the new PWM with new frequency will give a very wrong duty cycle value
  cpp1_duty_cycle(0);

  long int frequency_value;
  uint8_t t2_prescalar_value;
  for(t2_prescalar_value=0; t2_prescalar_value<T2_PRESCALAR_MODES_NUMBER; t2_prescalar_value++) {

    frequency_value = (_XTAL_FREQ/(frequency*4*T2_PRE_ACTUAL[t2_prescalar_value]) -1);

    if (frequency_value <= 255) {

      T2_PRESCALAR = (unsigned char)t2_prescalar_value;
      PR2 = (unsigned char)frequency_value;

      // computing new duty_cycle_constant for next duty_cycle change
      duty_cycle_constant = (_XTAL_FREQ/100.0/(float)frequency/(float)T2_PRE_ACTUAL[t2_prescalar_value]); 

      return;
    
    }
  }

  // Halting everything and sending error message
  GIE = 0;
  while(1) {
    print_str("Couldn't set PR2, frequency value at PRESCALE 16: ");
    print_int((int)frequency_value);
    print_char('\n');
  }
 
}

void cpp1_duty_cycle(float duty_cycle) {
  // NOTE: I regular try to optimize this function and make duty_cycle int8_t 
  // PLEASE REMEMBER THE duty_cycle variable is also used to get assigned to 
  // the duty_cycle registers, which means no extra variable has to be created!
  // I HOPE I DON'T FORGET AGAIN ;)
  //
  // NOW, I have completely changed to a float ;)
  // because it needs to be multiplied by a float!

  // re-using the parameter variable to be the duty_cycle value
  duty_cycle *= duty_cycle_constant;

  // setting the duty_cycle
  DC1B0 = ((uint16_t)duty_cycle) & 1;
  DC1B1 = ((uint16_t)duty_cycle) & 2;
  CCPR1L = ((uint16_t)duty_cycle) >> 2;

}
