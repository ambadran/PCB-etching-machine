#include <stdio.h>

/* enum test_t {test1, test2}; */
typedef enum {
  test1,
  test2
} test_t;

void test_func(test_t test) {
  printf("%d\n", test);
}

int main() {

  /* printf("%d\n", test3); */
  test_func(test2);

  return 0;
}

