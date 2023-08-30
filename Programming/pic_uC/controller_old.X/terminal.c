/*
 * File:   debugger.c
 * Author: mr-atom
 *
 * Created on August 26, 2023, 2:26 PM
 */

#include "includes.h"

void terminal_execute_line(char* line) {

  uint8_t char_count = 0;
  char letter;
  float value;
  while (line[char_count] != 0) {

    // Reading letter argument, aka which function to execute
    letter = line[char_count];

    // only capital letters expected
    if ((letter < 'A') || (letter > 'Z')) { 
      print_str("Expected a capital command letter!\n"); 
      continue;
    }

    // incrementing char_count for next iteration
    char_count++;

    /* Identifying function and printing the resultant execution */
    switch(letter) {

      case 'A':

        print_str("read_V(): ");
        print_double(read_V());
        print_str("\n");
        break;

      case 'B':

        print_str("read_V_averaged(): ");
        print_double(read_V_averaged());
        print_str("\n");
        break;

      case 'C':

        print_str("read_R_averaged(): ");
        print_int(read_R_averaged());
        print_str("\n");
        break;

      case 'D':

        print_str("read_T(): ");
        print_double(read_T());
        print_str("\n");
        break;

      case 'E':
        // reading float argument
        if (!read_float(line, &char_count, &value)) {
          print_str("Bad Number Format\n");
          break;
        }

        // controlling cpp frequency
        cpp1_freq((long int)value);

        // Reporting
        print_str("cpp1_freq(");
        print_int((int)value);
        print_str(")\n");
        break;

      case 'F':
        // reading float argument
        if (!read_float(line, &char_count, &value)) {
          print_str("Bad Number Format\n");
          break;
        }

        // controlling cpp frequency
        cpp1_duty_cycle((uint8_t)value);

        // Reporting
        print_str("cpp1_duty_cycle(");
        print_int((uint8_t)value);
        print_str(")\n");
        break;


      default:
        print_str("Command Letter Not Implemented\n");
        break;

    }

  }

}


