#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/sensors.h"

#include <stdio.h>
#include <unistd.h>

#define printf LOG

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

    // follow_left_wall_until_end( 190, 6.0 );
    // start_to_cp();
    
    // retrieve_victim_1();

    // while ( 1 )
    // {
    //     sensors( &vic, &back, &front, &left, &right );
    //     printf("front (cm): %.1f\n", front);
    //     printf("back (cm): %.1f\n", back);
    //     printf("left (cm): %.1f\n", left);
    //     printf("right (cm): %.1f\n", right);
    //     printf("vic (cm): %.1f\n", vic);
    //     printf("===========================\n");
    //     sleep(1);
    // }

    drive(32, 2);
    sleep(1);
    int i = 0;
//    int threshold = OG/4;
    //int speedyo = 2;
    int sleepytime = 2000000;
    int sleepstep = 100;

    int newspeed = 32 - (i*i);

    while (newspeed > 0)
    {
        printf("newspeed: %d\n", newspeed);
        printf("sleepytime: %d\n",sleepytime );
        drive(newspeed, sleepytime);


        ++i;
        sleepstep *= 10;
        newspeed = 32 - (i*i);
        sleepytime = 2000000 - (sleepstep * sleepstep);
        if (newspeed > 0)
            usleep(sleepytime);
    }

    // while (OG > threshold)
    // {
    //     //      32/1, 32/3, 32/5...
    //     drive(OG, (int)driveit);
    //     usleep(sleepytime);
    //     OG = floor(OG/1.25);
    //     driveit *= .875;

    // }
    // //drive(0, 0);

    drive(0, 0);



    // drive(4, 1);
    // usleep(500000);
    // drive(8, 1);
    // usleep(500000);
    // drive(16, 1);
    // usleep(500000);
    // drive(8, 1);
    // usleep(500000);
    // drive(4, 1);
    // usleep(1000000);


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