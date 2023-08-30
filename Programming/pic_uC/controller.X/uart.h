
#ifndef UART_H
#define UART_H

#define INT_DIGITS 5
#define FLOAT_WHOLE_DIGITS 3
#define FLOAT_WHOLE_DECIMALS 3

#define RX_BUFFER_SIZE 64
#define SERIAL_NO_DATA 0xFF

#define LINE_BUFFER_SIZE 80


#define MAX_INT_DIGITS 8  // maximum digits stored in uint32_t

void uart_init(long int baudrate);
void uart_rx_ISR(void);


// reads int only not floats
uint8_t read_int(char *line, uint8_t *char_count, float *float_ptr);

// arguments are the line of terminal code itself
// char_count_ptr to increment it as we read and be able to use it later in the terminal_execute_line after this func is done
// float_ptr to assign the output of this function to it
// THIS FUNCTION RETURN WHETHER IT SUCCESSFULYY READ THE FLOAT OR NOT, AKA (0 OR 1) 
uint8_t read_float(char *line, uint8_t *char_count, float *float_ptr); 

unsigned char uart_read(void);

void print_char(unsigned char character);
void print_str(char *message);
unsigned divu10(unsigned n);  //TODO: test if we really need this in PIC18F2550 or is the hardware multiplier enough
uint8_t uint8ToASCII(uint8_t number);
void print_int(int value);
void print_double(double value);
void print();

#endif
