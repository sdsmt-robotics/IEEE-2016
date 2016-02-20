#include "../include/main.h"
#include "../include/main_loop.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include <stdio.h>

//Test drive robot with xbox 360 controller.
int main( int argc, char* argv[] )
{
    int serial_port = 0;

    sys_init( &serial_port );

    //Check joystick for updates, and write those to the arduino.
    main_loop( &serial_port );

    return 0;
}

void sys_init( int *serial_port )
{
    *serial_port = serial_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE);

    if( *serial_port < 0 )
	{
		printf("Can't open serial port.\n");
		exit(-1);
	}

    clearPort(*serial_port);
    printf("Serial successfully initialized\n");
}
