
#ifndef TERMINAL_H
#define TERMINAL_H

#define MODE_FUNC_NONE 0
#define MODE_FUNC_CPP 1

#define SELECT_CPP1 0
#define SELECT_CPP2 1
#define SELECT_FREQ 0
#define SELECT_DUTY 1
uint8_t selected_cpp;
uint8_t selected_freq_duty;
const void (*cppx_func[2][2])(int) = { {cpp1_freq, cpp1_duty}, {cpp2_freq, cpp2_duty} };
const char* cpp_func_print_string[] = {"_freq(", "_duty_cycle("};

void terminal_execute_line(char* line);

#endif
