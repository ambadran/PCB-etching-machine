
#ifndef CONFIG_H
#define CONFIG_H

// uC specific configs
#define _XTAL_FREQ (float)48000000.0

/********* User specific configs **********/

/********************** UART configs **********************/
#define UART_BAUDRATE 250000
#define INT_DIGITS 5
#define FLOAT_WHOLE_DIGITS 3
#define FLOAT_WHOLE_DECIMALS 3
/***************************************************/

/********************* PID configs **********************/
#define Kp 22.4303
#define Ki 0.005
#define Kd 21.336

#define PID_MAX_OUTPUT 100
#define PID_MIN_OUTPUT 0

// put the function that returns the input to the pid controller, aka the the value to subtract from setpoint to find error
#define PID_INPUT_FUNC read_T
// put the function that takes in the output of the pid controller to influence the plant
#define PID_OUTPUT_FUNC(v) heater_set(v)

// comment out to not report pid outputs
#define REPORT_LAST_OUTPUT
/***************************************************/


/********************* DC Motor configs **********************/

#define V1_ACTIVE_LOW_STATE 0
#define V2_ACTIVE_LOW_STATE 0
#define G1_ACTIVE_LOW_STATE 1
#define G2_ACTIVE_LOW_STATE 1

/***************************************************/

#endif
