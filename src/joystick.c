#include "../include/joystick.h"
#include "../include/360_controller_map.h"
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
