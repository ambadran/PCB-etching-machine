from machine import Pin, UART
import machine
from time import ticks_us, ticks_diff, ticks_add, sleep
import _thread
import sys

machine.freq(250000000)

uart = UART(0, baudrate=115200, tx=Pin(12), rx=Pin(1), timeout_char=50, invert=UART.INV_TX)

# Second core will only print whatever read on terminal
def print_read():
    '''
    reading tx pin from PIC microcontroller and printing it

    This will be done on the second core
    '''
    while True:
        # while uart.read(1) != b'\n':
        #     pass
        value = uart.read()
        if value is None:
            continue
        print(value.decode())
        sleep(0.05)

def test():
    uart.send('A\n')
    while True:
        value = uart.read()
        if value is not None:
            print(value)

def start():
    _thread.start_new_thread(print_read, ())

def send(command):
    uart.write(f"{command}\n")


def main():
    '''
    #TODO: figure why the fuck it doesn't work with ampyrun?!??!
    as of now it will only work if called from within REPL
    '''
    # Core1 to only read inputs :D
    start()

    # Core0 to only wait for user to send stuff :D
    while True:
        sleep(0.2)
        k = input('-> ')
        send(k)

if __name__ == '__main__':
    main()