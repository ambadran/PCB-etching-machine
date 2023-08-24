/*
 * File:   timers.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:01 PM
 */


#include "includes.h"

void timer2_init() {

  // Pre-scalar Value
  T2_PRESCALAR = T2_PRE_1;
  
  // Post-scalar Value
  T2_POSTSCALAR = T2_POST_1;

  // Timer2 ON
  TMR2ON = 1;

}
