
#ifndef CONFIG_H
#define CONFIG_H

// uC specific configs
#define _XTAL_FREQ 48000000

/* User specific configs */

// UART configs
#define INT_DIGITS 5
#define FLOAT_WHOLE_DIGITS 3
#define FLOAT_WHOLE_DECIMALS 3


// PID configs
#define Kp 22.4303
#define Ki 0.005
#define Kd 21.336

#define PID_MAX_OUTPUT 100
#define PID_MIN_OUTPUT 0

#define PID_INPUT_FUNC read_T
#define PID_OUTPUT_FUNC(v) heater_set(v)

#endif
