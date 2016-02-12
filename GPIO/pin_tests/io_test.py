#!/usr/bin/env python
from gpiolib import *


def main_loop():
    while True:
        pin = int(raw_input('pin (13-27): '))
        mode = raw_input('mode (INPUT/OUTPUT): ')
        if mode.lower() in ["o", "output", "out"]:
            value = int(raw_input('value (1/0): '))
            pinMode(pin, OUTPUT)
            digitalWrite(pin, value)
        elif mode.lower() in ["i", "input", "in"]:
            pinMode(pin, INPUT)
            print "pin: ", pin, " val: ", digitalRead(pin)
        else:
            print mode, "not 'INPUT' or 'OUTPUT'"


def main():
    sys_init()
    try:
        main_loop()
    except KeyboardInterrupt:
        print("\n\nExiting...\n\n")
    sys_clean()


if __name__ == '__main__':
    main()
