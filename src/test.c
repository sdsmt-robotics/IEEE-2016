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
    /* For some reason this is no longer working. It ran on the very first try and never since.
     * What I think is happening is that serial_init() is no opening the correct file, because
     * in the joystick test the serial_file is 4, and here's 3. I'm not sure it matter though,
     * but that's the only difference I can find between the source code.
     *
     * The following segment of code is identical to what is found in joystick/full_featured_RC/odroid_test_analog.c,
     * which works. (start and back buttons on the controller).
     *
     * This leads me to believe that the problem isn't in the following segment of code, but rather
     * that it's in the serial initialization, which I know nothing about. Because the code ran on
     * startup works as it should, I wonder if the serial file is being closed properly, and 
     * we're trying to open and write to a file that's already open.
     */

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
