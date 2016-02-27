#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"

#include <stdio.h>
#include <stdbool.h>

#define printf LOG //To log to logfile AND console

//Test drive robot with xbox 360 controller.
int main( int argc, char* argv[] )
{
    int serial_port = 0;

    sys_init( &serial_port );

    // the following are defined in navigation.c
    start_to_cp();

    if ( !retreive_victim_1() )
    {
        get_to_cp();
    }

    if ( !retreive_victim_2() )
    {
        get_to_cp();
    }

    if ( !retreive_victim_3() )
    {
        get_to_cp();
    }

    if ( !retreive_victim_4() )
    {
        get_to_cp();
    }

    cp_to_start();
    

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
