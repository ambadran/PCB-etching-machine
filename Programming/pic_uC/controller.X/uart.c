/*
 * File:   uart.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:02 PM
 */

#include "includes.h"

void uart_init(int baudrate) {

  TX9 = 0; // 8-bit transmission
  RX9 = 0; // 8-bit receiving
  SYNC = 0; // Asynchronous mode
  TXEN = 1;
  SPEN = 1; // serial port enabled
  CREN = 1; // enable continous receiving 
  FERR = 0; // no framing error
  OERR = 0; // no overrun error

  BRGH = 1;
  SPBRG = (uint8_t)(_XTAL_FREQ/baud_rate)/16; 

}


