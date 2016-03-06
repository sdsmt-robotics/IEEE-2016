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
    int serial_file = sys_init();
    unsigned char flag = RIGHT_MOTOR_STEPS_FLAG;
    int steps = 400;
    int seconds = 2;

    printf("\nMoving %d steps in %d seconds.\n", steps, seconds);
    int n = write(serial_file, &flag, 1);
    n = n + write(serial_file, &steps, sizeof(steps));
    n = n + write(serial_file, &seconds, sizeof(seconds));
    printf("Wrote %d bytes.\n", n);

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
    printf("Serial successfully initialized. File handle: %d\n", serial_port );

    return serial_port;
}
