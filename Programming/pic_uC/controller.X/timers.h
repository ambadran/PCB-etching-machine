
#ifndef TIMERS_H
#define TIMERS_H

#define T1_PRESCALAR_MODES_NUMBER 4
#define T1_PRESCALAR T1CONbits.T1CKPS
enum T1_PRESCALAR_VALUE {T1_PRE_1, T1_PRE_2, T1_PRE_4, T1_PRE_8};
const uint8_t T1_PRE_ACTUAL[T1_PRESCALAR_MODES_NUMBER] = {1, 2, 4, 8};
#define TIMER1_M_OVERFLOW_MS 50  // total time passed after timer1_overflow_counter reaches target in ms
#define TARGET_TIMER1_OVERFLOW TIMER1_M_OVERFLOW_MS/5  // I set pre-loading value in timer1_ISR to be exactly 5ms

#define T2_PRESCALAR_MODES_NUMBER 3
#define T2_PRESCALAR T2CONbits.T2CKPS
enum T2_PRESCALAR_VALUE {T2_PRE_1, T2_PRE_4, T2_PRE_16};
const uint8_t T2_PRE_ACTUAL[T2_PRESCALAR_MODES_NUMBER] = {1, 4, 16};
#define T2_POSTSCALAR T2CONbits.TOUTPS
enum T2_POSTSCALAR_VALUE {T2_POST_1, T2_POST_2,  T2_POST_3,  T2_POST_4,  T2_POST_5,  T2_POST_6,  T2_POST_7,  T2_POST_8,  T2_POST_9,  T2_POST_10, T2_POST_11,  T2_POST_12, T2_POST_13};

uint8_t timer1_overflow_counter = 0;

void timer1_init(void);
void timer1_ISR(void);
void timer2_init(void);

#endif
