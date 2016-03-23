#include "../include/sensors.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/serial.h"

#include <stdio.h>
#include <unistd.h>
//#define printf LOG


double map_voltage_to_distance( int voltage )
{
    // https://acroname.com/articles/linearizing-sharp-ranger-data
    return IR_DISTANCE_SCALAR * ( (6787.0)/(voltage - 3.0) - 4.0 );
}

double left_sensor()
{
    return map_voltage_to_distance( poll_left_sensor() );
}

double right_sensor()
{
    return map_voltage_to_distance( poll_right_sensor() );
}

double front_sensor()
{
    return map_voltage_to_distance( poll_front_sensor() );
}

double back_sensor()
{
    return map_voltage_to_distance( poll_back_sensor() );
}

void temporary_sensor_request()
{
    unsigned char flag = SENSOR_REQUEST;
    char buffer[512] = "";
    int n = 0;

    int nothing = write( send_port, &flag, 1);
    nothing += 1;
    while ( n <= 6 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        n += read( receive_port, &buffer, sizeof(buffer) );
    }

    if( n > 0 )
    {
        buffer[n] = '\0';
        printf( "Buffer (%d bytes):\n=======\n%s\n=======\n", n, buffer );
        fflush(stdout);
    }
}

void poll_sensors()
{
    //Arduino Mega is little-Endian
    char buffer[8] = "";
    unsigned char flag = SENSOR_REQUEST;
    int n = 0;

    unsigned short left = 0;
    unsigned short right = 0;
    unsigned short front = 0;
    unsigned short back = 0;

    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( send_port, &flag, 1);
    nothing += 1;
    while ( n < 6 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        n += read( receive_port, &buffer, sizeof(buffer) );
    }

    //Odroid is little-Endian as well.
    left_byte = (unsigned char) buffer[0];
    right_byte = (unsigned char) buffer[1];
    left = left | left_byte;
    left = left << 8;
    left = left | right_byte;

    left_byte = (unsigned char) buffer[2];
    right_byte = (unsigned char) buffer[3];
    right = right | left_byte;
    right = right << 8;
    right = right | right_byte;

    left_byte = (unsigned char) buffer[4];
    right_byte = (unsigned char) buffer[5];
    front = front | left_byte;
    front = front << 8;
    front = front | right_byte;

    left_byte = (unsigned char) buffer[6];
    right_byte = (unsigned char) buffer[7];
    back = back | left_byte;
    back = back << 8;
    back = back | right_byte;

    printf("front (V): %d front (cm): %.1f\n", front, map_voltage_to_distance(front) );
    printf("back (V): %d back (cm): %.1f\n", back, map_voltage_to_distance(back) );
    printf("left (V): %d left (cm): %.1f\n", left, map_voltage_to_distance(left) );
    printf("right (V): %d right (cm): %.1f\n", right, map_voltage_to_distance(right) );
    printf("===========================\n");
   
}

int poll_left_sensor()
{
    unsigned char request_flag = LEFT_SENSOR_REQUEST;
    char buffer[2] = "";
    int n = 0;

    unsigned short value = 0;
    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( send_port, &request_flag, 1 );
    nothing += 1;
    while ( n < 2 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        n += read( receive_port, &buffer, sizeof(buffer) );
    }

    left_byte = (unsigned char) buffer[0];
    right_byte = (unsigned char) buffer[1];
    value = value | left_byte;
    value = value << 8;
    value = value | right_byte;

    return value;
}

int poll_right_sensor()
{
    unsigned char request_flag = RIGHT_SENSOR_REQUEST;
    char buffer[2] = "";
    int n = 0;

    unsigned short value = 0;
    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( send_port, &request_flag, 1 );
    nothing += 1;

    while ( n < 2 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        n += read( receive_port, &buffer, sizeof(buffer) );
        
    }

    left_byte = (unsigned char) buffer[0];
    right_byte = (unsigned char) buffer[1];
    value = value | left_byte;
    value = value << 8;
    value = value | right_byte;

    return value;
}

int poll_front_sensor()
{
    unsigned char request_flag = FRONT_SENSOR_REQUEST;
    char buffer[2] = "";
    int n = 0;

    unsigned short value = 0;
    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( send_port, &request_flag, 1 );
    nothing += 1;
    while ( n < 2 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        n += read( receive_port, &buffer, sizeof(buffer) );
    }

    left_byte = (unsigned char) buffer[0];
    right_byte = (unsigned char) buffer[1];
    value = value | left_byte;
    value = value << 8;
    value = value | right_byte;

    return value;
}

int poll_back_sensor()
{
    unsigned char request_flag = BACK_SENSOR_REQUEST;
    char buffer[2] = "";
    int n = 0;

    unsigned short value = 0;
    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( send_port, &request_flag, 1 );
    nothing += 1;
    while ( n < 2 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        n += read( receive_port, &buffer, sizeof(buffer) );
    }

    left_byte = (unsigned char) buffer[0];
    right_byte = (unsigned char) buffer[1];
    value = value | left_byte;
    value = value << 8;
    value = value | right_byte;
    
    return value;
}
