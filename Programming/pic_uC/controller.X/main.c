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
  TRISB0 = 0;

  // Main Routine
  while(1) {

    /* print(buffer); */
    print("Number: ");
    print_int(234);
    print("\n");
    __delay_ms(50);

    RB0 = 1;
    __delay_ms(10);
    RB0 = 0;
    __delay_ms(10);

  }

  return;
}

void __interrupt() ISR(void) {



  return;
}
