#include <stdio.h>

#define B_FACTOR 4300.0
#define B_FACTOR_INV 1/B_FACTOR

#define RT 273.15
#define RT_INV 1/RT

int main() {

  printf("%f, %f\n", B_FACTOR, B_FACTOR_INV);

  printf("%f, %f\n", RT, RT_INV);

  return 0;
}
