#include "../include/sensors.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/serial.h"

#include <stdio.h>
#include <unistd.h>

//#define printf LOG



int Left_IR()
{
    return 0;
}

int Right_IR()
{
    return 0;
}

int Forward_IR()
{
    return 0;
}

int Front_Right_IR()
{
    return 0;
}

int Front_Left_IR()
{
    return 0;
}

int Backward_IR()
{
    return 0;
}

void temporary_sensor_request( int serial_port )
{
    unsigned char flag = SENSOR_REQUEST;
    char buffer[512] = "";
    int m = 0;

    int nothing = write( serial_port, &flag, 1 );

    usleep(10000); // wait 10 ms for Arduino to receive flag and send back sensor input

    m = read( serial_port, &buffer, sizeof(buffer) );

    if( m > 0 )
    {
        buffer[m] = '\0';
        printf( "Buffer (%d bytes):\n=======\n%s\n=======\n", m, buffer );
        fflush(stdout);
    }
}

int extract_sensor_data( int serial_port, int sensor )
{
    //Arduino Mega is little-Endian
    char buffer[7] = "";
    unsigned char flag = SENSOR_REQUEST;
    int n = 0;

    unsigned short left = 0;
    unsigned short right = 0;
    unsigned short front = 0;

    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( serial_port, &flag, 1);
    usleep(10000);
    n = read( serial_port, &buffer, sizeof(buffer) );
    //printf( "buffer (%d bytes):\n%s\n", n, buffer );

    //Odroid is little-Endian as well.
    left_byte = (unsigned char) buffer[0];
    right_byte = (unsigned char) buffer[1];
    left = left | left_byte;
    left = left << 4;
    left = left | right_byte;

    left_byte = (unsigned char) buffer[2];
    right_byte = (unsigned char) buffer[3];
    right = right | left_byte;
    right = right << 4;
    right = right | right_byte;

    left_byte = (unsigned char) buffer[4];
    right_byte = (unsigned char) buffer[5];
    front = front | left_byte;
    front = front << 4;
    front = front | right_byte;

    if ( sensor == LEFT )
    {
        return left;
    } else if ( sensor == RIGHT )
    {
        return right;
    } else if ( sensor == FRONT )
    {
        return front;
    } else
    {
        return -1;
    }

}