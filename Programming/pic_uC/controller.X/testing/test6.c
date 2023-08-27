#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


uint8_t read_float(char *line, uint8_t *char_count_ptr, float *float_ptr) {

  char *ptr = line + *char_count_ptr; // the variable that will extract the next digit from
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

    // Converting 
    c -= '0';  // 
  }

  return (true);

}

int main() {

  char line[40] = "E83.542";
  uint8_t char_count = 1; // already skipped the first commannd letter
  float value;

  printf("Function Success: %d\n", read_float(line, &char_count, &value));

  return 0;
}
