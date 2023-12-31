/*
 * File:   uart.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:02 PM
 */

#include "includes.h"

// uart receiver data buffer
unsigned char uart_rx_buffer[RX_BUFFER_SIZE];

// head index, points to new data
uint8_t uart_rx_buffer_head = 0;

// tail index, points to read data
uint8_t uart_rx_buffer_tail = 0;  //TODO: understand why it's volatile in grbl

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

// Writing character to buffer
void uart_rx_ISR(void) {

  // reading data
  unsigned char data = RCREG;

  // creating new head index to assign to uart_rx_buffer_head after using it
  uint8_t next_head = uart_rx_buffer_head + 1;
  if (next_head == RX_BUFFER_SIZE) { next_head = 0; }

  // writing data to buffer unless it is full
  if (next_head != uart_rx_buffer_tail) {
    uart_rx_buffer[uart_rx_buffer_head] = data;
    uart_rx_buffer_head = next_head;
  } //TODO: else alarm an overflow somehow

}

unsigned char uart_read(void) {

  // if head pointer == tail pointer, no new data not read :)
  if (uart_rx_buffer_head == uart_rx_buffer_tail) {

    return SERIAL_NO_DATA;

  } else {

    // reading unread data in buffer
    unsigned char data = uart_rx_buffer[uart_rx_buffer_tail];

    // updating tail pointer value
    uart_rx_buffer_tail++;
    if (uart_rx_buffer_tail == RX_BUFFER_SIZE) { uart_rx_buffer_tail = 0; }

    return data;

  }

}

void print_char(unsigned char character) {

  while(!TRMT);
  TXREG = character;

}

void print_str(char* message) {

  uint8_t ind = 0;
  while (message[ind] != '\0') {
    while(!TRMT);
    TXREG = message[ind];
    ind++;
  }

}

uint8_t uint8ToASCII(uint8_t number) {
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

  uint8_t* digits[INT_DIGITS];
  int8_t i;

  digits[INT_DIGITS-1] = uint8ToASCII(value%10);

  for(i=INT_DIGITS-2 ; i>=0 ; i--) {
    value = divu10(value);
    digits[i] = uint8ToASCII(value%10);
  }

  for(i=0; i<INT_DIGITS; i++) {
    while(!TRMT);
    TXREG = digits[i];
  }

}

void print_double(double value) {

  // getting the whole number stuff
  int tmp = (int)value;
  uint8_t* digits[FLOAT_WHOLE_DIGITS];
  int8_t i;

  digits[FLOAT_WHOLE_DIGITS-1] = uint8ToASCII(tmp%10);

  for(i=FLOAT_WHOLE_DIGITS-2 ; i>=0 ; i--) {
    tmp = divu10(tmp);
    digits[i] = uint8ToASCII(tmp%10);
  }

  // sending whole number
  for(i=0; i<FLOAT_WHOLE_DIGITS; i++) {
    while(!TRMT);
    TXREG = digits[i];
  }

  // sending decimal point '.'
  while(!TRMT);
  TXREG = '.';

  // getting and sending the decimal number stuff
  // since no need to inverse digit order :)
  for (i=0; i<FLOAT_WHOLE_DECIMALS; i++) {
    /* value = mulu10(value); */
    value *= 10;
    while(!TRMT);
    TXREG = uint8ToASCII((int)value%10);
  }

}

void print() {

}
