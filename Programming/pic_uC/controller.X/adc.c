/*
 * File:   adc.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:01 PM
 */

#include "includes.h"

void adc_init(void) {
  
  // voltage refrence
  VCFG0 = 0;
  VCFG1 = 0;

  // A/D Conversion Clock Select bits
  ADCON2bits.ADCS = 0b110;
  ADCON2bits.ACQT = 0b10;


  // Result Format
  ADFM = 1;

  // Configuration Pin A0 and Pin A1 to be the only analogue pin
  ADCON1bits.PCFG = 0b1101; 

  // Module ON
  ADON = 1;

}

// Reading ADC value
int read_adc(ADC_UTILIZING_DEVICE adc_utilizing_device) {
    
    ADCON0bits.CHS = adc_utilizing_device

    __delay_us(22); // delaying the acquisition time
    GO_DONE = 1;
    while(GO_DONE) {
        // waiting for the ADC conversion to finish
    }

    return (ADRESH << 8) + ADRESL; // reading the analog input
}
