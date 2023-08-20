
#ifndef THERMISTOR_H
#define THERMISTOR_H

#define VCC 3.28  // Vcc value in thermistor voltage divider
#define R_10K 9880  // The exact value of the 10kohm resistor
#define NUM_SAMPLES 5  // how many samples to take before taking average 
#define B_FACTOR 4300  // Thermistor B factor from its datasheet
#define R_NOMINAL 9360  // Nominal resistance value at room temperature
#define ROOM_TEMPERATURE 298.15  // 25degree + 273.15 to get value in kelvin
#define ROOM_TEMPERATURE_INV 1/ROOM_TEMPERATURE
#define B_FACTOR_INV 1/B_FACTOR

// convert ADC value to Voltage
double read_V();

// reads NUM_SAMPLES voltages and takes average for more accuracy
double read_V_averaged();

// Converts averaged Voltage to resistance
double read_R();

// uses Steinhart-Hart equations to convert resistance to temperature
double read_T();

#endif
