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
  TRISB0 = 0;

  // Main Routine
  buffer = "testing!\n";
  while(1) {
    /* print("testing!\n"); */
    print(buffer);


  }

  return;
}

void __interrupt() ISR(void) {



  return;
}
