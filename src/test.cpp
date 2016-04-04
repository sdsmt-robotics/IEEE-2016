#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
//#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/sensors.h"

#include <stdio.h>
#include <unistd.h>

//#define printf LOG
//Logging with g++ makes lots of warnings

int send_port;
int receive_port;
int victim_color;

int main( int argc, char* argv[] )
{
    send_port = sys_init(ARDUINO_COMM_LOCATION);
    receive_port = sys_init(SENSORS_COMM_LOCATION);
    victim_color = RED;

    // double left = 0;
    // double right = 0;
    // double back = 0;
    // double vic = 0;
    // double front = 0;

    // while ( 1 )
    // {
    //     sensors( &vic, &back, &front, &left, &right );
    //     printf("front (cm): %.1f\nback (cm): %.1f\nleft (cm): %.1f\nright (cm): %.1f\nvic (cm): %.1f\n", front, back, left, right, vic);
    //     printf("===========================\n");
    //     sleep(1);
    // }

    drive(32, 2);
    sleep(1);
    int i = 0;
    int dist = 32 - (i*i);
    int sleepytime = 0;
    int sleepstep = 0;

    while (dist > 1)
    {
        drive(dist, 2);


        ++i;
        sleepytime = sleepytime % 10;
        dist = 32 - (i*i);
        sleepytime = 1000000 - sleepstep;

        if (dist > 17)
        {
            usleep(50000);
            stop();
        }
    }
    stop();


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
