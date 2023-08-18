/*
 * File:   uart.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:02 PM
 */

#include "includes.h"


void uart_init(long int baudrate) {

  // Module SFR setup
  TX9 = 0; // 8-bit transmission
  RX9 = 0; // 8-bit receiving
  SYNC = 0; // Asynchronous mode
  TXEN = 1;
  SPEN = 1; // serial port enabled
  CREN = 1; // enable continous receiving 
  FERR = 0; // no framing error
  OERR = 0; // no overrun error

  // Baudrate setup
  BRGH = 1;
  SPBRG = (uint8_t)(_XTAL_FREQ/baudrate)/16; 

  // Tris register setup
  TX_TRIS = 1;
  RX_TRIS = 1;

}

void uart_ISR() {

}

void print(char* message) {

  uint8_t ind = 0;
  while (message[ind] != '\0') {
    while(!TRMT);
    TXREG = message[ind];
    ind++;
    RB0 = 1;
    __delay_ms(50);
    RB0 = 0;
    __delay_ms(50);

  }

  /* while(!TRMT); */
  /* TXREG = 'a'; */
  /* __delay_ms(50); */

}


