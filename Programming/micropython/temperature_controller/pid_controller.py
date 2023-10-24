'''
Refer to iPad notes Project 'PCB-etching machine' for detailed math and explanations throughout
'''
from machine import Pin, PWM, ADC, Timer
from math import log
from time import ticks_ms, ticks_ms, sleep_ms, sleep
import _thread

class Device:
    ESP32S3 = 0
    PICO = 0

# Pin assignments
ADC_PIN = {Device.PICO: 26, Device.ESP32S3: 15}
PWM_PIN = {Device.PICO: 1, Device.ESP32S3: 8}

# Choosing wanted device to select proper pin
# device = Device.PICO
device_name = Device.ESP32S3

# For esp32s3, must choose a timer index
PID_TIMER_IND = 2
HEATER_STEP_SIGNAL_TIMER_IND = 3 

def do_nothing():
    pass

class Heater:
    '''
    Class to control Heater
    '''

    def __init__(self, pwm_pin):
        '''
        setter method
        '''
        # attribute defs
        self.pwm = PWM(Pin(pwm_pin))
        self.pwm.freq(20000)

        # heater init
        self.set(0)

        # constant attributes
        self.state_funcs = {False: self.off, True: self.on}

    def set_u16(self, value_u_16):
        '''
        controls pwm with 16-bit value
        '''
        self.pwm.duty_u16((value_u_16))

    def set(self, percentage):
        '''
        controls pwm with percentage out of 100% instead of 65534

        NOTE: it also inverts the percentage (100-percentage) as the pwm value is put through an inverter npn first for buffering the darlington npn
        '''
        self.pwm.duty_u16(round((percentage/100)*65534))

    @property
    def percent(self):
        '''
        return current heating value
        '''
        return round((self.pwm.duty_u16()/65534)*100, 3)

    @property
    def raw_value(self):
        '''
        return current raw pwm value
        '''
        return self.pwm.duty_u16() 

    def step_signal(self, delay):
        '''
        used to test open-loop step response

        :param delay: in ms
        '''
        self.set(100)
        temp_timer = Timer(HEATER_STEP_SIGNAL_TIMER_IND)
        temp_timer.init(period=delay, mode=Timer.ONE_SHOT, callback=lambda t:self.set(0))

    def on(self):
        '''
        turns heater 100%
        '''
        self.set(100)

    def off(self):
        '''
        turns heater 0%
        '''
        self.set(0)
    
    def state(self, state):
        '''
        turns heater 100%
        '''
        self.state_funcs[state]()

    def __repr__(self):
        '''
        displays current heating value
        '''
        return f"Heater at {self.percent}% which is {self.raw_value} pwm u_16 "


