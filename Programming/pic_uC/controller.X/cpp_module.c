/*
 * File:   cpp_module.c
 * Author: mr-atom
 *
 * Created on August 24, 2023, 12:35 PM
 */

#include "includes.h"

void cpp1_init(void) {

  // setting CCP module to PWM mode
  CCP1CONbits.CCP1M= 0b1100;
 
  // setting RC2 to be output
  CCP1_TRIS = 0;
  TRISC = 0x00;

}

void cpp1_freq(long int frequency) {

  long int frequency_value = (_XTAL_FREQ/(frequency*4*T2_PRE_ACTUAL[T2_PRE_1]) -1);
	if (frequency_value <= 255) {

    T2_PRESCALAR = T2_PRE_1;
    PR2 = (unsigned char)frequency_value;

    // computing new duty_cycle_constant for next duty_cycle change
    duty_cycle_constant = (_XTAL_FREQ/frequency/100/T2_PRE_ACTUAL[T2_PRE_1]); 

    return;

	}

	frequency_value = (_XTAL_FREQ/(frequency*4*T2_PRE_ACTUAL[T2_PRE_4]) -1);
  if (frequency_value <= 255) {

    T2_PRESCALAR = T2_PRE_4;
    PR2 = (unsigned char)frequency_value;

    // computing new duty_cycle_constant for next duty_cycle change
    duty_cycle_constant = (_XTAL_FREQ/frequency/100/T2_PRE_ACTUAL[T2_PRE_4]); 

    return;

	} 

	frequency_value = (_XTAL_FREQ/(frequency*4*T2_PRE_ACTUAL[T2_PRE_16]) -1);
  if (frequency_value <= 255) {

    T2_PRESCALAR = T2_PRE_16;
    PR2 = (unsigned char)frequency_value;

    // computing new duty_cycle_constant for next duty_cycle change
    duty_cycle_constant = (_XTAL_FREQ/frequency/100/T2_PRE_ACTUAL[T2_PRE_16]);

    return;

  } else {
    // Halting everything and sending error message
    GIE = 0;
    while(1) {
      print_str("Couldn't set PR2, frequency value at PRESCALE 16: ");
      print_int(frequency_value);
      print_char('\n');
    }
  }
 
}

void cpp1_duty_cycle(unsigned int duty_cycle) {

  // re-using the parameter variable to be the duty_cycle value
  duty_cycle *= duty_cycle_constant;

  // setting the duty_cycle
  DC1B1 = (duty_cycle) & 1;
  DC1B0 = (duty_cycle) & 2;
  CCPR1L = (duty_cycle) >> 2;

}
