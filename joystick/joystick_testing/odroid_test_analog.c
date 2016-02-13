#include <stdio.h>
#include <limits.h>
#include "joystick.h"
#include "robot_defines.h"
#include "easy_serial.h"
#include "360_controller_map.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned char map_stick(short input)
{
  //printf("%li", map(input, SHRT_MIN, SHRT_MAX, -127, 127));
  return map(input, SHRT_MIN, SHRT_MAX, 0, 255); //centerpoint @ 127;
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
	int i = 0;
	for(i = 0; i < BUTTON_COUNT; i++)
		if(new_button_array[i] != old_button_array[i])
		{
			update_button(i, new_button_array[i], serial_file);
			old_button_array[i] = new_button_array[i];
		}
	return 0;
}
int  send_axis_updates(int *old_axis_array, int *new_axis_array, int serial_file)
{
	int i = 0;
	for(i = 0; i < AXIS_COUNT; i++)
		if(new_axis_array[i] != old_axis_array[i])
		{
			update_axis(i, new_axis_array[i], serial_file);
			old_axis_array[i] = new_axis_array[i];
		}
	return 0;
}

int send_command(unsigned char *flag, unsigned char *value, int serial_file)
{
	unsigned char received = 0;
	clearPort(serial_file);
	write(serial_file, flag, 1);
	write(serial_file, value, 1);
//	while(received != ARDUINO_RECEIVED_BYTE);
//		read(serial_file, &received, 1);
	return 0;
}

int update_button(int button, int button_state, int serial_file)
{
	unsigned char flag = 0;
	char value = 0;
	switch(button)
	{
		case BUTTON_LEFT_BUMPER:
			flag = LEFT_MOTOR_FLAG;
			if(button_state == 1)
			{
				value = 255;
				printf("Driving left motor.\n");
				send_command(&flag, &value, serial_file);//do left bumper pressed thing
			}
			if(button_state == 0)
			{
				value = 127;
				send_command(&flag, &value, serial_file);//do left bumper released things
			}
			break;
		case BUTTON_RIGHT_BUMPER:
			flag = RIGHT_MOTOR_FLAG;
			if(button_state == 1)
			{
				value = 255;
				printf("Driving right motor.\n");
				send_command(&flag, &value, serial_file);//do left bumper pressed thing
			}
			if(button_state == 0)
			{
				value = 127;
				send_command(&flag, &value, serial_file);		
			}
			break;
	}
	return 0;
}
		
int update_axis(int axis, int axis_value, int serial_file)
{
	unsigned char flag = 0;
	unsigned char value = 0;
	switch(axis)
	{
		case AXIS_LEFT_STICK_VERTICAL:
			flag = LEFT_MOTOR_FLAG;
			value = map_stick(axis_value);
			if(value > 127 && (value - DEADZONE) < 127) //keeps stuff in the deadzone from being sent.
				value = 127;
			if(value < 127 && (value + DEADZONE) > 127)
				value = 127; 
			printf("Driving left motor to speed: %i\n", value);
			send_command(&flag, &value, serial_file);//do left stick up down thing
			break;
		case AXIS_RIGHT_STICK_VERTICAL:
			flag =RIGHT_MOTOR_FLAG;
			value = map_stick(axis_value);
			if(value > 127 && (value - DEADZONE) < 127) //keeps stuff in the deadzone from being sent.
				value = 127;
			if(value < 127 && (value + DEADZONE) > 127)
				value = 127; 
			printf("Driving right motor to speed: %i\n", value);
			send_command(&flag, &value, serial_file);//do right stick up down thing
			break;
	}
	return 0;
}		



/* a little test program */
int main(int argc, char *argv[])
{
	int  joy_file, received, serial_file; // file IDs for the joy, in serial, and out serial ports
	char joy_address[32] = "/dev/input/";
	int  old_axis_values[8] = {0};     // intialize all buttons to "off" (0) this array is check edagainst for button updates and if an update is found the update is sent. Axis stuff is the same
	int  old_button_values[11] = {0};  // array as mentioned above     
	int  new_button_values[11] = {0};  // just look above 
	int  new_axis_values[8] = {0};     // just look above the above 
		
	struct js_event jse;               // stores the joystick data


	if(argc > 2)
	{
		printf("Too many arguments, exiting."); // only one arg for the time being. 
		exit(1);
	}
	if(argc == 2)
	{
		strcat(joy_address, argv[1]);
		joy_file = open_joystick(joy_address); // sets the joystick to attach to, assumes that the arduino is mapped as seen in the ROBOT_DEFINITIONS.h
	}
	else if(argc == 1)
	{
		joy_file = open_joystick("/dev/input/js0"); // if not specified use js0
	}
	
	if (joy_file < 0) 
	{
		printf("Joystick open failed.\n"); // a thing is broken and you should stop
		exit(2);
	}

	serial_file = serialport_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE); // attempts to open the connection to the arduino with the BAUDRATE specified in the ROBOT_DEFINITIONS.h
	
	if(serial_file < 0)
	{
		printf("Can't open serial port."); // arduino not located, please stop breaking things
		exit(3);
	}

	clearPort(serial_file);

	while (1) {
		received = read_joystick_event(&jse); // check for a joystick update
		//printf("Reading...\n");
		usleep(1000); // check for updates every 1mS
		if (received == 1) { // if an update is available
			switch(jse.type)
			{
				case TYPE_BUTTON:
					//printf("NOT_BUTTON\n");
					button_update(&jse, new_button_values); // update teh new button array
					break;	
				case TYPE_NOT_BUTTON:
					//printf("BUTTON\n");
					axis_update(&jse, new_axis_values); // udpate the new axis values array
					break;
			}	
		}
		send_button_updates(old_button_values, new_button_values, serial_file); // checks bot the old and new button arrays for differences, if it finds one then an update is sent
		send_axis_updates(old_axis_values, new_axis_values, serial_file); // ditto from above
		
	}
}

