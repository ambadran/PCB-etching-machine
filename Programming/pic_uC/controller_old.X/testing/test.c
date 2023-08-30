#include <stdio.h>
#include <stdint.h>

void print(char* str) {

  while(*str != '\0') {

    printf("%c", *str);
    str ++;
  }
  printf("\n");
}


void main() {

  char* message = "testing";

  /* uint8_t ind = 0; */
  /*   while (message[ind] != '\0') { */
  /*     printf("%d\n", message[ind]); */
  /*     ind++; */
  /*   } */

  print("lskdjfl\n");

  return;
}
