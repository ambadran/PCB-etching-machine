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

  // Testing
  TRISB0 = 0;

  // Main Routine
  while(1) {

    /* print(buffer); */
    print_str("ADC_value: ");
    print_int(read_V());
    print_str("\n");

  }

  return;
}

void __interrupt() ISR(void) {



  return;
}
