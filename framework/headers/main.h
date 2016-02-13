#ifndef __MAIN_DEFINES_
#define __MAIN_DEFINES_

#include <stdio.h>

//our definitions
#include "serial.h"
#include "locomotion.h"
#include "main_loop.h"
#include "joystick.h"
#include "360_controller_map.h"
#include "navigation.h"
#include "robot_defines.h"

//functions
void sys_init( int &serial_port, int &joystick );

#endif
