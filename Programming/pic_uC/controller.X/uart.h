
#ifndef UART_H
#define UART_H

char* buffer;

void uart_init(long int baudrate);
void uart_ISR();
void print(char *message);

#endif
