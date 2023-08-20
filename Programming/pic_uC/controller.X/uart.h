
#ifndef UART_H
#define UART_H

char* buffer;

void uart_init(long int baudrate);
void uart_ISR();
void print_str(char *message);
int intToASCII(int number);
unsigned divu10(unsigned n);
void print_int(int value);
void print();

#endif
