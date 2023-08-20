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

void print_str(char* message) {

  uint8_t ind = 0;
  while (message[ind] != '\0') {
    while(!TRMT);
    TXREG = message[ind];
    ind++;
  }

}

int intToASCII(int number) {
  return '0' + number;
}

unsigned divu10(unsigned n) {
    unsigned q, r;
    q = (n >> 1) + (n >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
//    q = q + (q >> 16);  // it doesn't support 16bits anyway
    q = q >> 3;
    r = n - (((q << 2) + q) << 1);
    return q + (r > 9);
}

void print_int(int value) {

  /* uint8_t digit5 = intToASCII(value%10); */
  /* value = divu10(value); */
  uint8_t digit4 = intToASCII(value%10);
  value = divu10(value);
  uint8_t digit3 = intToASCII(value%10);
  value = divu10(value);
  uint8_t digit2 = intToASCII(value%10);
  value = divu10(value);
  uint8_t digit1 = intToASCII(value%10);

  while(!TRMT);
  TXREG = digit1;
  while(!TRMT);
  TXREG = digit2;
  while(!TRMT);
  TXREG = digit3;
  while(!TRMT);
  TXREG = digit4;
  /* while(!TRMT); */
  /* TXREG = digit5; */
 
}

void print_double(double value) {



}

void print() {

}
