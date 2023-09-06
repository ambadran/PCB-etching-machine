
#ifndef HEATER_H
#define HEATER_H

uint8_t current_heating_value;

void heater_init(void);
void heater_set(uint8_t value);
void heater_off(void);
void heater_on(void);

#endif
