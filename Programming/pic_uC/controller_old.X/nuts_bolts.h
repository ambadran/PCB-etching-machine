
#ifndef NUTS_BOLTS_H
#define NUTS_BOLTS_H

#define MAX_INT_DIGITS 8  // maximum digits stored in uint32_t

// reads int only not floats
uint8_t read_int(char *line, uint8_t *char_count, float *float_ptr);

// arguments are the line of terminal code itself
// char_count_ptr to increment it as we read and be able to use it later in the terminal_execute_line after this func is done
// float_ptr to assign the output of this function to it
// THIS FUNCTION RETURN WHETHER IT SUCCESSFULYY READ THE FLOAT OR NOT, AKA (0 OR 1) 
uint8_t read_float(char *line, uint8_t *char_count, float *float_ptr); 


#endif
