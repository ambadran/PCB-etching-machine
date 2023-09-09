
#ifndef UART_H
#define UART_H

#define RX_BUFFER_SIZE 64
#define SERIAL_NO_DATA 0xFF

#define LINE_BUFFER_SIZE 80

#define MAX_INT_DIGITS 8  // maximum digits stored in uint32_t

typedef union {
  float float_;
  long int long_int;
} value_t;

void uart_init(long int baudrate);
void uart_rx_ISR(void);
uint8_t read_int(char *line, uint8_t *char_count, value_t *value_ptr);
/* uint8_t read_float(char *line, uint8_t *char_count, value_t *value_ptr); */ 
uint8_t read_float(char *line, uint8_t *char_count, value_t *value_ptr); 
unsigned char uart_read(void);

unsigned divu10(unsigned n);  //TODO: test if we really need this in PIC18F2550 or is the hardware multiplier enough
void print_bit(uint8_t bit_);
void print_char(unsigned char character);
void print_str(char *message);
void print_int(int value);
void print_float(float value);
void print();

#endif
