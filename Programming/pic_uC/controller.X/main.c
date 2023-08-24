/*
 * File:   main.c
 * Author: ambadran717
 *
 * Created on August 12, 2023, 11:17 AM
 */


#include "includes.h"

void main(void) {

  // INITs/DEINITS
  uart_init(115200);
  adc_init();
  timer2_init();
  cpp1_init();

  // Testing
  TRISB0 = 0;

  cpp1_freq(1500);
  cpp1_duty_cycle(60);

  // Main Routine
  while(1) {

    /* print(buffer); */
    print_str("PR2: ");
    print_int(PR2);
    print_str("duty_cycle_constant: ");
    print_int(duty_cycle_constant);
    print_str("\n");

  }

  return;
}

void __interrupt() ISR(void) {



  return;
}
