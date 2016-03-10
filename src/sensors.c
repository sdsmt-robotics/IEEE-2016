#include "../include/sensors.h"
#include "../include/robot_defines.h"
#include "../include/serial.h"

#include <stdio.h>
#include <unistd.h>

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
    //request sensor input, will worry about what to do with it once I get some sleep and some food.
    int nothing = write( serial_port, &flag, 1 );

    get_buffer( serial_port, &buffer );
    print_buffer( &buffer );
}