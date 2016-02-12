#!/usr/bin/env python
from gpiolib import *


def main():
    sys_init()
    try:
        while True:
            pin = input("pin to test: ")
            pinMode(pin, INPUT)
            print("pin: " + str(pin) + " value: " + str(digitalRead(pin)))
    except KeyboardInterrupt:
        sys_clean()
    print("\nCleaned up mmap\n")


if __name__ == '__main__':
    main()
