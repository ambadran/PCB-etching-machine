'''
Timer0 is used for limit swtich debouncing
Timer1 is used for motor pins PWM sweep functions
'''
from machine import Pin, PWM, Timer
from time import sleep_ms

class Dir:
    '''
    acts like an enum
    '''
    CW = True
    CCW = False

class MotorPin:
    '''
    class to handle Motor pins
    '''

    # Mapping everything to everything :D
    ON_OFF_FUNC = [Pin.value, PWM.duty_u16]
    STATE_FUNC = [Pin.value, PWM.duty]
    ON_VALUE = [[1, 0], [65535, 0]]
    OFF_VALUE = [[0, 1], [0, 65535]]
    VALUES = [OFF_VALUE, ON_FUNC]

    # other constants
    SAMPLE_TIME = 1  # ms 

    def __init__(self, pin: Pin = None, pwm: PWM = None, is_active_low: bool = None, maximum_duty_cycle=None):
        '''
        contstructor
        '''
        if pin is None and pwm is None:
            raise ValueError("Motor Pin must either be a normal pin or a pwm pin")

        if is_active_low is None:
            raise ValueError("Must speicify whether the motor pin is active high or active low")

        self.is_active_low = is_active_low

        if maximum_duty_cycle:
            self.maximum_duty_cycle = maximum_duty_cycle

        else:
            # must initialize internal variable nonetheless
            self._maximum_duty_cycle = 65535


        if pin:
            self.pin_pwm_mode = 0
            self.motor_pin = pin

        else:
            self.pin_pwm_mode = 1
            self.motor_pin = pwm

    @property
    def maximum_duty_cycle(self):
        '''
        duty cycle when transistor is ON
        '''
        return self._maximum_duty_cycle

    @maximum_duty_cycle.setter
    def maximum_duty_cycle(self, value):
        '''
        value in 0 to 65535
        '''
        self._maximum_duty_cycle = value

        MotorPin.ON_VALUE[1][0] = value
        MotorPin.OFF_VALUE[1][1] = 65535 - value  # it's inverted!!

        # if the transistor is already on, must call function again with updated value
        self.set(self.state)

    @property
    def state(self):
        '''
        returns whether current pin is on/off, transistor is allowing current or not

        #TODO: optimize 
            remove the not not and use a bitwise operation to return 1 if the output is >1 or 0
        '''
        return (not not MotorPin.STATE_FUNC[self.pin_pwm_mode](self.motor_pin)) ^ self.is_active_low

    def on(self):
        '''
        turns pin fully on, aka transistor allow current through

        This function is maps pin_pwm_mode and is_active_low to the correct function that will turn on the motor pin
        '''
        MotorPin.ON_OFF_FUNC[self.pin_pwm_mode](self.motor_pin, MotorPin.ON_VALUE[self.pin_pwm_mode][self.is_active_low])

    def off(self):
        '''
        turns pin fully off, aka transistor doesn't allow current through

        This function is maps pin_pwm_mode and is_active_low to the correct function that will turn off the motor pin
        '''
        MotorPin.ON_OFF_FUNC[self.pin_pwm_mode](self.motor_pin, MotorPin.OFF_VALUE[self.pin_pwm_mode][self.is_active_low])

    def set(self, value: bool):
        '''
        turns pins to value
        
        This function is maps value(on or off), pin_pwm_mode and is_active_low to the correct function that will 
            turn the motor pin on or off 
        
        :param value: must be boolean-like value. e.g- 0/1 or False/True
        '''
        MotorPin.ON_OFF_FUNC[self.pin_pwm_mode](self.motor_pin, MotorPin.VALUES[value][self.pin_pwm_mode][self.is_active_low])

    def duty(self, value: int):
        '''
        sets PWM pins to wanted duty cycle
        '''
        self.motor_pin.duty(value)

    def _update_pwm_duty_cycle(self, x):
        '''
        :param x: redundant variable for Timer class 
        ISR function for Timer1, gets executed every MotorPin.sample_time, until samples list is finished
        '''
        self.motor_pin.duty_u16(self.samples[self.current_index])
        self.current_index += 1
        if self.current_index >= self.num_samples:
            self.timer.deinit()

    def sweep(self, sweep_time: int):
        '''
        :time in ms!!!!
        sweeps PWM to wanted signal
        '''
        m = 1/sweep_time
        self.num_samples = ceil(sweep_time/MotorPin.sample_time)

        self.samples = []
        for ind in range(1, num_samples+1):
            self.samples.append(int(m*ind*65535))

        self.current_index = 0
        self.timer = Timer(1, Mode=Timer.PERIODIC, period=MotorPin.sample_time, callback=self._update_pwm_duty_cycle)

    def __repr__(self):
        return f"{repr(self.motor_pin)}, state: {self.state}"

    def __str__(self):
        return f"{str(self.motor_pin)}, state: {self.state}"

