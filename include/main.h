#ifndef __MAIN_DEFINES_
#define __MAIN_DEFINES_

#include <stdio.h>

//our definitions
#include "serial.h"
#include "main_loop.h"
#include "robot_defines.h"

//functions
export main_loop;

void sys_init( int *serial_port, int *joystick );

#endif
