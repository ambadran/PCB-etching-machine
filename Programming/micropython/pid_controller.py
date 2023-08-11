'''
Refer to iPad notes Project 'PCB-etching machine' for detailed math and explanations throughout
'''
from machine import Pin, PWM, ADC, Timer
from math import log
from time import ticks_ms, ticks_ms


class Heater:
    '''
    Class to control Heater
    '''
    pwm_pin = 1

    def __init__(self):
        '''
        setter method
        '''
        # attribute defs
        self.pwm = PWM(Pin(Heater.pwm_pin))

        # heater init
        self.set_heat(0)

    def set_heat_u16(self, value_u_16):
        '''
        controls pwm with 16-bit value

        NOTE: it also inverts the percentage (65535-value_u_16) as the pwm value is put through an inverter npn first for buffering the darlington npn
        '''
        self.pwm.duty_u16((65535-value_u_16))

    def set_heat(self, percentage):
        '''
        controls pwm with percentage out of 100% instead of 65535

        NOTE: it also inverts the percentage (100-percentage) as the pwm value is put through an inverter npn first for buffering the darlington npn
        '''
        self.pwm.duty_u16(int(((100-percentage)/100)*65535))

    def off(self):
        '''
        just sets heater power to 0
        '''
        self.set_heat(0)

    @property
    def percent(self):
        '''
        return current heating value
        '''
        return int(100-(self.pwm.duty_u16()/65535)*100)

    @property
    def raw_value(self):
        '''
        return current raw pwm value
        '''
        return self.pwm.duty_u16() 

    def __str__(self):
        '''
        displays current heating value
        '''
        return f"Heater at {self.percent}% which is {self.raw_value} pwm u_16 NOT INVERTED"


class Thermistor:
    '''
    Class to read and process Thermistor
    '''
    adc_pin = 26

    def __init__(self, Vcc, R_10k, num_samples, B_factor, R_nominal, room_temp):
        '''

        :param Vcc: Vcc of micropython board, measure with multimeter to get exact value for more accuracy
        :param R_10k: the exact value of the 10k resistor in series of the voltage divider is probably not 10k exactly
        :param num_samples: the number of samples to average for every average_reading
        :param B_factor: also known as thermistor factor, value from datasheet
        :param R_nominal: Thermistor resistance value at room temperature
        '''
        # User defined attributes
        self.adc = ADC(Pin(Thermistor.adc_pin))
        self.Vcc = Vcc
        self.R_10k = R_10k
        self.num_samples = num_samples
        self.B_factor = B_factor
        self.R_nominal = R_nominal
        self.room_temp = room_temp + 273.15

        # processing attributes
        self.room_temp_inv = 1/self.room_temp
        self.B_factor_inv = 1/self.B_factor

    def read_V(self):
        '''
        returns voltage value read 
        '''
        return ((self.adc.read_u16()*self.Vcc)/65535)

    def read_V_averaged(self):
        '''
        reads 'num_samples' voltage samples then return average V
        '''
        samples = []
        for _ in range(self.num_samples):
            samples.append(self.read_V())

        return sum(samples)/self.num_samples

    def read_R(self):
        '''
        returns resistance value of thermistor
        '''
        Vi = self.read_V()
        return ((self.R_10k*Vi)/(self.Vcc-Vi))

    def read_R_averaged(self):
        '''
        reads 'num_samples' voltage samples then return  Resistance value from the average V
        '''
        Vi = self.read_V_averaged()
        return ((self.R_10k*Vi)/(self.Vcc-Vi))

    def read_T(self):
        '''
        reads temperature using simplified B parameter Steinhard-Hart equations
        '''
        T_inverse = self.room_temp_inv + self.B_factor_inv*log(self.read_R_averaged()/self.R_nominal)
        T_celcuis = 1/T_inverse - 273.15
        return T_celcuis


