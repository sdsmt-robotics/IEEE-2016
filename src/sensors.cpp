#include "../include/sensors.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/serial.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>
// #define printf LOG


double map_voltage_to_distance( int voltage, int flag )
{
    // https://acroname.com/articles/linearizing-sharp-ranger-data
    // We had to divide the function the above link gives by two to
    // properly characterize the sensor output.
    static double last_left = 0;
    static double last_right = 0;
    static double last_front = 0;
    static double last_back = 0;
    static double last_vic = 0;

    double dist = IR_DISTANCE_SCALAR * ( (6787.0)/(voltage - 3.0) - 4.0);

    switch(flag)
    {
        case LEFT:
        {
            if ( isinf(dist) || dist < 0 )
            {
                dist = last_left;
            }
            last_left = dist;
            return dist;
        }
        case RIGHT:
        {
            if ( isinf(dist) || dist < 0 )
            {
                dist = last_right;
            }
            last_right = dist;
            return dist;
        }
        case FRONT:
        {
            if ( isinf(dist) || dist < 0 )
            {
                dist = last_front;
            }
            last_front = dist;
            return dist;
        }
        case BACK:
        {
            if ( isinf(dist) || dist < 0 )
            {
                dist = last_back;
            }
            last_back = dist;
            return dist;
        }
        case VIC:
        {
            if ( isinf(dist) || dist < 0 )
            {
                dist = last_vic;
            }
            last_vic = dist;
            return dist;
        }
    } 
}

double left_sensor()
{
    return map_voltage_to_distance( poll_left_sensor(), LEFT );
}

double right_sensor()
{
    return map_voltage_to_distance( poll_right_sensor(), RIGHT );
}

double front_sensor()
{
    return map_voltage_to_distance( poll_front_sensor(), FRONT );
}

double back_sensor()
{
    return map_voltage_to_distance( poll_back_sensor(), BACK );
}

double vic_sensor()
{
    return map_voltage_to_distance( poll_vic_sensor(), VIC );
}

void poll_sensors()
{
    //requests and prints feedback from all five sensors

    char buffer[10] = "";
    unsigned char flag = SENSOR_REQUEST;
    int n = 0;

    unsigned short left = 0;
    unsigned short right = 0;
    unsigned short front = 0;
    unsigned short back = 0;
    unsigned short vic = 0;

    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( send_port, &flag, 1 );
    nothing += 1; // to make gcc stfu
    while ( n < 10 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        //will overwrite first few bytes in buffer, but won't leave unclaimed bytes hanging in memory...
        n += read( receive_port, &buffer, sizeof(buffer) );
    }

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

    left_byte = (unsigned char) buffer[8];
    right_byte = (unsigned char) buffer[9];
    vic = vic | left_byte;
    vic = vic << 8;
    vic = vic | right_byte;

    printf("front (V): %d front (cm): %.1f\n", front, map_voltage_to_distance(front, FRONT ));
    printf("back (V): %d back (cm): %.1f\n", back, map_voltage_to_distance(back, BACK ));
    printf("left (V): %d left (cm): %.1f\n", left, map_voltage_to_distance(left, LEFT ));
    printf("right (V): %d right (cm): %.1f\n", right, map_voltage_to_distance(right, RIGHT ));
    printf("vic (V): %d vic (cm): %.1f\n", vic, map_voltage_to_distance(vic, VIC ));
    printf("===========================\n");

    clear_buffer();
}

void sensors( double *vic, double *back, double *front, double *left, double *right )
{
    //"returns" sensor feedback from all five sensors to variables passed by reference

    char buffer[10] = "";
    unsigned char flag = SENSOR_REQUEST;
    int n = 0;

    unsigned short left_v = 0;
    unsigned short right_v = 0;
    unsigned short front_v = 0;
    unsigned short back_v = 0;
    unsigned short vic_v = 0;

    unsigned char left_byte = 0;
    unsigned char right_byte = 0;

    int nothing = write( send_port, &flag, 1 );
    nothing += 1; // to make gcc stfu
    while ( n < 10 )
    {
        usleep(SENSOR_PROC_DELAY_US);
        //will overwrite first few bytes in buffer, but won't leave unclaimed bytes hanging in memory...
        n += read( receive_port, &buffer, sizeof(buffer) );
    }

    left_byte = (unsigned char) buffer[0];
    right_byte = (unsigned char) buffer[1];
    left_v = left_v | left_byte;
    left_v = left_v << 8;
    left_v = left_v | right_byte;

    left_byte = (unsigned char) buffer[2];
    right_byte = (unsigned char) buffer[3];
    right_v = right_v | left_byte;
    right_v = right_v << 8;
    right_v = right_v | right_byte;

    left_byte = (unsigned char) buffer[4];
    right_byte = (unsigned char) buffer[5];
    front_v = front_v | left_byte;
    front_v = front_v << 8;
    front_v = front_v | right_byte;

    left_byte = (unsigned char) buffer[6];
    right_byte = (unsigned char) buffer[7];
    back_v = back_v | left_byte;
    back_v = back_v << 8;
    back_v = back_v | right_byte;

    left_byte = (unsigned char) buffer[8];
    right_byte = (unsigned char) buffer[9];
    vic_v = vic_v | left_byte;
    vic_v = vic_v << 8;
    vic_v = vic_v | right_byte;

    clear_buffer();

    *vic = map_voltage_to_distance( vic_v, VIC );
    *back = map_voltage_to_distance( back_v, BACK );
    *front = map_voltage_to_distance( front_v, FRONT );
    *left = map_voltage_to_distance( left_v, LEFT );
    *right = map_voltage_to_distance( right_v, RIGHT );
}

int poll_left_sensor()
{
    //grabs the raw left sensor output
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

    clear_buffer();
    return value;
}

int poll_right_sensor()
{
    //grabs the raw right sensor output
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

    clear_buffer();
    return value;
}

int poll_front_sensor()
{
    //grabs the raw front sensor output
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

    clear_buffer();
    return value;
}

int poll_back_sensor()
{
    //grabs the raw "back" sensor output
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

    clear_buffer();
    return value;
}

int poll_vic_sensor()
{
    //grabs the raw "victim sensor" output
    unsigned char request_flag = VIC_SENSOR_REQUEST;
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

    clear_buffer();
    return value;
}

void SetVictimLocation()
{
    //does things
    printf("You've called SetVictimLocation(), but it doesn't do anything...\n");
}
