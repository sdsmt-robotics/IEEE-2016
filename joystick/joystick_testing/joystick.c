#include <stdio.h>
#include "joystick.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* a little test program */
int main(int argc, char *argv[])
{
	int fd, rc;
	int done = 0;
	char joy[32] = "/dev/input/\0";
	if(argc > 2)
	{
		printf("Too many arguments, exiting.");
		return 0;
	}

	struct js_event jse;

	if(argc == 2)
	{
		strcat(joy, argv[1]);
		fd = open_joystick(joy);
	}
	else if(argc == 1)
	{
		fd = open_joystick("dev/input/js0");
	}
	
	if (fd < 0) 
	{
		printf("open failed.\n");
		exit(1);
	}

	while (!done) {
		rc = read_joystick_event(&jse);
		usleep(1000);
		if (rc == 1) {
			printf("Event: time %8u, value %8hd, type: %3u, axis/button: %u\n", 
				jse.time, jse.value, jse.type, jse.number);
		}
	}
}
