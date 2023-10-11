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
    ON_OFF_FUNC = [Pin.on, PWM.duty_u16]
    STATE_FUNC = [Pin.value, PWM.duty]
    ON_VALUE = [[0, 1], [0, 65535]]
    OFF_VALUE = [[1, 0], [65535, 0]]
    VALUES = [OFF_VALUE, ON_OFF_FUNC]

    def __init__(self, pin: Pin = None, pwm: PWM = None, is_active_low: bool = None, default_duty_cycle=None):
        '''
        contstructor
        '''
        if pin is None and pwm is None:
            raise ValueError("Motor Pin must either be a normal pin or a pwm pin")

        if is_active_low is None:
            raise ValueError("Must speicify whether the motor pin is active high or active low")

        self.is_active_low = is_active_low

        if default_duty_cycle:
            self.default_duty_cycle = default_duty_cycle

        if pin:
            self.pin_pwm_mode = 0
            self.motor_pin = pin

        else:
            self.pin_pwm_mode = 1
            self.motor_pin = pwm

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
        MotorPin.ON_OFF_FUNC[self.pin_pwm_mode](self.motor_pin, MotorPin.ON_VALUE[self.pin_pwm_mode][is_active_low])

    def off(self):
        '''
        turns pin fully off, aka transistor doesn't allow current through

        This function is maps pin_pwm_mode and is_active_low to the correct function that will turn off the motor pin
        '''
        MotorPin.ON_OFF_FUNC[self.pin_pwm_mode](self.motor_pin, MotorPin.OFF_VALUE[self.pin_pwm_mode][is_active_low])

    def set(self, value: bool):
        '''
        turns pins to value
        
        This function is maps value(on or off), pin_pwm_mode and is_active_low to the correct function that will 
            turn the motor pin on or off 
        
        :param value: must be boolean-like value. e.g- 0/1 or False/True
        '''
        MotorPin.ON_OFF_FUNC[self.pin_pwm_mode](self.motor_pin, MotorPin.VALUES[value][self.pin_pwm_mode][is_active_low])

    def partial_on(self):
        '''
        Turns PWM pins to a pre-decided default_duty_cycle value,

        used for when a specific lower rpm is needed most of the time
        '''
        self.motor_pin.duty(self.default_duty_cycle)

    def duty(self, value: int):
        '''
        sets PWM pins to wanted duty cycle
        '''
        self.motor_pin.duty(value)

    def __repr__(self):
        return repr(self.motor_pin)

    def __str__(self):
        return str(self.motor_pin)

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
    They have .partial_on() for a 'default_duty_cycle' argument set by user
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
                 default_duty_cycle=512):
        '''
        Constructor
        '''
        # low-level Motor control pin variables
        self.default_duty_cycle = default_duty_cycle
        self._v1_pin = Pin(v1_pin_num, Pin.OUT)
        self._v2_pin = Pin(v2_pin_num, Pin.OUT)
        self._g1_pwm = PWM(Pin(g1_pin_num), freq=20000)
        self._g2_pwm = PWM(Pin(g2_pin_num), freq=20000)

        # User Motor control pins
        self.v1 = MotorPin(pin = self._v1_pin, is_active_low = v1_active_low)
        self.v2 = MotorPin(pin = self._v2_pin, is_active_low = v2_active_low)
        self.g1 = MotorPin(pwm = self._g1_pwm, is_active_low = g1_active_low)
        self.g2 = MotorPin(pwm = self._g2_pwm, is_active_low = g2_active_low)

        # Initializing with motor off
        self.off()

        # Default state is Clockwise
        self._cw_ccw: bool = Dir.CW 

        # class dictionary to map cw_ccw value to corresponding activating cw/ccw method
        self.cw_values = {Dir.CW: self.cw, Dir.CCW: self.ccw}


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
        return (self.v1.state() and self.g2.state()) or (self.v2.state() and self.g1.state())


    def cw(self):
        '''
        clockwise motion
        #TODO: implement gradual duty_cycle incrementing to avoid motor sudden vibrations
        '''
        # first closing the circuit to ensure no dead-time short
        self.v1.off()
        self.v2.off()

        # openning n1 and p2
        self.v1.on()
        self.v2.off()
        self.g1.off()
        self.g2.on()

        # Saving current state
        self._cw_ccw = True

    def ccw(self):
        '''
        anti-clockwise motion
        '''
        # first closing the circuit to ensure no dead-time short
        self.v1.off()
        self.v2.off()

        # openning n2 and p1
        self.v1.off()
        self.v2.on()
        self.g1.on()
        self.g2.off()

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
            return f"Motor is ON, moving {string_dir}"

        else:
            return "Motor is OFF"


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

    def _ISR(self, x):
        '''
        I Must call the _motor_func from this to add the buffer argument

        This does add a function call to the ISR
        #TODO: make specific method in Motor for calling in ISR
        '''
        self._motor_func()

        # As measured from oscilloscope the limit switches bounces for about 700 ms
        # So to deal with it, will deactivate IRQ for 1ms
        #TODO: fix this 
        # self.irq_pin.irq(handler=None)
        # self.timer0 = Timer(0)
        # self.timer0.init(period=10, mode=Timer.ONE_SHOT, callback=lambda t: self.activate_irq)

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
        pass

    def off(self):
        '''
        deactivate motor and IRQ
        '''
        motor.off()

# Object Initializations
motor = Motor(4, 5, 6, 7, v1_active_low=False, v2_active_low=False, g1_active_low=True, g2_active_low=True)
limit_sw1 = LimitSwitch(15, motor, Dir.CW)
limit_sw2 = LimitSwitch(16, motor, Dir.CCW)

