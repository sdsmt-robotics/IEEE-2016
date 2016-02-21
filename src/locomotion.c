#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/localization.h"
#include <stdio.h>  //printf()
#include <unistd.h>
#include <math.h>

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

void driveWheelSteps( int wheel, int steps, float speed, int serial_port )
{
    printf("Driving %d wheel %d steps at %d speed\n", wheel, steps, speed);
    //TODO: implement this
}

void turn( int serial_port, int angle, float time )
{
    double arc_length;
    int steps;
    // arc length = 2 * PI * R * (theta/360)
    arc_length = PI * WHEEL_BASE_MM * ( angle / 360.0 ); //in mm

    /* 180 deg .. 400 steps
     * 90 deg ... 200 steps
     * 45 deg ... 100 steps
     * 22.5 deg . 50 steps
     */

     steps = round( STEPS_PER_MM * arc_length );

     driveWheelSteps( RIGHT, -steps, time, serial_port );
     driveWheelSteps( LEFT, steps, time, serial_port );
}

void drive( int serial_port, float distance, float time )
{
    // Drives the robot forward at `speed` for `distance`, in cm.

    int steps;

    steps = STEPS_PER_CM * distance;

    driveWheelSteps( RIGHT, steps, time, serial_port );
    driveWheelSteps( LEFT, steps, time, serial_port );
}
