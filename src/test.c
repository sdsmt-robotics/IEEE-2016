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
    int serial_port = 0;

    sys_init( &serial_port );

    drive( serial_port, 20, 10 ); // drive forward 20cm in 10 seconds
    usleep(11 * 1000);
    drive( serial_port, -20, 10 ); // drive backwards 20cm in 10 seconds
    usleep(5 * 1000);
    stop( serial_port ); // stop robot after 5 seconds
    turn( serial_port, FULL_RIGHT_TURN, 5); // right in 5 seconds
    usleep(6 * 1000);
    turn(serial_port, FULL_LEFT_TURN, 5); // left in 5 seconds
    usleep(6 * 1000);

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
