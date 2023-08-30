#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_INT_DIGITS 8

uint8_t read_float(char *line, uint8_t *char_count, float *float_ptr) {

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
    *float_ptr = -fval;
  } else {
    *float_ptr = fval;
  }

  *char_count = ptr - line - 1; // Set char_counter to next statement
 
  return (true);
}

int main() {

  char line[40] = "E83.542";
  uint8_t char_count = 1; // already skipped the first commannd letter
  float value;

  printf("Function Success: %d\n", read_float(line, &char_count, &value));
  printf("Float: %3f\n", value);

  return 0;
}
