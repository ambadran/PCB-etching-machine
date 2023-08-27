#include <stdio.h>
#include <stdint.h>

// used to convert a 'number' dataype to a char :D
// aka 3 to '3'
uint8_t intToAscii(uint8_t int_value) {

  return int_value + '0';

}

// used to convert a char datatype which is a digit to 'number' datatype :D
// aka '3' to 3
uint8_t AsciiToint(uint8_t ascii_value) { // try char

  return ascii_value - '0';

}

int main() {

  char character = '3';
  uint8_t number = 3;

  printf("character in c: %c, in d(ascii): %d\n", character, character);
  printf("AsciiToint(character) in c: %c(no char, already an int), in d: %d\n", AsciiToint(character), AsciiToint(character));
  printf("\n");
  printf("number in c: %c(no char, already an int), in d: %d\n", number, number);
  printf("intToAscii(number) in c: %c, in d(ascii): %d\n", intToAscii(number), intToAscii(number));

  return 0;
}

