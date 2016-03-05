#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"

#include <stdio.h>
#include <stdbool.h>

#define printf LOG //To log to logfile AND console

int main( int argc, char* argv[] )
{
    int serial_port = 0;

    sys_init( &serial_port );

    // the following are defined in navigation.c
    /*start_to_cp( serial_port );

    if ( !retreive_victim_1( serial_port ) )
    {
        get_to_cp( serial_port );
    }

    if ( !retreive_victim_2( serial_port ) )
    {
        get_to_cp( serial_port );
    }

    if ( !retreive_victim_3( serial_port ) )
    {
        get_to_cp( serial_port );
    }

    if ( !retreive_victim_4( serial_port ) )
    {
        get_to_cp( serial_port );
    }

    cp_to_start( serial_port );
    */

    drive( serial_port, 20, 10 ); // drive forward 20cm in 10 seconds
    sleep(10);
    drive( serial_port, -20, 10 );
    sleep(5);
    stop( serial_port );

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
