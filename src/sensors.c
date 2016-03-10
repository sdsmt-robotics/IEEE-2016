#include "../include/sensors.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"

#include <stdio.h>
#include <unistd.h>

#define printf LOG



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

int assemble_int( unsigned char *input, int byte_offset )
{
    //Arduino Mega is little-Endian
    unsigned char inBytes[4] = {0}; //zeroes the storage array
    int num = 0;
    for(int i = 0; i < 4; i++) //grab the first four bytes starting at the index
    {
        inBytes[i] = input[byte_offset + i];
    }
    for(int i = 3; i > -1; i--) //assemble the bytes into an "int" in arduino (long errywhere else)
    {
        num = num << 8;
        num = num | inBytes[i];
    }
    return num;
}