class Motor:
    '''
    H-Bridge controller for DC motors

    Pin definitions:

    v1          v2
    |           |
    |           |
    |           |
    +---o   o---+
    |           |
    |           |
    |           |
    g1          g2

    This class ultimate goal is let user turn on/off each pin REGARDLESS of whether they are active low/high
    using vx or gx .on() or .off()
    
    Also all pins have .state(value) where value is (0 or 1) or (False or True)
    
    Also g1 and g2 are actually PWM pins so their .on() is just 100% duty cycle,
    They have .partial_on() for a 'maximum_duty_cycle' argument set by user
    They also have .duty(value) to set a particular duty_cycle


    #TODO: rename p1, p2, n1, n2 to pins 1, 2, 3, 4
        this is to include h-bridges made of 4 npns for example

        differentiate between the underlying self.p1 (should be renamed to self._p1)
        and the wanted transistor control

        should only be controlled with .on() or .off(), the active high/active low should be
        pre-choosen by the user

        make .partial_on() for pwm pins (or sth)

    '''

    def __init__(self, v1_pin_num: int, v2_pin_num: int, g1_pin_num: int, g2_pin_num: int, 
                 v1_active_low, v2_active_low, g1_active_low, g2_active_low,
                 maximum_duty_cycle: int = None, sweep_time: int = 0):
        '''
        Constructor
        :param maximum_duty_cycle: its default is set in class variables ON_VALUE and OFF_VALUE in MotorPin, if it's assigned it will be assigned there too
        :param sweep_time: in ms, is the time where PWM turns from one state to the other
        '''
        # low-level Motor control pin variables
        self._v1_pin = Pin(v1_pin_num, Pin.OUT)
        self._v2_pin = Pin(v2_pin_num, Pin.OUT)
        self._g1_pwm = PWM(Pin(g1_pin_num), freq=20000)
        self._g2_pwm = PWM(Pin(g2_pin_num), freq=20000)

        # User Motor control pins
        self.v1 = MotorPin(pin = self._v1_pin, is_active_low = v1_active_low)
        self.v2 = MotorPin(pin = self._v2_pin, is_active_low = v2_active_low)
        self.g1 = MotorPin(pwm = self._g1_pwm, is_active_low = g1_active_low)
        self.g2 = MotorPin(pwm = self._g2_pwm, is_active_low = g2_active_low)
        if maximum_duty_cycle:
            self.maximum_duty_cycle = maximum_duty_cycle

        else:
            # must initialize internal variable nonetheless
            self._maximum_duty_cycle = 65535

        # sweep time is the time where PWM turns from one state to the other
        self.sweep_time = sweep_time 

        # Initializing with motor off
        self.off()

        # Default state is Clockwise
        self._cw_ccw: bool = Dir.CW 

        # class dictionary to map cw_ccw value to corresponding activating cw/ccw method
        self.cw_values = {Dir.CW: self.cw, Dir.CCW: self.ccw}

    @property
    def maximum_duty_cycle(self):
        '''
        duty cycle when transistor is ON
        '''
        return self._maximum_duty_cycle

    @maximum_duty_cycle.setter
    def maximum_duty_cycle(self, value):
        '''
        value in 0 to 65535
        '''
        self._maximum_duty_cycle = value
        self.g1.maximum_duty_cycle = value
        self.g2.maximum_duty_cycle = value

    def on(self):
        '''
        activates H-bridge to the last known cw_ccw value
        '''
        self.direction = self._cw_ccw

    def off(self):
        '''
        deactivates H-bridge
        '''
        # Turning off any power source
        self.v1.off()
        self.v2.off()

        # Grounding both motor pins
        self.g1.on()
        self.g2.on()

    @property
    def is_on(self):
        '''
        returns whether motor is on or off
        '''
        return (self.v1.state and self.g2.state) or (self.v2.state and self.g1.state)


    def cw(self):
        '''
        clockwise motion
        #TODO: implement gradual duty_cycle incrementing to avoid motor sudden vibrations
        '''
        # first closing the circuit to ensure no dead-time short
        self.off()

        # setting all pins that won't sweep
        self.v1.on()
        self.v2.off()
        self.g1.off()

        # sweeping (if there is a sweep_time value)
        self.g2.sweep(self.sweep_time)

        # Saving current state
        self._cw_ccw = True

    def ccw(self):
        '''
        anti-clockwise motion
        '''
        # first closing the circuit to ensure no dead-time short
        self.off()

        # setting all pins that won't sweep
        self.v1.off()
        self.v2.on()
        self.g2.off()

        # sweeping (if there is a sweep_time value)
        self.g1.sweep(self.sweep_time)

        # Saving current state
        self._cw_ccw = False

    @property
    def direction(self):
        '''
        returns current cw_ccw
        '''
        return self._cw_ccw

    @direction.setter
    def direction(self, value: bool):
        '''
        sets motor direction
        '''
        self.cw_values[value]()

    def toggle_dir(self):
        '''
        toggles the direction of motor
        '''
        self.direction = not self._cw_ccw

    def __repr__(self):
        '''
        also return string representation :D
        '''
        return str(self)

    def __str__(self):
        '''
        string representation
        '''
        if self.is_on:
            string_dir = 'CW' if self._cw_ccw else 'CCW'
            return f"Motor is ON, moving {string_dir}, V1: {self.v1.state}, V2: {self.v2.state}, G1: {self.g1.state}, G2: {self.g2.state}"

        else:
            return "Motor is OFF, V1: {self.v1.state}, V2: {self.v2.state}, G1: {self.g1.state}, G2: {self.g2.state}"



