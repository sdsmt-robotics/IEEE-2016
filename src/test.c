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

int send_port;
int receive_port;
bool victim_color;

int main( int argc, char* argv[] )
{
    send_port = send_init();
    receive_port = receive_init();
    victim_color = YELLOW;

    test_follow_left_wall_until_end( 200, 6.0 );
    // while ( 1 )
    // {
    //     // printf("left: %.1f\n", left_sensor() );
    //     poll_sensors();
    //     sleep(1);
    // }
    return 0;
}

int send_init()
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

int receive_init()
{
    int serial_file = serial_init(SENSORS_COMM_LOCATION, ROBOT_BAUDRATE); // attempts to open the connection to the arduino with the BAUDRATE specified in the ROBOT_DEFINITIONS.h

    if(serial_file < 0)
    {
        while(serial_file < 0)
        {
            printf("Can't open serial port, trying again in 1 sec.\n"); // arduino not located, please stop breaking things
            sleep(1);
            serial_file = serial_init(SENSORS_COMM_LOCATION, ROBOT_BAUDRATE);
        }
    }

    clearPort(serial_file);
    printf("Serial successfully initialized. File handle: %d\n", serial_file );
    sleep(2); //wait for serial to initialize properly
    return serial_file;
}
