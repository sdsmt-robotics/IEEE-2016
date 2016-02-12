import gpio as g

XU_GPIO_ADDR = 0x13400000  # Odroid-XU base GPIO address

# To be used outside of the wrapper.
OUTPUT = 1
INPUT = 0
ON = 1
OFF = 0

# used in pinMode, discuss if necessary
PULLDS = 0  # disable pull up/down
PULLUP = 1  # enable pull up
PULLDN = 2  # enable pull down

# maps the GPIO pin to the memory mapped offset and bit
# These are the GPIO/external interrupt pins
gpio_addresses = {13: [0x0c24, 5], 14: [0x0c44, 3], 15: [0x0c24, 2],
                  16: [0x0c24, 0], 17: [0x0c24, 6], 18: [0x0c24, 3],
                  19: [0x0c44, 6], 20: [0x0c44, 4], 21: [0x0c44, 5],
                  22: [0x0c44, 7], 23: [0x0c44, 2], 24: [0x0c44, 1],
                  25: [0x0c24, 7], 26: [0x0c44, 0], 27: [0x0c64, 1]}


def check_pin(pin):
    return pin in range(13, 28)  # 13..27 are only valid GPIO pins


def sys_init():
    """Initialize everything"""
    g.sys_init()


def sys_clean():
    """Cleans up mmap"""
    g.sys_clean()


def pinMode(pin, mode=INPUT):
    """Set `pin` to `mode` INPUT or OUTPUT. Defaults to INPUT."""
    if check_pin(pin):
        g.pinMode(gpio_addresses[pin][0], gpio_addresses[pin][1], PULLDS, mode)
    else:
        raise ValueError("Can only use pins 13-27. Attempted to use pin: " + str(pin))


def digitalWrite(pin, state=OFF):
    """Write ON or OFF to `pin`. Defaults to OFF."""
    if check_pin(pin):
        g.digitalWrite(gpio_addresses[pin][0], gpio_addresses[pin][1], state)
    else:
        raise ValueError("Can only use pins 13-27. Attempted to use pin: " + str(pin))


def digitalRead(pin):
    """Read ON or OFF from `pin`. Returns integer 0 or 1."""
    if check_pin(pin):
        return g.digitalRead(gpio_addresses[pin][0], gpio_addresses[pin][1])
    else:
        raise ValueError("Can only use pins 13-27. Attempted to use pin: " + str(pin))
