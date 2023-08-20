/*
 * File:   adc.c
 * Author: mr-atom
 *
 * Created on August 17, 2023, 2:01 PM
 */

#include "includes.h"

void adc_init(void) {

  // A/D Conversion Clock Select bits
  ADCON0bits.ADCS = 0b10;
  ADCS2 = 0;

  // Result Format
  ADFM = 1;

  // Configuration Pin A0 to be the only analogue pin and choosing it permenantly
  ADCON1bits.PCFG = 0b1110; 
  ADCON0bits.CHS = 0;

  // Module ON
  ADON = 1;

}

// Reading ADC value
int read_adc() {
    __delay_us(22); // delaying the acquisition time
    GO_DONE = 1;
    while(GO_DONE) {
        // waiting for the ADC conversion to finish
    }

    return (ADRESH << 8) + ADRESL; // reading the analog input
}
