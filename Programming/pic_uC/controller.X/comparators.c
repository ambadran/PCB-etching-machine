/*
 * File:   comparators.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:02 PM
 */


#include "includes.h"

void comparator_deinit(void) {

  CMCONbits.CM = 0b111;

}
