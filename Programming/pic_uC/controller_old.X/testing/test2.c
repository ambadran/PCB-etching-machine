#include <stdio.h>
#include <stdint.h>

#define INT_DIGITS 4
#define FLOAT_DIGITS 5

int intToASCII(int value) {
  return '0' + value;
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

/* unsigned mulu10(double n) { */
/*   return ((n << 3.0) + (n << 1.0)); */
/* } */


int main() {

  double value = 1738.873;
  printf("first value: %2f\n", value);

  int tmp = (int)value;
  printf("tmp: %d\n", tmp);

  // whole number stuff
  uint8_t* digits[INT_DIGITS];
  int8_t i;

  digits[INT_DIGITS-1] = (uint8_t)(tmp%10);

  for(i=INT_DIGITS-2 ; i>=0 ; i--) {
    tmp = divu10(tmp);
    digits[i] = tmp%10;
  }

  for(i=0; i<INT_DIGITS; i++) {
    printf("output: %d at index: %d\n", digits[i], i);
  }

  // decimal number stuff
  for(i=0; i<FLOAT_DIGITS; i++) {
    /* value = mulu10(value); */
    value *= 10;
    printf("value: %d, tmp: %f\n", (int)value%10, value);
  }

  return 0;
}