class Thermistor:
    '''
    Class to read and process Thermistor
    '''
    def __init__(self, adc_pin, Vcc, R_10k, num_samples, B_factor, R_nominal, room_temp):
        '''

        :param Vcc: Vcc of micropython board, measure with multimeter to get exact value for more accuracy
        :param R_10k: the exact value of the 10k resistor in series of the voltage divider is probably not 10k exactly
        :param num_samples: the number of samples to average for every average_reading
        :param B_factor: also known as thermistor factor, value from datasheet
        :param R_nominal: Thermistor resistance value at room temperature
        '''
        # User defined attributes
        self.adc = ADC(Pin(adc_pin))
        if device_name == Device.ESP32S3:
            self.adc.atten(3)

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
        return ((self.adc.read_u16()*self.Vcc)/65534)

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

    def monitor(self, time_delay=100, exit_on_interrupt=False):
        '''
        continiously print temperature until KeyboardInterrupt

        :param time_delay: how much time to delay between each print in ms

        could be used with terminal command tee to pull data off the micorpython device
        '''
        try:
            while True:
                # print(f"Temperature: {self.read_T()}\r", end='')
                print(f"Temperature: {self.read_T()}", end='\r')  # for use in logging
                sleep_ms(time_delay)

        except KeyboardInterrupt:
            # if exit_on_interrupt:
            #     _thread.exit()  # doesn't work for some reason ?!?
            # else:
            return

        finally:
            do_nothing()


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
        self._last_input = None  # will be used in differential_on_measurement mode
        self._last_output = None

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
        self._integral += self.Ki * error * dt
        self._integral = self.clamp(self._integral)  # Avoiding integral windup

        # Calculating the Derivative term
        self._derivative = self.Kd * d_error / dt

        # Calculating PID output
        output = self.clamp(self._proportional + self._integral + self._derivative)

        # setting previous variables
        self._last_error = error  # for next derivative term
        self._last_time = now
        self._last_input = input_  # will be used in differential_on_measurement mode, and for monitor purposes
        self._last_output = output  # no functional usage, just for monitoring

        return output

    def _calc(self, input_):
        '''
        calculates next pid value output (to try and make next input_ as close to .setpoint as possible)

        :NOTE: This function shouldn't be called by user at any time as it assumes constant dt, 
                aka it is called at VERY CONSTANT intervals!
                This is done by using the Timer Module in micropython device which 
                is automatically activated using the .activate() method.

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
        self._last_output = output  # no functional usage, just for monitoring
        self._last_input = input_  # will be used in differential_on_measurement mode and for monitoring purposes

        return output

    def execute(self, t):
        '''
        calls input_func to know current input
        calls _calc to calculate next output
        calls output_func to execute current output
        '''
        self.output_func(self._calc(self.input_func()))

    def activate(self, input_func=None, output_func=None, monitor=False):
        '''
        When this method is called a Hardware Timer block is used to call self._calc() at regular 
        time intervals. 

        :param input_func: the function to return the current input of the system
        :param output_func: the function to set plant output
        :param activating
        '''
        #  Reseting pid internal variables
        self.reset()

        # saving new input/output funcs if passed
        self.input_func = input_func if input_func is not None else self.input_func
        self.output_func = output_func if output_func is not None else self.output_func

        # Initializing the Hardware timer to start continiously calling .execute method
        if device_name == Device.ESP32S3:
            self._pid_timer = Timer(PID_TIMER_IND)
            self._pid_timer.init(period=PID.HARDWARE_TIMER_dt, mode=Timer.PERIODIC, callback=self.execute)

        elif device_name == Device.PICO:
            self._pid_timer = Timer(period=PID.HARDWARE_TIMER_dt, mode=Timer.PERIODIC, callback=self.execute)

        else:
            raise ValueError("unkonwn device")

        # activating Monitor if wanted
        if monitor:
            self.monitor()
        
    def deactivate(self):
        '''
        Deactivates the Timer and the constant calling of self._calc()
        '''
        self._pid_timer.deinit()
        self.output_func(min_output)

    def monitor(self, delay=200, exit_on_interrupt=False):
        '''
        :param delay: delay in ms between each print

        continiously print __repr__ for monitoring

        could be used with terminal command tee to pull data off the micorpython device
        '''
        try:
            while True:
                print(repr(self))
                sleep_ms(delay)

        except KeyboardInterrupt:
            if exit_on_interrupt:
                _thread.exit()
            else:
                return

    def __repr__(self):
        '''
        returns current internal variables states
        '''
        return f"""
           Current PID values:

            {{'_last_input': {self._last_input},
            'Kp': {self.Kp},
            'Ki': {self.Ki},
            'Kd': {self.Kd},
            'setpoint': {self.setpoint},
            '_proportional': {self._proportional},
            '_integral': {self._integral},
            '_derivative': {self._derivative},
            '_last_output': {self._last_output}}}
            """

def open_loop_step_response_test():
    '''
    This function will send step_signal of random lengths and in the same time 
    print the temperature sensor readings to terminal so that it can be logged 
    out with 'tee' command in terminal.

    This data will then be used for System Identification in Matlab :D

    The two cores of the microcontroller will be used:
        - one core will continiously print the current temperature to terminal
        - the second will execute the random step signals
    '''
    ### Core 1 constant just reading sensor and printing to terminal
    _thread.start_new_thread(thermistor.monitor, ())  

    ### Core 0 sending random step_signals
    time_changes = [2, 40, 40, 20, 10]
    # time_changes = [2, 2, 2, 2, 2]
    state = False
    for ind, time in enumerate(time_changes):
        print(f"\n\nHeater state: {state}\t")
        print(f"Time passed: {sum(time_changes[:ind])}\t\n")
        heater.state(state)
        sleep(time)
        state = not state

    # Test finished
    print('\n\nTest Finished!')
    _thread.exit()
    

### Main Routine ###
# Creating Input Object -> Thermistor
# Creating Control Algorithm Object -> PID
# Creating Output Object -> Heater

### Initialize Thermistor Object 
adc_pin = ADC_PIN[device_name]
Vcc = 3.28
R_10k = 9880
num_samples = 5
B_factor = 4300
R_nominal = 9360
room_temp = 25  # input here in celcuis, but in calculations, it will be converted to kelvin
thermistor = Thermistor(adc_pin, Vcc, R_10k, num_samples, B_factor, R_nominal, room_temp)

### Initialize Heater Object
pwm_pin = PWM_PIN[device_name]
heater = Heater(pwm_pin)

### Initialize PID Control Object
Kp = 22.4303
Ki = 0.005
Kd = 21.336
setpoint = 45  # degrees celcuis
min_output = 0  # output power unit in percentage
max_output = 100

pid = PID(Kp, Ki, Kd, setpoint, min_output, max_output, input_func=thermistor.read_T, output_func=heater.set)


if __name__ == '__main__':
    ### Activating System :)
    # for ampyrun
    # pid.activate(monitor=True)
    # open_loop_step_response_test()
    heater.set(100)
    thermistor.monitor()


