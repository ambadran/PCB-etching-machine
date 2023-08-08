'''
Refer to iPad notes Project 'PCB-etching machine' for detailed math and explanations throughout
'''
from machine import Pin, PWM, ADC
from math import log


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

        NOTE: it also inverts the percentage (65335-value_u_16) as the pwm value is put through an inverter npn first for buffering the darlington npn
        '''
        self.pwm.duty_u16((65335-value_u_16))

    def set_heat(self, percentage):
        '''
        controls pwm with percentage out of 100% instead of 65535

        NOTE: it also inverts the percentage (100-percentage) as the pwm value is put through an inverter npn first for buffering the darlington npn
        '''
        self.pwm.duty_u16(int(((100-percentage)/100)*65335))

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

        


# Global Variables
Vcc = 3.28
R_10k = 9880
num_samples = 5
B_factor = 4300
R_nominal = 9360
room_temp = 25  # input here in celcuis, but in calculations, it will be converted to kelvin

heater = Heater()
thermistor = Thermistor(Vcc, R_10k, num_samples, B_factor, R_nominal, room_temp)
