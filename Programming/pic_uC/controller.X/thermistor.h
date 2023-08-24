
#ifndef THERMISTOR_H
#define THERMISTOR_H

#define VCC 4.93  // Vcc value in thermistor voltage divider
#define R_10K 9880  // The exact value of the 10kohm resistor
#define NUM_SAMPLES 5  // how many samples to take before taking average 
#define B_FACTOR 4300.0  // Thermistor B factor from its datasheet
#define R_NOMINAL 9360.0  // Nominal resistance value at room temperature
#define ROOM_TEMPERATURE 298.15  // 25degree + 273.15 to get value in kelvin
#define ROOM_TEMPERATURE_INV 1/ROOM_TEMPERATURE
#define B_FACTOR_INV 1/B_FACTOR
/* #define B_FACTOR_INV 0.000208 */

// convert ADC value to Voltage
double read_V(void);

// reads NUM_SAMPLES voltages and takes average for more accuracy
double read_V_averaged(void);

// Converts averaged Voltage to resistance
int read_R_averaged(void);

// uses Steinhart-Hart equations to convert resistance to temperature
double read_T(void);

#endif
