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
  value_t value;  // union of either float or long int depending on user input
  while (line[char_count] != 0) {

    // Reading letter argument, aka which function to execute
    letter = line[char_count];

    // incrementing char_count for next iteration
    char_count++;

    // only capital letters expected
    if ((letter < 'A') || (letter > 'Z')) { 
      print_str("Expected a capital command letter!\n"); 
      continue;
    }


    /* Identifying function and printing the resultant execution */
    switch(letter) {

      case 'A':

        print_str("read_V(): ");
        print_float(read_V());
        print_str("\n");
        break;

      case 'B':

        print_str("read_V_averaged(): ");
        print_float(read_V_averaged());
        print_str("\n");
        break;

      case 'C':

        print_str("read_R_averaged(): ");
        print_int(read_R_averaged());
        print_str("\n");
        break;

      case 'D':

        print_str("read_T(): ");
        print_float(read_T());
        print_str("\n");
        break;

      case 'E':
        // reading int argument
        if (!read_int(line, &char_count, &value)) {
          print_str("Bad integer Number Format\n");
          break;
        }

        print_str("Read INT value: ");
        print_int((int)value.long_int);
        print_char('\n');
        break;

      case 'F':

        // reading float argument
        if (!read_float(line, &char_count, &value)) {
          print_str("Bad float Number Format\n");
          break;
        }

        print_str("Read Float value: ");
        print_float(value.float_);
        print_char('\n');
        break;


      case 'G':

        // reading int argument
        if (!read_int(line, &char_count, &value)) {
          print_str("Bad int Number Format\n");
          break;
        }

        // controlling cpp frequency
        cpp1_freq(value.long_int);

        // Reporting
        print_str("cpp1_freq(");
        print_int((int)value.long_int);
        print_str(")\n");
        break;

      case 'H':
        // reading int argument
        if (!read_int(line, &char_count, &value)) {
          print_str("Bad int Number Format\n");
          break;
        }

        // controlling cpp frequency
        cpp1_duty_cycle((float)value.long_int);

        // Reporting
        print_str("cpp1_duty_cycle(");
        print_int((int)value.long_int);
        print_str(")\n");
        break;

      case 'I':
        // reading int argument
        if (!read_int(line, &char_count, &value)) {
          print_str("Bad int Number Format\n");
          break;
        }

        heater_set((uint8_t)value.long_int);

        // Reporting
        print_str("heater_set(");
        print_int((int)value.long_int);
        print_str(")\n");
        break;

      case 'J':

        // printing current heatering value
        print_str("Current Heating Value: ");
        print_int((uint8_t)current_heating_value);
        print_char('\n');
        break;

      case 'K':

        // reading float argument
        if (!read_float(line, &char_count, &value)) {
          print_str("Bad float Number Format\n");
          break;
        }

        // setting PID setpoint
        pid_setpoint(value.float_);

        // Reporting
        print_str("pid_setpoint(");
        print_float(value.float_);
        print_str(")\n");
        break;

      case 'L':

        // Activating PID controller !
        pid_activate();

        // Reporting
        print_str("Activating PID Controller!\n");

        break;

      case 'M':

        // Activating PID controller !
        pid_deactivate();

        // Reporting
        print_str("Deactivating PID Controller!\n");

        break;

      case 'N':

        // toggling PID report status
        pid_report_show = !pid_report_show;

        // Reporting
        print_str("PID report showing status: ");
        print_bit(pid_report_show);
        print_char('\n');

        break;

      default:
        print_str("Command Letter Not Implemented\n");
        break;

    }

  }

}
