/*
 * File:   protocol.c
 * Author: mr-atom
 *
 * Created on August 26, 2023, 2:25 PM
 */

#include "includes.h"

static char line[LINE_BUFFER_SIZE];

static void protocol_execute_line(char* line) {

  if(line[0] == 0) {

    print_str("Enter Received..\n");
    
  } else {

    terminal_execute_line(line);

  }

}

void protocol_main_loop(void) {

  uint8_t char_count = 0;
  uint8_t c;
  while(1) {

    while ((c = uart_read()) != SERIAL_NO_DATA) {

      // line end
      if ((c == '\n') || (c == '\r')) {
        line[char_count] = 0; // string termination character
        char_count = 0;

        // executing line!
        protocol_execute_line(line);

      // throw away whitespaces and control characters
      } else if (c <= ' '){
        // do nothing

      // buffer overflow
      } else if (char_count >= (LINE_BUFFER_SIZE-1)) {

        print_str("BUFFER OVERFLOW!!!!");
        char_count = 0;

      // store uart_rx_buffer in line character array
      } else {

        line[char_count++] = c;

      }
    }

    if (pid_execute_flag) {
      RB0 = 1;  // for measuring on oscilloscope
      pid_execute();
      RB0 = 0;
      pid_execute_flag = 0;
    }

    if (pid_report_show && !pid_report_shown) {
      RB1 = 1;
      pid_report();
      RB1 = 0;
      pid_report_shown = 1;
    }
    
  }

}
