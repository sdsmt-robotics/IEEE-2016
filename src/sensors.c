#include "../include/sensors.h"

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
    char *buffer[512] = "";
    //request sensor input, will worry about what to do with it once I get some sleep and some food.
    write( serial_port, &flag, 1 );

    int n = read( serial_port, &buffer, sizeof(buffer) );
    if(n > 0)
    {
        buffer[n] = '\0';
        // print the part of the buffer that had stuff in it
        printf("buffer (%d bytes):\n==================\n%s\n==================\nEnd buffer. Yay.\n", n, buffer);          fflush(stdout);
    }
}