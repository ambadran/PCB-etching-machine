/*
 * File:   main.c
 * Author: ambadran717
 *
 * Created on August 12, 2023, 11:17 AM
 */

#include "includes.h"

void main(void) {

  // initial delay for voltage to stabilize
  /* GIE = 0; */

  // INITs/DEINITS
  interrupt_init();
  comparator_deinit();
  uart_init(UART_BAUDRATE);
  adc_init();
  timer1_init();
  timer2_init();
  cpp1_init();
  heater_init();

  // Testing
  TRISB0 = 0;
  TRISB1 = 0;

  // Informing User System started
  print_str("System Started..");

  // Main Routine
  protocol_main_loop();
  return;

}

void __interrupt() ISR(void) {

  if (OSCFIF) {
    print_str("OSCIF");
    OSCFIF = 1;
  }

  if (HLVDIF) {
    print_str("HLVDIF");
    HLVDIF = 1;
  }

  if (BCLIF) {
    print_str("BCLIF");
    BCLIF = 1;
  }

  if (RCIF) {
    uart_rx_ISR();
    RCIF = 0;
  }

  if (TMR1IF) {
    timer1_ISR();
    TMR1IF = 0;
  }

  // to identify a rogue interrupt source!
  /* print_char('F'); */
  return;
}
