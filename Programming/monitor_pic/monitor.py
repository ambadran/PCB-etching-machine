from machine import Pin
import machine
from time import ticks_us, ticks_diff, ticks_add
machine.freq(250000000)

class PinMonitor:
    '''
    A class to monitor a pin on my pic
    '''
    def __init__(self, pin_num):
        '''
        constructor
        '''
        self.reader = Pin(pin_num, Pin.IN)

    @property
    def state(self):
        '''
        Pin instantanious state
        '''
        return self.reader.value()

    @property
    def frequency(self):
        '''
        measures frequency at pin

        IMPORTANT: return in kHz
        '''
        repetitions = 20

        differences = []
        for _ in range(repetitions):
            while self.reader.value():
                pass
            falling_edge1 = ticks_us()
            while not self.reader.value():
                pass
            rising_edge = ticks_us()
            while self.reader.value():
                pass
            falling_edge2 = ticks_us()

            differences.append(ticks_diff(falling_edge2, falling_edge1))

        difference = sum(differences)/20
        
        return round(1/(difference/1000000))

    def persist(self, attrib):
        '''
        prints an attribute forever until KeyBoardInterrupt
        '''
        if attrib == 0:
            while True:
                print(f"Pin State: {self.state}\r", end='')

        elif attrib == 1:
            while True:
                print(f"Frequency: {self.frequency} Hz\r", end='')


p0 = PinMonitor(0)
p1 = PinMonitor(1)

if __name__ == '__main__':
    p1.persist(1)
