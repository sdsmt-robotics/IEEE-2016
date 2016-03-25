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

#define printf LOG

int send_port;
int receive_port;
int victim_color;

int main( int argc, char* argv[] )
{
    send_port = sys_init(ARDUINO_COMM_LOCATION);
    receive_port = sys_init(SENSORS_COMM_LOCATION);
    victim_color = YELLOW;

    // var_test_follow_left_wall_until_end( 190, 6.0 );
    // retrieve_victim_1();

    while ( 1 )
    {
        poll_sensors();
        sleep(1);
    }
    return 0;
}

int sys_init( const char* serialport )
{
    int serial_file_handle = serial_init(serialport, ROBOT_BAUDRATE); // attempts to open the connection to the arduino with the BAUDRATE specified in the ROBOT_DEFINITIONS.h

    while(serial_file_handle < 0)
    {
        printf("Can't open serial port %s, trying again in 1 sec.\n", serialport); // arduino not located, please stop breaking things
        sleep(1);
        serial_file_handle = serial_init(serialport, ROBOT_BAUDRATE);
    }

    clearPort(serial_file_handle);
    printf("Serial successfully initialized. File handle: %d\n", serial_file_handle );
    sleep(2); //wait for serial to initialize properly
    return serial_file_handle;
}