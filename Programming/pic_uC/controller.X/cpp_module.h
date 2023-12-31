
#ifndef CPP_MODULE_H
#define CPP_MODULE_H

#define DIVISION_CONSTANT _XTAL_FREQ/100

// setting CCP module to PWM mode and setting TRIS registers
void cpp1_init(void);
void cpp2_init(void);

// setting the prescalar and getting the frequency value
// NOTE: when both cpp are configured as PWM, they will have same frequency
// so only one cpp_freq function is defined
void cpp_freq(long int frequency);

// sets duty cycle of PWM
void cpp1_duty_cycle(float duty_cycle);
void cpp2_duty_cycle(float duty_cycle);


#endif
