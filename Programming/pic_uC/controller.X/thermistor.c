/*
 * File:   thermistor.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:02 PM
 */


#include "includes.h"

float read_V(void) {
  return ((read_adc(THERMISTOR_ADC)*VCC)/1024);
  /* return (read_adc()*READ_V_CONSTANT) */
}

float read_V_averaged(void) {
  float sum = 0;
  uint8_t i;
  for(i=0; i<NUM_SAMPLES; i++) {
    sum += read_V();
  }
  return (sum/NUM_SAMPLES);
}

int read_R_averaged(void) {

  float Vi = read_V_averaged();
  return (int)((R_10K*Vi) / (VCC-Vi));

}

float read_T() {

  float T_inverse = ROOM_TEMPERATURE_INV + B_FACTOR_INV*log(read_R_averaged()/R_NOMINAL);
  return ((1/T_inverse) - 273.15);

}



