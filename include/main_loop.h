#ifndef __MAINLOOP_DEFINES_
#define __MAINLOOP_DEFINES_

#include "joystick.h"
#include "360_controller_map.h"
#include "robot_defines.h"

export read_joystick_event;
export button_update;
export axis_update;
export send_button_updates
export send_axis_updates

void main_loop( int *serial_port, int *joystick );

#endif
