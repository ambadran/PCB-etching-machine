
#ifndef ADC_H
#define ADC_H

// CHS = 0 points to Pin AN0 which is connected to Thermistor
// CHS = 1 points to Pin AN1 which is connected to DC_motor_current_sensor
typedef enum {
  THERMISTOR_ADC,
  DC_MOTOR_CURRENT_ADC
} ADC_UTILIZING_DEVICE;

void adc_init(void);
int read_adc(ADC_UTILIZING_DEVICE adc_utilizing_device);

#endif
