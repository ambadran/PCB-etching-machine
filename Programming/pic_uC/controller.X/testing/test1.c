#include <stdio.h>

int main() {

  double value = 243.32;
  printf("Value: %f\n", value);

  // Whole number section
  int digit = (int)value%10;
  printf("Digit1: %d\n", digit);

  double tmp = value;

  tmp /= 10;
  int digit2 =  (int)tmp%10;
  printf("Digit2: %d\n", digit2);

  tmp /= 10;
  int digit3 =  (int)tmp%10;
  printf("Digit3: %d\n", digit3);

  tmp /= 10;
  int digit4 =  (int)tmp%10;
  printf("Digit4: %d\n", digit4);

  printf("tmp: %f\n", tmp);

  return 0;
}
