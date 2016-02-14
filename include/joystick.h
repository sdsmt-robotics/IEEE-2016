/*
    (C) Copyright 2007,2008, Stephen M. Cameron.

    This file is part of wordwarvi.

    wordwarvi is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    wordwarvi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with wordwarvi; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 */
#include <stdio.h>
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define JOYSTICK_DEVNAME "/dev/input/js0"

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */


struct js_event {
	unsigned int time;	/* event timestamp in milliseconds */
	short value;   /* value */
	unsigned char type;     /* event type */
	unsigned char number;   /* axis/button number */
};

struct wwvi_js_event {
	int button[11];
	int stick1_x;
	int stick1_y;
	int stick2_x;
	int stick2_y;
};

static int joystick_fd = -1;



int update_axis(int axis, int axis_value, int serial_file);
int update_button(int button, int button_state, int serial_port);
int  send_axis_updates(int *old_axis_array, int *new_axis_array, int serial_port);
int  send_button_updates(int *old_button_array, int *new_button_array, int serial_file);
long map(long x, long in_min, long in_max, long out_min, long out_max);
unsigned char map_stick(short input);
void button_update(struct js_event *jse, int *button_update_array);
void axis_update(struct js_event *jse, int *axis_update_array);
int open_joystick(char *joystick_location);
int read_joystick_event(struct js_event *jse);
void close_joystick();
int get_joystick_status(struct wwvi_js_event *wjse);


#endif
