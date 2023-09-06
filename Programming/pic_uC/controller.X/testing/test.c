#include <stdio.h>

#define test1(v) test4(v)
#define test3 test5

#define test() test1(test2(test3()))

void test4(int v) {
  printf("Outputing %d\n", v);
}

int test5() {
  return 5;
}

int test2(int v) {
  return v+1;
}

int main() {

  test(); // it works ;D

  return 0;
}
