/*
 * File:   debugger.c
 * Author: mr-atom
 *
 * Created on August 26, 2023, 2:26 PM
 */

#include "includes.h"

void terminal_execute_line(char* line) {

  // Mode functions are functions that don't pass argument
  // with command word
  // instead they pass function mode with command letter
  // and then pass argument in argument letter 
  // e.g- 'G' -> cpp funcs
  uint8_t mode_func_ind = MODE_FUNC_NONE;

  uint8_t char_count = 0;
  char letter;
  value_t value;  // union of either float or long int depending on user input
  uint8_t int_value; // meant to be assigned to x in Gx.0
  uint16_t mantissa; // not really the mantissa defined by xc8 compiler, it's what the gerber code named it. It's basically the mantissa without the whole number digits.
  while (line[char_count] != 0) {

    // Reading letter argument, aka which function to execute
    letter = line[char_count];

    // incrementing char_count for next iteration
    char_count++;

    // IMP: capital letters are command letters
    //      small letters are argument letters
    // Checking letter is a letter
    // checking if it's before A or after z or any character in between Z and a in the ascii table
    if ( letter < 'A' || letter > 'z' || (letter > 'Z' && letter < 'a')) {
      print_str("Expected a letter!\n"); 
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
        // command letter value will only choose
        // if we want to control cpp1 or cpp2
        // and if we want to control frequency or duty cycle
        // 
        // So the loop will continue on to catch letter 'p'
        // which has the frequency/duty_cycle value for cpp1/2
        // only then will the actual function get executed

        // reading int argument
        if (!read_int(line, &char_count, &value)) {
          print_str("Bad int Number Format\n");
          break;
        }

        mode_func_ind = MODE_FUNC_CPP;

        selected_cpp = trunc(value);
        selected_freq_duty =  round(100*(value - int_value));

        break;

      case 'H':

        break;

      case 'I':
        // reading int argument
        if (!read_int(line, &char_count, &value)) {
          print_str("Current Heating Value: ");
          print_int((uint8_t)current_heating_value);
          print_char('\n');
          break;
        }

        heater_set((uint8_t)value.long_int);

        // Reporting
        print_str("heater_set(");
        print_int((int)value.long_int);
        print_str(")\n");
        break;

      case 'J':

        // reading float argument
        if (!read_float(line, &char_count, &value)) {
          print_str("Current pid value is set to: ");
          print_float(pid.setpoint);
          print_char('\n');
          break;
        }

        // setting PID setpoint
        pid.setpoint = value.float_;

        // Reporting
        print_str("pid_setpoint(");
        print_float(value.float_);
        print_str(")\n");
        break;

      case 'K':

        // Activating PID controller !
        pid_activate();

        // Reporting
        print_str("Activating PID Controller!\n");

        break;

      case 'L':

        // Activating PID controller !
        pid_deactivate();

        // Reporting
        print_str("Deactivating PID Controller!\n");

        break;

      case 'M':

        // toggling PID report status
        pid_report_show = !pid_report_show;

        // Reporting
        print_str("PID report showing status: ");
        print_bit(pid_report_show);
        print_char('\n');

        break;

      default:

        switch(letter) {

          case 'a':
            // reading int argument
            if (!read_int(line, &char_count, &value)) {
              print_str("Bad int Number Format\n");
            }
            break;

          case 'b':
            // reading float argument
            if (!read_float(line, &char_count, &value)) {
              print_str("Bad float Number Format\n");
              break;
            }

        }
        print_str("Command Letter Not Implemented\n");
        break;

    }

  }
  /* Parsing complete! (and non mode funcs are executed too)*/

  // The rest of the steps are only Applicable for Mode funcs
  if (mode_func_ind == MODE_FUNC_NONE) {
    return;
  }

  /* [ Step 3: Error checking ] */
  switch(mode_func_ind) {

    case MODE_FUNC_CPP:
      if (selected_cpp > 1) {

        print_str("ERROR: selected cpp module is either 0 or 1 for cpp1 or cpp2 respectively.\n");
        return;

      } else if (selected_freq_duty > 1) {

        print_str("ERROR: can only choose 0 for frequency control and 1 for duty cycle control");
        return;

      }

  }

  /* [ Step 4: Execute! ] */
  switch (mode_func_ind) {

    case MODE_FUNC_CPP:

        // controlling cpp frequency
        cpp1_freq(value.long_int);
        // controlling cpp frequency
        cpp1_duty_cycle((float)value.long_int);

        // Reporting
        print_str("cpp");
        print_int(selected_cpp+1);
        print_str(cpp_func_print_string[selected_freq_duty])
        print_int((int)value.long_int);
        print_str(")\n");

  }

}
