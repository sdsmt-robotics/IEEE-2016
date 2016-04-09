#include <iostream>
#include <vector>
#include <unistd.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/sensors.h"
#include "../include/vision.h"

#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace cv;

// #define printf LOG
//Logging with g++ makes lots of warnings

int send_port;
int receive_port;
int victim_color;

int sys_init( const char* serialport );
bool initialize_all_the_things();


int main( int argc, char* argv[] )
{
    //initializes all the things. Serial and vision. Also tops off the magic blue smoke tanks.
    if ( !initialize_all_the_things() )
    {
        printf("initialize_all_the_things() failed miserably. You suck at programming. Go get some sleep and try again in a week.\n");
        return -1;
    }

    //Start doing the things.

    // while(1)
    // {
    //     int color =  checkColor();
    //     printf("color: %i\n", color);
    //     sleep(1);
    // }
        
    start_to_cp();
    retrieve_victim_1();

    retrieve_victim_2();

    retrieve_victim_3();

    retrieve_victim_4();

    cp_to_start();

    return 0;
}

int sys_init( const char* serialport )
{
    //opens up a serial connection to the given file
    printf("function: %s\n", __func__);

    int serial_file_handle = serial_init(serialport, ROBOT_BAUDRATE); // attempts to open the connection to the arduino with the BAUDRATE specified in the ROBOT_DEFINITIONS.h

    while(serial_file_handle < 0)
    {
        printf("Can't open serial port %s, trying again in 1 sec.\n", serialport); // arduino not located, please stop breaking things
        sleep(1);
        serial_file_handle = serial_init(serialport, ROBOT_BAUDRATE);
    }

    clearPort(serial_file_handle);
    printf("%s successfully initialized. File handle: %d\n", serialport, serial_file_handle);
    sleep(2); //wait for serial to initialize properly
    return serial_file_handle;
}

bool initialize_all_the_things()
{
    //initializes all the things
    printf("function: %s\n", __func__);

    send_port = sys_init(ARDUINO_COMM_LOCATION);
    receive_port = sys_init(SENSORS_COMM_LOCATION);
    clearPort( send_port );
    clearPort( receive_port );

   victim_color = UNKNOWN_COLOR;

    while ( !init_camera() )
    {
        init_camera();
        sleep(1);
    }

    return true;
}




