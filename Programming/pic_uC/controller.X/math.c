
#include "includes.h"

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
/*   return ((n << 3) + (n << 1)); */
/* } */

/* double sum(double* array, uint8_t array_length) { */

/*   int sum = 0; */
/*   for(int i=0; i<array_length; i++) { */
/*     sum += array[i]; */
/*   } */ 

/*   return (sum/array_length); */
/* } */


