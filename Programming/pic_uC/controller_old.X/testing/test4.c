#include <stdio.h>

void test(int num) {

  if (num == 1) {

    printf("Num: %d\n", num);
    return;

  } 

  printf("one\n");

  if (num == 2) {

    printf("Num: %d\n", num);
    return;

  } 

  printf("two\n");

  if (num == 3) {

    printf("Num: %d\n", num);
    return;

  }

  printf("three\n");

}


int main() {

  test(2);

}
