
#ifndef CPU_MAP_H
#define CPU_MAP_H

// uart pins
#define TX_TRIS TRISC6
#define RX_TRIS TRISC7

// MSSP pins
#define SDI TRISB0
#define SDO TRISC6
#define SCK TRISB1
#define CS TRISC0

// heater pin
#define CCP1_TRIS TRISC2

// DC motor pins
#define CCP2_TRIS TRISC1
#define V1_TRIS TRISB3
#define V2_TRIS TRISB4
#define DEMULTIPLEXER_SELECT_TRIS TRISB5
#define V1_PIN PORTB3
#define V2_TRIS PORTB4
#define DEMULTIPLEXER_SELECT_PIN PORTB5


#endif
