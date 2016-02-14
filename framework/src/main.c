#include "../include/main.h"

//Test drive robot with xbox 360 controller.
int main( int argc, char* argv[] )
{
    int serial_port = 0;
    int joystick = 0;

    sys_init( serial_port, joystick );

    //Check joystick for updates, and write those to the arduino.
    main_loop( serial_port, joystick );

    return 0;
}

void sys_init( int &serial_port, int &joystick )
{
    serial_port = serial_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE);

    if( serial_port < 0 )
	{
		printf("Can't open serial port.\n");
		exit(-1);
	}

    // default file. Get from commandline in the future.
    joy_file = open_joystick("/dev/input/js0");

    if ( joy_file < 0 )
    {
        printf("Can't open joystick file.\n");
        exit(-1);
    }

    clearPort(serial_port);
    printf("Serial and Joystick successfully initialized\n");
}
