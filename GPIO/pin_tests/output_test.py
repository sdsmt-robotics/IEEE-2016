#!/usr/bin/env python
from gpiolib import *


def main():
    sys_init()
    try:
        while True:
            pin = input('pin to test: ')
            state = input('1/0: ')
            pinMode(pin, OUTPUT)
            digitalWrite(pin, state)
    except KeyboardInterrupt:
        sys_clean()
    print("\ncleaned up mmap\n")

if __name__ == '__main__':
    main()
