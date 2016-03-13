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

int serial_port;

int main( int argc, char* argv[] )
{
    serial_port = sys_init();
    
    // while ( 1 )
    // {
    //     int voltage = poll_left_sensor();
    //     double distance = map_voltage_to_distance( voltage );
    //     printf("distance: %f voltage: %d\n", distance, voltage );
    //     sleep(1);
    //     printf("left_sensor(): %f\n", left_sensor() );
    //     printf("=============\n");
    //     sleep(1);
    // }
    //follow_right_wall_until_end();

    while( 1 )
    {
        // turn( FULL_RIGHT_TURN, 4 );
        // sleep(6);
        // turn( FULL_LEFT_TURN, 4 );
        // sleep(6);

        // var_turn( FULL_RIGHT_TURN, 4 );
        // sleep(5);
        // var_turn( FULL_LEFT_TURN, 4 );
        // sleep(5);

        // drive( 10, 4 );
        // sleep(6);
        // drive( -10, 4 );
        // sleep(6);

        
        claw( CLOSE );
        claw( RAISE );
        sleep(1);
        claw( OPEN );
        claw( LOWER );
        sleep(1);
    }
    

    

    return 0;
}

int sys_init( )
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
    sleep(2); // because Arduino ignores the very first write, wait for serial to initialize properly
    return serial_file;
}
