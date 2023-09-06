
#ifndef CPP_MODULE_H
#define CPP_MODULE_H

#define DIVISION_CONSTANT _XTAL_FREQ/100

// setting CCP module to PWM mode and setting TRIS registers
void cpp1_init(void);

// setting the prescalar and getting the frequency value
void cpp1_freq(long int frequency);

// sets duty cycle of PWM
void cpp1_duty_cycle(float duty_cycle);


#endif
