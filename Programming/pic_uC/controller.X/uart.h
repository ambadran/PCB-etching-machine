
#ifndef UART_H
#define UART_H

#define INT_DIGITS 5
#define FLOAT_WHOLE_DIGITS 2
#define FLOAT_WHOLE_DECIMALS 3

#define RX_BUFFER_SIZE 64
#define SERIAL_NO_DATA 0xFF

#define LINE_BUFFER_SIZE 80

void uart_init(long int baudrate);
void uart_rx_ISR();
unsigned char uart_read();

void print_char(unsigned char character);
void print_str(char *message);
unsigned divu10(unsigned n);
uint8_t uint8ToASCII(uint8_t number);
void print_int(int value);
void print_double(double value);
void print();

#endif
