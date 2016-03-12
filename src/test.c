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

#define printf LOG //To log to logfile AND console


int main( int argc, char* argv[] )
{
    int serial_port = sys_init();
    
    while ( 1 )
    {
        int voltage = poll_left_sensor( serial_port );
        double distance = map_voltage_to_distance( voltage );
        printf("distance: %f voltage: %d\n", distance, voltage );
        printf("hanging here?\n");
        printf("left_sensor(): %f\n", left_sensor() );
        sleep(1);
    }
    //start_to_cp( serial_port );

    

    return 0;
}

int sys_init( )
{
    int serial_port = serial_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE); // attempts to open the connection to the arduino with the BAUDRATE specified in the ROBOT_DEFINITIONS.h
    
    if(serial_port < 0)
    {
        while(serial_port < 0)
        {
            printf("Can't open serial port, trying again in 1 sec.\n"); // arduino not located, please stop breaking things
            sleep(1);
            serial_port = serial_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE);
        }
    }

    clearPort(serial_port);
    printf("Serial successfully initialized. File handle: %d\n", serial_port );
    sleep(2); // because Arduino ignores the very first write, wait for serial to initialize properly
    return serial_port;
}
