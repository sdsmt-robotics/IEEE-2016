#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/sensors.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

//#define printf LOG //To log to logfile AND console

int serial_port;

int main( int argc, char* argv[] )
{
    serial_port = sys_init();

    //follow_right_wall_until_obstacle( 200, WALL_FOLLOW_TARGET, 10 );
    // follow_left_wall_until_end( 200, WALL_FOLLOW_TARGET );

    retreive_victim_1();

    // while( 1 )
    // {
    //
    //     poll_sensors();
    //     sleep(1);
    // }

    return 0;
}

int sys_init()
{
    int serial_file = serial_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE); // attempts to open the connection to the arduino with the BAUDRATE specified in the ROBOT_DEFINITIONS.h

    if(serial_file < 0)
    {
        while(serial_file < 0)
        {
            printf("Can't open serial port, trying again in 1 sec.\n"); // arduino not located, please stop breaking things
            sleep(1);
            serial_file = serial_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE);
        }
    }

    clearPort(serial_file);
    printf("Serial successfully initialized. File handle: %d\n", serial_file );
    sleep(2); //wait for serial to initialize properly
    return serial_file;
}
