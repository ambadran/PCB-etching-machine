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

    print_str("Enter Received..");
    
  } else {

    terminal_execute_line(line);

  }

}

void protocol_main_loop() {

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

      // buffer overflow
      } else if (char_count >= (LINE_BUFFER_SIZE-1)) {

        print_str("BUFFER OVERFLOW!!!!");
        char_count = 0;

      // store uart_rx_buffer in line character array
      } else {

        line[char_count++] = c;

      }
    }
  }
}
