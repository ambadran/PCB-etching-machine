/*
 * File:   interrupts.c
 * Author: mr-atom
 *
 * Created on August 26, 2023, 1:44 PM
 */



#include "includes.h"

void interrupt_init(void) {

  // Global Interrupt Enable
  GIE = 1;

  // Peripheral Interrupt Enable
  PEIE = 1;

  // UART Recieve interrupt Enable
  RCIE = 1;
  TXIE = 0;

  // Timers
  TMR0IE = 0;
  TMR1IE = 0;
  TMR2IE = 0;
  TMR3IE = 0;

  // USB Module
  USBIE = 0;

  // IRQ pins
  INT0IE = 0;
  INT1IE = 0;
  INT2IE = 0;

  // CCP
  CCP1IE = 0;
  CCP2IE = 0;

  // AD converter
  ADIE = 0;

  // Parallel streaming
  SSPIE = 0;

  // RB port 
  RBIE = 0;   

  // comparator module
  CMIE = 0;

  // EEPROM
  EEIE = 0;

}




