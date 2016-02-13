#include "locomotion.h"

void setWheelSpeed( int wheel, unsigned char speed, int serial_port )
{
    unsigned char motor_flag = 0;
    unsigned char rate = speed;

    if ( wheel == RIGHT )
    {
        printf("Writing %d to right motor.\r", rate);
        motor_flag = RIGHT_MOTOR_FLAG;
        write( serial_port, &motor_flag, 1 );
        write( serial_port, &rate, 1 );
    } else if ( wheel == LEFT )
    {
        printf("Writing %d to left motor.\r", rate);
        motor_flag = LEFT_MOTOR_FLAG;
        write( serial_port, &motor_flag, 1 );
        write( serial_port, &rate, 1 );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
    }

}
