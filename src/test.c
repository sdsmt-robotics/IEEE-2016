#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define printf LOG //To log to logfile AND console

int main( int argc, char* argv[] )
{
    int serial_port = sys_init();

    setWheelSpeed( RIGHT, 255, serial_port );
    usleep(11000);
    setWheelSpeed( RIGHT, 127, serial_port );

    

    return 0;
}

int sys_init( )
{
    int serial_port = serial_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE);

    if( serial_port < 0 )
	{
		printf("Can't open serial port.\n");
		exit(-1);
	}

    clearPort(serial_port);
    printf("Serial successfully initialized\n");

    return serial_port;
}
