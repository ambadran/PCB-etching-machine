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


void uart_init(int32_t baudrate) {

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
  ABDEN = 0;
  WUE = 0;
  RXDTP = 0;
  RXDTP = 0;
  BRG16 = 1;
  BRGH = 0; //TODO: try  making it 4 and make the equation /4 instead of /16
  SPBRG = (uint8_t)(((_XTAL_FREQ/baudrate)/16) - 1); 
  /* SPBRG = 11; // baudrate 250000 */

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

// reads int only not floats
uint8_t read_int(char *line, uint8_t *char_count, value_t *value_ptr) {

  char *ptr = line + *char_count; // the variable that will extract the next digit from
  unsigned char c; // the variable that will hold each digit

  c = *ptr++;  // extracting the next digit!

  // catching negative values :)
  bool isnegative = false;
  if (c == '-') {
    isnegative = true;
    c = *ptr++;

  // incrementing pointer anyway if +ve sign is there
  } else if (c == '+') {
    c = *ptr++;
  }

  // Algorithm to convert char digits to int
  uint32_t intval = 0;
  uint8_t ndigit = 0;  // current digit read index
  int8_t exp = 0;  // NOT uint8_t as this must hold +ve and -ve numbers
  while (1) {

    c -= '0';
    if (c <= 9) {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) {

        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
                                                      //
      } else {

        exp++;  // Drop overflow digits
      }

    } else {
      // encountered a non integer character
      break;
    }
    c = *ptr++;
  }

  // Return if no digits have been read.
  if (!ndigit) { return(false); };

  long int lival;
  lival = (long int)intval;
  
  if (exp > 0) {
    do {
      lival *= 10.0;
    } while (--exp > 0);
  } 

  // Assign floating point value with correct sign.    
  if (isnegative) {
    value_ptr->long_int = -lival;
  } else {
    value_ptr->long_int = lival;
  }

  *char_count = ptr - line - 1; // Set char_counter to next statement
 
  return (true);
}

// arguments are the line of terminal code itself
// char_count_ptr to increment it as we read and be able to use it later in the terminal_execute_line after this func is done
// float_ptr to assign the output of this function to it
// THIS FUNCTION RETURN WHETHER IT SUCCESSFULYY READ THE FLOAT OR NOT, AKA (0 OR 1) 
uint8_t read_float(char *line, uint8_t *char_count, value_t *value_ptr) {

  char *ptr = line + *char_count; // the variable that will extract the next digit from
  unsigned char c; // the variable that will hold each digit

  c = *ptr++;  // extracting the next digit!

  // catching negative values :)
  bool isnegative = false;
  if (c == '-') {
    isnegative = true;
    c = *ptr++;

  // incrementing pointer anyway if +ve sign is there
  } else if (c == '+') {
    c = *ptr++;
  }

  // extracting ALL digits from line until a non-digit character is encountered to variable intval
  // variable exp will hold value x (in *10^x) which will be applied to intval after it's extracted
  // if x is negative, then the value is a float and that decimals will appear in the number 
  // if x is positive, then the value is has digits more than MAX_INT_DIGITS and this will mean
  // that after multiplication all digits after MAX_INT_DIGITS are just zeros
  // exp will be decremented with every digit registerd if '.' is encountered
  // exp will be incremented if ndigit > MAX_INT_DIGITS and still no '.' encountered
  uint32_t intval = 0;
  int8_t exp = 0;  // NOT uint8_t as this must hold +ve and -ve numbers
  uint8_t ndigit = 0;  // current digit read index
  bool isdecimal =  false;  // to be set if '.' is encountered
  while (1) {

    c -= '0';  // converting ascii number of digit to the digit itself >:)
    if (c <= 9) {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) {
        if (isdecimal) {
          exp--; 
        }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
                                                      //
      } else {
        if (!(isdecimal)) { 
          exp++;  // Drop overflow digits
        }
      }
    } else if (c == (('.'-'0') & 0xff)  &&  !(isdecimal)) {
      isdecimal = true;

    } else {
      break;

    }
    c = *ptr++;
  }
   
  // Return if no digits have been read.
  if (!ndigit) { return(false); };
  
  // Convert integer into floating point.
  float fval;
  fval = (float)intval;
  
  // Apply decimal. Should perform no more than two floating point multiplications for the
  // expected range of E0 to E-4.
  if (fval != 0) {
    while (exp <= -2) {
      fval *= 0.01; 
      exp += 2;
    }
    if (exp < 0) { 
      fval *= 0.1; 
    } else if (exp > 0) {
      do {
        fval *= 10.0;
      } while (--exp > 0);
    } 
  }

  // Assign floating point value with correct sign.    
  if (isnegative) {
    value_ptr->float_ = -fval;
  } else {
    value_ptr->float_ = fval;
  }

  *char_count = ptr - line - 1; // Set char_counter to next statement
 
  return (true);
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



//TODO: test if we really need this in PIC18F2550 or is the hardware multiplier enough
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

void print_bit(uint8_t bit_) {

  while(!TRMT);
  TXREG = bit_ + '0';

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

void print_int(int value) {

  uint8_t* digits[INT_DIGITS];
  int8_t i;

  // dealing with negative numbers
  if (value < 0) {

    // sending negative sign
    while(!TRMT);
    TXREG = '-';

    // inverting the value to deal with it
    value = -value;
  }

  digits[INT_DIGITS-1] = ((value%10) + '0');

  for(i=INT_DIGITS-2 ; i>=0 ; i--) {
    value = divu10(value);
    digits[i] = ((value%10) + '0');
  }

  for(i=0; i<INT_DIGITS; i++) {
    while(!TRMT);
    TXREG = digits[i];
  }

}

void print_float(float value) {

  // getting the whole number stuff
  /* int tmp = (int)value; */ //TODO: test if this method is better or the library function is better
  float tmp2;
  value = modff(value, &tmp2);  // assigning whole number to tmp and decimal to value 
  int tmp = (int)tmp2;
  uint8_t* digits[FLOAT_WHOLE_DIGITS];  // because we need to invert the order when printing
  int8_t i;  // for loops

  // dealing with negative numbers first
  if (signbit(value)) {
    while(!TRMT);
    TXREG = '-';

    // inverting value to deal with it
    tmp = -tmp;
    value = -value;
  }

  digits[FLOAT_WHOLE_DIGITS-1] = ((tmp%10) + '0');

  for(i=FLOAT_WHOLE_DIGITS-2 ; i>=0 ; i--) {
    tmp = divu10(tmp);
    digits[i] = ((tmp%10) + '0');
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
  /* value = value - truncf(value); */  //TODO: read above todo
  for (i=0; i<FLOAT_WHOLE_DECIMALS; i++) {
    /* value = mulu10(value); */
    value *= 10;
    while(!TRMT);
    TXREG = (((int)value%10) + '0');
  }

}

void print() {

}

