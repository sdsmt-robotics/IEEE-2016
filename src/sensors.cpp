#include "../include/sensors.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/serial.h"

#include <stdio.h>
#include <unistd.h>

#define printf LOG


double map_voltage_to_distance( int voltage )
{
    // https://acroname.com/articles/linearizing-sharp-ranger-data
    // We had to divide the function the above link gives by two to
    // properly characterize the sensor output.
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

double vic_sensor()
{
    return map_voltage_to_distance( poll_vic_sensor() );
}

void poll_sensors()
{
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

    printf("front (V): %d front (cm): %.1f\n", front, map_voltage_to_distance(front) );
    printf("back (V): %d back (cm): %.1f\n", back, map_voltage_to_distance(back) );
    printf("left (V): %d left (cm): %.1f\n", left, map_voltage_to_distance(left) );
    printf("right (V): %d right (cm): %.1f\n", right, map_voltage_to_distance(right) );
    printf("vic (V): %d vic (cm): %.1f\n", vic, map_voltage_to_distance(vic) );
    printf("===========================\n");

    clear_buffer();
}

void sensors( double *vic, double *back, double *front, double *left, double *right )
{
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

    *vic = map_voltage_to_distance( vic_v );
    *back = map_voltage_to_distance( back_v );
    *front = map_voltage_to_distance( front_v );
    *left = map_voltage_to_distance( left_v );
    *right = map_voltage_to_distance( right_v );
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

    clear_buffer();
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

    clear_buffer();
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

    clear_buffer();
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

    clear_buffer();
    return value;
}

int poll_vic_sensor()
{
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
    printf("You've called SetVictimLocation(), but it doesn't do anything...\n");
}
