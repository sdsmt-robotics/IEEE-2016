#!/usr/bin/env python
from gpiolib import *


def main():
    sys_init()

    pinMode(27, OUTPUT)
    digitalWrite(27, ON)
    print("pin: 27 value: ON")

    pinMode(24, INPUT)
    print("pin: 24 value: " + str(digitalRead(24)))

    sys_clean()  # clean up mmap in the C library


if __name__ == '__main__':
    main()
