#include "../include/sensors.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/serial.h"

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

int extract_sensor_data( int serial_port, int sensor )
{
    //Arduino Mega is little-Endian
    char buffer[200] = "";
    unsigned char flag = SENSOR_REQUEST;
    int n = 0;
    int count = 0;

    char left[4] = "";
    char right[4] = "";
    char front[4] = "";

    int nothing = write( serial_port, &flag, 1);
    usleep(10000);
    n = read( serial_port, &buffer, sizeof(buffer) );

    char *token = strtok(buffer, (char *) IR_PACKET_END);
    printf("token: %s", token );
    strcpy( left, token );
    while(token) {
        token = strtok(NULL, (char *) IR_PACKET_END);
        count++;
        printf("token: %s", token );
        switch (count)
        {
            case 1:
            {
                strcpy( right, token );
                break;
            }
            case 2:
            {
                strcpy( front, token );
                break;
            }
            default:
            {
                break;
            }
        }

    }
    //make sure they're null terminated :(
    left[3] = '\0';
    right[3] = '\0';
    front[3] = '\0';

    if ( sensor == LEFT )
    {
        return atoi( left );
    } else if ( sensor == RIGHT )
    {
        return atoi( right );
    } else if ( sensor == FRONT )
    {
        return atoi( front );
    } else
    {
        return -1;
    }

}