class LimitSwitch:
    '''
    Class to handle interfacing of limit switches

    NB: ISR here is specifically implemented to change motor direction to a specific orientation
    '''
    def __init__(self, pin_num: int, motor_object: Motor, motor_dir: bool):
        '''
        constructor
        '''
        # initializing pin
        self.irq_pin = Pin(pin_num, Pin.IN)

        # Setting interrupt service routine method
        self.motor: Motor = motor_object

        if motor_dir == Dir.CW:
            self._motor_func = self.motor.cw

        elif motor_dir == Dir.CCW:
            self._motor_func = self.motor.ccw

        # Activating IRQ pin
        self.activate_irq()
        # self.irq_pin.irq(handler = self._ISR, trigger = Pin.IRQ_FALLING)

    def _fake_ISR(self, x):
        '''
        There is no clear way to disable a specific IRQ, and disabling all IRQ compromises rshell
        This is a nop function to execute for when I don't want the IRQ
        '''
        pass

    def _ISR(self, x):
        '''
        I Must call the _motor_func from this to add the buffer argument

        This does add a function call to the ISR
        #TODO: make specific method in Motor for calling in ISR
        '''
        self._motor_func()
        print('irq happenned')  #TODO: remove this after extensive testing

        # As measured from oscilloscope the limit switches bounces for about 700 ms
        # So to deal with it, will deactivate IRQ for 1ms
        self.deactivate_irq()
        self.timer0 = Timer(0)
        self.timer0.init(period=10, mode=Timer.ONE_SHOT, callback=lambda t: self.activate_irq)
        # sleep_ms(10)
        # self.activate_irq

    def deactivate_irq(self):
        self.irq_pin.irq(handler=self._fake_ISR)

    def activate_irq(self):
        self.irq_pin.irq(handler = self._ISR, trigger = Pin.IRQ_FALLING)


class StirringMechanism:
    '''
    Class to handle the Whole Mechanism
    '''
    def __init__(self, motor: Motor, limit_sw1: LimitSwitch, limit_sw2: LimitSwitch):
        '''
        Constructor
        '''
        self.motor = motor
        self.limit_sw1 = limit_sw1
        self.limit_sw2 = limit_sw2

    def on(self):
        '''
        activate IRQ
        '''
        self.limit_sw1.activate_irq()
        self.limit_sw2.activate_irq()

    def off(self):
        '''
        deactivate motor and IRQ
        '''
        self.limit_sw1.deactivate_irq()
        self.limit_sw2.deactivate_irq()
        motor.off()

    def monitor(self):
        '''
        prints current motor dir and last limit switch triggered
        '''
        while True:
            print(f"Motor dir: {self.motor._cw_ccw \r"}

# Object Initializations
sweep_time = 100  # in ms
motor = Motor(4, 5, 6, 7, v1_active_low=False, v2_active_low=False, g1_active_low=True, g2_active_low=True, sweep_time=100)
limit_sw1 = LimitSwitch(15, motor, Dir.CW)
limit_sw2 = LimitSwitch(16, motor, Dir.CCW)
sr = StirringMechanism(motor, limit_sw1, limit_sw2)

