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
    if ((letter < 'A') || (letter > 'Z')) { print_str("Expected a capital command letter!\n"); }


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
        // reading argument
        char_count++;
        if (!read_float(line, &char_count, &value)) { print_str("Bad Number Format\n"); }


      default:
        print_str("Command Letter Not Implemented\n");

    }

  }

}


