#include "../include/joystick.h"
#include "../include/360_controller_map.h"
#include "../include/locomotion.h"
#include <stdio.h>
#include <limits.h>

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    //does mappy things
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned char map_stick(short input)
{
    /*
     * Maps a raw input to input our code knows how to work with.
     */
    return map(input, SHRT_MIN, SHRT_MAX, 0, 255); //centerpoint is at 127;
}

void button_update(struct js_event *jse, int *button_update_array)
{
	button_update_array[jse->number] = jse->value;
	return;
}

void axis_update(struct js_event *jse, int *axis_update_array)
{
	axis_update_array[jse->number] = jse->value;
	return;
}

int  send_button_updates(int *old_button_array, int *new_button_array, int serial_file)
{
	int i;
	for(i = 0; i < BUTTON_COUNT; i++)
		if(new_button_array[i] != old_button_array[i])
		{
			update_button(i, new_button_array[i], serial_file);
			old_button_array[i] = new_button_array[i];
		}
	return 0;
}

int  send_axis_updates(int *old_axis_array, int *new_axis_array, int serial_port)
{
	int i;
	for(i = 0; i < AXIS_COUNT; i++)
		if(new_axis_array[i] != old_axis_array[i])
		{
			update_axis(i, new_axis_array[i], serial_port);
			old_axis_array[i] = new_axis_array[i];
		}
	return 0;
}

int update_button(int button, int button_state, int serial_port)
{
    /*
     * The R/L bumpers are either on or off.
     */
	switch(button)
	{
		case BUTTON_LEFT_BUMPER:
			if(button_state == 1)
			{
				printf("Driving left motor.\n");
                setWheelSpeed( LEFT, 255, serial_port );
			}
			if(button_state == 0)
			{
                setWheelSpeed( LEFT, 127, serial_port );
			}
			break;
		case BUTTON_RIGHT_BUMPER:
			if(button_state == 1)
			{
				printf("Driving right motor.\n");
				setWheelSpeed( RIGHT, 255, serial_port );
			}
			if(button_state == 0)
			{
				setWheelSpeed( RIGHT, 127, serial_port );
			}
			break;
	}
	return 0;
}

int update_axis(int axis, int axis_value, int serial_file)
{
	unsigned char value = 0;
	switch(axis)
	{
		case AXIS_LEFT_STICK_VERTICAL:
			value = map_stick(axis_value);

            //keeps stuff in the deadzone from being sent.
			if(value > 127 && (value - DEADZONE) < 127)
				value = 127;
			if(value < 127 && (value + DEADZONE) > 127)
				value = 127;

			printf("Driving left motor to speed: %i\n", value);
            setWheelSpeed( LEFT, value, serial_file );
			break;

		case AXIS_RIGHT_STICK_VERTICAL:
			value = map_stick(axis_value);

            //keeps stuff in the deadzone from being sent.
			if(value > 127 && (value - DEADZONE) < 127)
				value = 127;
			if(value < 127 && (value + DEADZONE) > 127)
				value = 127;

			printf("Driving right motor to speed: %i\n", value);
            setWheelSpeed( RIGHT, value, serial_file );
			break;
	}
	return 0;
}

int open_joystick(char *joystick_location)
{
	joystick_fd = open(joystick_location, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */
	if (joystick_fd < 0)
		return joystick_fd;

	/* maybe ioctls to interrogate features here? */

	return joystick_fd;
}

int read_joystick_event(struct js_event *jse)
{
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse));

	if (bytes == -1)
		return 0;

	if (bytes == sizeof(*jse))
		return 1;

	printf("Unexpected bytes from joystick:%d\n", bytes);

	return -1;
}

void close_joystick()
{
	close(joystick_fd);
}

int get_joystick_status(struct wwvi_js_event *wjse)
{
	int rc;
	struct js_event jse;
	if (joystick_fd < 0)
		return -1;

	// memset(wjse, 0, sizeof(*wjse));
	while ((rc = read_joystick_event(&jse) == 1)) {
		jse.type &= ~JS_EVENT_INIT; /* ignore synthetic events */
		if (jse.type == JS_EVENT_AXIS) {
			switch (jse.number) {
			case 0: wjse->stick1_x = jse.value;
				break;
			case 1: wjse->stick1_y = jse.value;
				break;
			case 2: wjse->stick2_x = jse.value;
				break;
			case 3: wjse->stick2_y = jse.value;
				break;
			default:
				break;
			}
		} else if (jse.type == JS_EVENT_BUTTON) {
			if (jse.number < 10) {
				switch (jse.value) {
				case 0:
				case 1: wjse->button[jse.number] = jse.value;
					break;
				default:
					break;
				}
			}
		}
	}
	// printf("%d\n", wjse->stick1_y);
	return 0;
}
