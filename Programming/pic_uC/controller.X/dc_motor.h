
#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#define v1_on() 
#define v1_off() 
#define v1_set(v)
#define v1_state()

#define v2_on() 
#define v2_off() 
#define v2_set(v)
#define v2_state()

#define g1_on() 
#define g1_off() 
#define g1_set(v)
#define g1_state()

#define g2_on() 
#define g2_off() 
#define g2_set(v)
#define g2_state()

typedef enum {
  CW,
  CCW
} CW_CCW_STATE;
extern CW_CCW_STATE cw_ccw_state;

void dc_motor_init(void);
void dc_motor_cw(void);
void dc_motor_ccw(void);
void dc_motor_dir(CW_CCW_STATE cw_ccw_state);
void dc_motor_toggle_dir(void);
uint8_t dc_motor_is_on(void);


#endif
