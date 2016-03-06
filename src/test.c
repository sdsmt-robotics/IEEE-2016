#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

//#define printf LOG //To log to logfile AND console

int main( int argc, char* argv[] )
{
    int serial_port = sys_init();
    int n = 0;
    int val_steps = 400;
    int val_time = 1000;

    printf("serial_port = %d\n", serial_port );

    printf("Writing to right motor.\n");

    unsigned char motor_flag = RIGHT_MOTOR_STEPS_FLAG;
    n = n + write( serial_port, &motor_flag, 1 );
    n = n + write( serial_port, &val_steps, sizeof(val_steps) );
    n = n + write( serial_port, &val_time, sizeof(val_time) );

    printf("%d bytes written to right wheel\n", n );

    

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
