from machine import Pin, PWM, Timer
from time import sleep_ms

class Motor:
    '''
    H-Bridge controller for DC motors

    H-bridge is two pnp and two npn
    NOTE: pnp is active high, npn is active low
    '''
    CW = True
    CCW = False

    def __init__(self, p1_pin_num: int, p2_pin_num: int, n1_pin_num: int, n2_pin_num: int, default_duty_cycle=512):
        '''
        Constructor
        '''
        # Motor control pins
        self.p1 = Pin(p1_pin_num, Pin.OUT)
        self.p2 = Pin(p2_pin_num, Pin.OUT)
        # self.n1 = Pin(n1_pin_num, Pin.OUT)
        # self.n2 = Pin(n2_pin_num, Pin.OUT)
        self.n1 = PWM(Pin(n1_pin_num), freq=20000)
        self.n2 = PWM(Pin(n2_pin_num), freq=20000)

        # Initializing with motor off
        self.off()

        # Default state is Clockwise
        self._cw_ccw: bool = Motor.CW 

        # class dictionary to map cw_ccw value to corresponding activating cw/ccw method
        self.cw_values = {Motor.CW: self.cw, Motor.CCW: self.ccw}

        self.default_duty_cycle = default_duty_cycle

    def on(self):
        '''
        activates H-bridge to the last known cw_ccw value
        '''
        self.direction = self._cw_ccw

    def off(self):
        '''
        deactivates H-bridge
        '''
        self.p1.off()
        self.p2.off()
        # self.n1.on()  # Remove this after checking everything
        # self.n2.on()  # Remove this after checking everything
        self.n1.duty(1023)  #TODO: test this if it's actually 100% duty cycle
        self.n2.duty(1023)  #TODO: test this if it's actually 100% duty cycle

    @property
    def is_on(self):
        '''
        returns whether motor is on or off
        '''
        # return (self.p1.value() or self.p2.value()) or not (self.n1.value() and self.n2.value())
        return self.p1.value() or self.p2.value() or (self.n1.duty() != 1023) or (self.n2.duty() != 1023)

    def cw(self):
        '''
        clockwise motion
        #TODO: implement gradual duty_cycle incrementing to avoid motor sudden vibrations
        '''
        # first closing the circuit to ensure no dead-time short
        # self.n1.on()
        # self.n2.on()
        self.n1.duty(1023)  #TODO: test this if it's actually 100% duty cycle
        self.n2.duty(1023)  #TODO: test this if it's actually 100% duty cycle

        # openning n1 and p2
        self.p1.on()
        self.p2.off()
        # self.n1.on()
        # self.n2.off()
        self.n1.duty(self.default_duty_cycle)
        self.n2.duty(0)

        # Saving current state
        self._cw_ccw = True

    def ccw(self):
        '''
        anti-clockwise motion
        '''
        # first closing the circuit to ensure no dead-time short
        # self.n1.on()
        # self.n2.on()
        self.n1.duty(1023)  #TODO: test this if it's actually 100% duty cycle
        self.n2.duty(1023)  #TODO: test this if it's actually 100% duty cycle

        # openning n2 and p1
        self.p1.off()
        self.p2.on()
        # self.n1.off()
        # self.n2.on()
        self.n1.duty(0)
        self.n2.duty(self.default_duty_cycle)

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

        if motor_dir == Motor.CW:
            self._motor_func = self.motor.cw

        elif motor_dir == Motor.CCW:
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
motor = Motor(4, 5, 6, 7)
limit_sw1 = LimitSwitch(15, motor, Motor.CW)
limit_sw2 = LimitSwitch(16, motor, Motor.CCW)

