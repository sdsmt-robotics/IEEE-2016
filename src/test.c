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

    int val_steps = 400;
    int val_time = 1000;
    char buffer[512] = "";
    while (1)
    {
        printf("Writing to right motor.\n");

        unsigned char motor_flag = RIGHT_MOTOR_STEPS_FLAG;
        int n = write( serial_port, &motor_flag, 1 );
        n = n + write( serial_port, &val_steps, 4 );
        n = n + write( serial_port, &val_time, 4 );

        printf("%d bytes written to right wheel\n", n );

        int m = read(serial_port, &buffer, sizeof(buffer));
        if(m>0)
        {
            buffer[m] = '\0';
            printf("\n-%s", buffer);
            fflush(stdout);
        }
    }

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