class PID:
    '''
    PID control algorithm

    Main Goal: make setpoint-input = 0

    PID object operates in two modes: hardware timer calling mode and user calling mode.

        In hardware timer calling mode, method to calculate output will be used with a hardware timer which 
        automatically calls it in regular intervals (this uses the ._calc() method). 

        In user calling mode, the user will call the .calc() method regularly and the method iself
        automatically calculates dt with time.ticks_diff() and time.ticks_ms()
        NOTE: in this mode, the .calc() MUSTT be called regularly as .tick_ms() wraps around!

    NOTE: User must not use the wrong calc method in the wrong mode! 

    #TODO: raise an error when the wrong calc method is used in the wrong mode
    #TODO: implement .first_calc_call() which calculates time and error variables for the first time 
        thus no need to poll them if _last_var is none every call
    #TODO: implement 'low pass filter' to prevent derivative high frequency noise problem
            Also implement the alternative solution: 'an integral feedback'
    '''
    # the period at which the system reads input, update pid value and set plant output, unit in ms
    HARDWARE_TIMER_dt = 50  

    def __init__(self, Kp, Ki, Kd, setpoint, min_output, max_output, start_output=0.0, input_func_delay=0, input_func=None, output_func=None):
        '''
        Constructor for PID control algorithm

        :param Kp: Proportional gain constant
        :param Ki: Integral gain constant
        :param Kd: Derivative gain constant
        :param setpoint: wanted input
        :param min_output: minimum output value, used to prevent integral windup
        :param max_output: maximum output value, used to prevent integral windup
        :param start_output: starting output, used to set initial integral value
        :param input_func_delay: delay of input function in ms
        :param input_func: 
        :param output_func
        '''
        # User defined attributes
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.setpoint = setpoint

        if min_output >= max_output:
            raise ValueError("min_output attribute must be less max_output")
        self.min_output = min_output
        self.max_output = max_output
        self.start_output = start_output
        self.input_func = input_func
        self.output_func = output_func
        
        # Process attributes
        self.timer_mode_dt = PID.HARDWARE_TIMER_dt + input_func_delay

        # Resetting all internal attributes
        self.reset(start_output)

    def clamp(self, value):
        '''
        clamps the value argument to self.min_output and self.max_output
        '''
        # This is supposed to be the most efficient way but I think this is slower than actual polling
        # return min(max_output, max(value, min_output)) 

        if value <= self.min_output:
            return self.min_output 

        elif value >= self.max_output:
            return self.max_output

        else:
            return value

    def reset(self, value=0):
        '''
        resets all hidden variables of PID controller
        '''
        self._proportional = 0
        self._integral = self.clamp(value)
        self._derivative = 0

        self._last_error = None
        self._last_time = None
        # self._last_input = None  # will be used in differential_on_measurement mode

    def calc(self, input_):
        '''
        calculates next pid value output (to try and make next input_ as close to .setpoint as possible)
        This method automatically calculates dt with time.ticks_diff() and time.ticks_ms()
        NOTE: in this mode, the .calc() MUSTTT be called regularly as .tick_ms() wraps around!

        :param input_: current input value (same unit as setpoint)
        '''
        # Calculating dt
        now = tick_ms()
        dt = ticks_diff(now, self._last_time) if self._last_time is not None else 0

        # Calculating error terms
        error = self.setpoint - input_
        # d_input = input_ - (self._last_input if (self._last_input is not None) else input_)  # will be used in differential_on_measurement mode
        d_error = error - (self._last_error if (self._last_error is not None) else error)

        # Calculating the Proportional term
        self._proportional = self.Kp * error

        # Calculating the Integral term
        self._integral += self.Ki * error * self.timer_mode_dt
        self._integral = self.clamp(self._integral)  # Avoiding integral windup

        # Calculating the Derivative term
        self._derivative = self.Kd * d_error / self.timer_mode_dt

        # Calculating PID output
        output = self.clamp(self._proportional + self._integral + self._derivative)

        # setting previous variables
        self._last_error = error  # for next derivative term
        self._last_time = now
        # self._last_input = input_  # will be used in differential_on_measurement mode

        return output

    def _calc(self, input_):
        '''
        calculates next pid value output (to try and make next input_ as close to .setpoint as possible)

        :NOTE: This function shouldn't be called by user at any time as it assumes constant dt, 
                aka it is called at VERY CONSTANT intervals!
                This is done by using the Timer Module in micropython device which 
                is automatically activated using the .activate_pid_control() method.

        the dt in the pid calculations is self.timer_mode_dt which is calculated automatically upon object init

        :param input_: current input value (same unit as setpoint)
        '''
        # Calculating error terms
        error = self.setpoint - input_
        # d_input = input_ - (self._last_input if (self._last_input is not None) else input_)  # will be used in differential_on_measurement mode
        d_error = error - (self._last_error if (self._last_error is not None) else error)

        # Calculating the Proportional term
        self._proportional = self.Kp * error

        # Calculating the Integral term
        self._integral += self.Ki * error * self.timer_mode_dt
        self._integral = self.clamp(self._integral)  # Avoiding integral windup

        # Calculating the Derivative term
        self._derivative = self.Kd * d_error / self.timer_mode_dt

        # Calculating PID output
        output = self.clamp(self._proportional + self._integral + self._derivative)

        # setting previous variables
        self._last_error = error  # for next derivative term
        # self._last_input = input_  # will be used in differential_on_measurement mode

        return output

    def execute(self, t):
        '''
        calls input_func to know current input
        calls _calc to calculate next output
        calls output_func to execute current output
        '''
        self.output_func(self._calc(self.input_func()))

    def activate_pid(self, input_func, output_func):
        '''
        When this method is called a Hardware Timer block is used to call self._calc() at regular 
        time intervals. 

        :param input_func: the function to return the current input of the system
        :param output_func: the function to set plant output
        '''
        self._pid_timer = Timer(period=PID.HARDWARE_TIMER_dt, mode=Timer.PERIODIC, callback=self.execute)
        
    def deactivate_pid(self):
        '''
        Deactivates the Timer and the constant calling of self._calc()
        '''
        self._pid_timer.deinit()


def main():
    '''
    ### Main Routine ###
    Creating Input Object -> Thermistor
    Creating Control Algorithm Object -> PID
    Creating Output Object -> Heater
    '''
    ### Initialize Thermistor Object 
    Vcc = 3.28
    R_10k = 9880
    num_samples = 5
    B_factor = 4300
    R_nominal = 9360
    room_temp = 25  # input here in celcuis, but in calculations, it will be converted to kelvin
    global thermistor
    thermistor = Thermistor(Vcc, R_10k, num_samples, B_factor, R_nominal, room_temp)

    ### Initialize PID Control Object
    Kp = 5
    Ki = 0.01
    Kd = 0.1
    setpoint = 40  # degrees celcuis
    min_output = 0  # output power unit in percentage
    max_output = 100
    global pid
    pid = PID(Kp, Ki, Kd, setpoint, min_output, max_output)

    ### Initialize Heater Object
    global heater
    heater = Heater()

    ### Activating System :)
    pid.activate_pid(thermistor.read_T, heater.set_heat)


