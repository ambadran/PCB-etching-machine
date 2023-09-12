#include <stdio.h>
#include <stdint.h>

#define PID_MAX_OUTPUT 100
#define PID_MIN_OUTPUT 50
#define CLAMP(value) (((uint8_t)value) <= (PID_MIN_OUTPUT) ? (PID_MIN_OUTPUT) : ( \
                      ((uint8_t)value) >= (PID_MAX_OUTPUT) ? (PID_MAX_OUTPUT) : ( ((uint8_t)value) )))

int main() {

  printf("Value: %d\n", CLAMP(74));

  return 0;
}
