/*
 * File:   cpp_module.c
 * Author: mr-atom
 *
 * Created on August 24, 2023, 12:35 PM
 */

/*
 * File:   cpp.c
 * Author: ambadran717
 *
 * Created on August 12, 2023, 11:17 AM
 */

#include "includes.h"

void cpp1_init() {

  // setting CCP module to PWM mode
  CCP1CONbits.CCP1M= 0b1100;
 
  // setting RC2 to be output
  CCP1_TRIS = 0;

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

	}
 
}

void cpp1_duty_cycle(unsigned int duty_cycle) {

  // re-using the parameter variable to be the duty_cycle value
  duty_cycle *= duty_cycle_constant;

  // setting the duty_cycle
  CCP1X = (duty_cycle) & 1;
  CCP1Y = (duty_cycle) & 2;
  CCPR1L = (duty_cycle) >> 2;

}
