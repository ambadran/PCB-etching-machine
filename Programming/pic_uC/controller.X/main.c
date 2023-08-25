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
  static char line[RX_BUFFER_SIZE];
  while(1) {

      

  }

  return;
}

void __interrupt() ISR(void) {

  if (RCIF) {

    uart_rx_ISR();
    RCIF = 0;

  }

  return;
}
