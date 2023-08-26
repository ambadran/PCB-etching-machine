/*
 * File:   interrupts.c
 * Author: mr-atom
 *
 * Created on August 26, 2023, 1:44 PM
 */



#include "includes.h"

void interrupt_init() {

  // Global Interrupt Enable
  GIE = 1;

  // Peripheral Interrupt Enable
  PEIE = 1;

  // UART Recieve interrupt Enable
  RCIE = 1;

}
