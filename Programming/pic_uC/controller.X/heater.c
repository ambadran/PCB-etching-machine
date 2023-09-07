/*
 * File:   heater.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:02 PM
 */

#include "includes.h"


void heater_init(void) {

  cpp1_freq(5000);
  heater_set(0);

}

void heater_set(uint8_t value) {
  cpp1_duty_cycle((float)(100-value));
  current_heating_value = value;
}

void heater_off(void) {
  cpp1_duty_cycle(100.0);
  current_heating_value = 0;
}

void heater_on(void) {
  cpp1_duty_cycle(0.0);
  current_heating_value = 100;
}

