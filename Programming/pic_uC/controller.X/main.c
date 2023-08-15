/*
 * File:   main.c
 * Author: ambadran717
 *
 * Created on August 12, 2023, 11:17 AM
 */


#include "includes.h"

void main(void) {

  // INITs/DEINITS
  TRISB = 0x00;
  
  
  // Initial setups
  RB0 = 1;
  RB2 = 0;
  

  // Main Routine
  while(1) {
      RB1 = 1;
      __delay_ms(50);
      RB1 = 0;
      __delay_ms(50);
  }

  return;
}

void __interrupt() ISR(void) {



  return;
}
