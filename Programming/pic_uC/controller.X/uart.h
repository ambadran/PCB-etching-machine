
#ifndef UART_H
#define UART_H

#define INT_DIGITS 5
#define FLOAT_WHOLE_DIGITS 2
#define FLOAT_WHOLE_DECIMALS 3

char* buffer;

void uart_init(long int baudrate);
void uart_ISR();
void print_str(char *message);
unsigned divu10(unsigned n);
uint8_t uint8ToASCII(uint8_t number);
void print_int(int value);
void print_double(double value);
void print();

#endif
