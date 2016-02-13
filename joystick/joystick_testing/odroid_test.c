#include <stdio.h>
#include "joystick.h"
#include "robot_defines.h"
#include "easy_serial.h"
#include "360_controller_map.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int send_button_update(struct js_event *jse, int serial_port)
{

	unsigned char speed = 0;
	unsigned char flag  = 0;
	
	if(jse->value == 1) 
		speed = 255;

	switch(jse->number)
	{
		case BUTTON_LEFT_BUMPER:
			flag = LEFT_MOTOR_FLAG;
			if(jse->value == 1)
			{
				printf("Turning left motor on.\r");
				write(serial_port, &flag, 1);
				write(serial_port, &speed, 1);
			}
			else
			{
				printf("Turning left motor off.\r");
				write(serial_port, &flag, 1);
				write(serial_port, &speed, 1);
			}
			break;
		case BUTTON_RIGHT_BUMPER:
			flag = RIGHT_MOTOR_FLAG;
			if(jse->value == 1)
			{
				printf("Turning right motor on.\r");
				write(serial_port, &flag, 1);
				write(serial_port, &speed, 1);
			}
			else
			{
				printf("Turning right motor off.\r");
				write(serial_port, &flag, 1);
				write(serial_port, &speed, 1);
			}
			break;
	}
}

/* a little test program */
int main(int argc, char *argv[])
{
	int  joy_file, received, serial_file;
	int  done = 0;
	char joy_address[32] = "/dev/input/\0";
	int  axis_values[8] = {0};
	int  button_values[11] = {0};

	struct js_event jse;

	if(argc > 2)
	{
		printf("Too many arguments, exiting.");
		return 0;
	}
	if(argc == 2)
	{
		strcat(joy_address, argv[1]);
		joy_file = open_joystick(joy_address);
	}
	else if(argc == 1)
	{
		joy_file = open_joystick("/dev/input/js0");
	}
	
	if (joy_file < 0) 
	{
		printf("Joystick open failed.\n");
		exit(1);
	}

	serial_file = serialport_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE);
	
	if(serial_file < 0)
	{
		printf("Can't open serial port.");
		exit(2);
	}

	clearPort(serial_file);

	while (!done) {
		received = read_joystick_event(&jse);
		usleep(1000);
		if (received == 1) {
			switch(jse.type)
			{
				case TYPE_NOT_BUTTON:
					if(axis_values[jse.number] != jse.value)
						axis_values[jse.number] = jse.value;
					break;	
				case TYPE_BUTTON:
					if(button_values[jse.number] != jse.value)
					{
						button_values[jse.number] = jse.value;
						send_button_update(&jse, serial_file);
					}
					break;
			}	
		}
	}
}

