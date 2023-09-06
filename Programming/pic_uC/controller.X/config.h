
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
#define Kp
#define Ki
#define Kd

#define PID_OUTPUT_START_VALUE
#define PID_MAX_OUTPUT 100.0
#define PID_MIN_OUTPUT 0.0

#define CALC_FUNC_DELAY

#define PID_INPUT_FUNC read_T
#define PID_OUTPUT_FUNC(v) heater_set(v)

#endif
