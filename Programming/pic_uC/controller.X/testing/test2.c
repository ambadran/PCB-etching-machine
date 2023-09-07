#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define PID_MIN_OUTPUT 0
#define PID_MAX_OUTPUT 100
#define CLAMP(value) (((uint8_t)value) <= (PID_MIN_OUTPUT) ? (PID_MIN_OUTPUT) : ( \
                      ((uint8_t)value) >= (PID_MAX_OUTPUT) ? (PID_MAX_OUTPUT) : ( (value) )))



int main() {

  float value = 100.03;
  uint8_t b1 = 1;
  uint8_t b2 = 0;

  printf("%f\n", CLAMP(value));

  printf("%d\n", (b1 && !b2));

  return 0;
}
