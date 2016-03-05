#include "../include/locomotion.h"
#include "../include/robot_defines.h"
//#include "../include/localization.h"
#include "../include/logger.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define printf LOG

/*void setWheelSpeed( int wheel, unsigned char speed, int serial_port )
{
    unsigned char motor_flag = 0;
    unsigned char rate = speed;
    int result = 0; // To make gcc shut up about ignoring the return value

    if ( wheel == RIGHT )
    {
        printf("Writing %d to right motor.\r", rate);
        motor_flag = RIGHT_MOTOR_FLAG;
        result = write( serial_port, &motor_flag, 1 );
        result = write( serial_port, &rate, 1 );
    } else if ( wheel == LEFT )
    {
        printf("Writing %d to left motor.\r", rate);
        motor_flag = LEFT_MOTOR_FLAG;
        result = write( serial_port, &motor_flag, 1 );
        result = write( serial_port, &rate, 1 );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line %d\n", result);
    }
}*/

void driveWheelSteps( int wheel, int steps, int time, int serial_port ) //arduino will eventually expect milliseconds
{
    printf("Driving %d wheel %d steps in %d seconds\n", wheel, steps, time);
    int n = 0;
    unsigned char motor_flag = 0;

    if ( wheel == RIGHT )
    {
        printf("Writing to right motor.\n");
        motor_flag = RIGHT_MOTOR_STEPS_FLAG;
        n = n + write( serial_port, &motor_flag, 1 );
        n = n + write( serial_port, &steps, sizeof(steps) );
        n = n + write( serial_port, &time, sizeof(time) );
        printf("%d bytes written to right wheel\n", n );
    } else if ( wheel == LEFT )
    {
        printf("Writing to left motor.\n");
        motor_flag = LEFT_MOTOR_STEPS_FLAG;
        n = n + write( serial_port, &motor_flag, 1 );
        n = n + write( serial_port, &steps, sizeof(steps) );
        n = n + write( serial_port, &time, sizeof(time) );
        printf("%d bytes written to left wheel\n", n );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line %d\n", n);
    }
}

void turn( int serial_port, int angle, int time )
{
    double arc_length;
    int steps;
    // arc length = 2 * PI * R * (theta/360)
    arc_length = M_PI * WHEEL_BASE_MM * ( angle / 360.0 ); //in mm

    /* 180 deg .. 400 steps
     * 90 deg ... 200 steps
     * 45 deg ... 100 steps
     * 22.5 deg . 50 steps
     */

     steps = round( STEPS_PER_MM * arc_length );

     printf("Turning %d degrees in %d seconds. arc length = %f and %d steps per wheel\n", angle, time, arc_length, steps );

     driveWheelSteps( RIGHT, -steps, time, serial_port );
     driveWheelSteps( LEFT, steps, time, serial_port );
}

void drive( int serial_port, float distance, int time )
{
    // Drives the robot forward at `speed` for `distance`, in cm.

    int steps = 0;

    steps = round( STEPS_PER_CM * distance );
    printf("Driving straight %d steps\n", steps );

    driveWheelSteps( RIGHT, steps, time, serial_port );
    driveWheelSteps( LEFT, steps, time, serial_port );
}

void stop( int serial_port )
{
    driveWheelSteps( RIGHT, 0, 0, serial_port );
    driveWheelSteps( LEFT, 0, 0, serial_port );
}

void claws( int serial_port, int state )
{
    unsigned char motor_flag;
    int bytes = 0;
    unsigned char value;

    if ( state == RAISE )
    {
        value = RAISE_VAL;
        printf("Raising claws. Victim begins to scream.\n");
        motor_flag = SERVO_CLAW_RAISE_TAG;
        bytes = write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &value, 1 );
        printf("%d bytes written to claw servo\n", bytes );
    } else if (state == LOWER )
    {
        value = LOWER_VAL;
        printf("Lowering claws. Victim breathes a sigh of relief.\n");
        motor_flag = SERVO_CLAW_RAISE_TAG;
        bytes = write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &value, 1 );
        printf("%d bytes written to claw servo\n", bytes );
    } else if ( state == OPEN )
    {
        value = OPEN_VAL;
        printf("Opening claws. Victim barely escapes with their life.\n");
        motor_flag = SERVO_CLAW_CLOSE_TAG;
        bytes = write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &value, 1 );
        printf("%d bytes written to claw servo\n", bytes );
    } else if ( state == CLOSE )
    {
        value = CLOSE_VAL;
        printf("Closing claws. Crushing victim.\n");
        motor_flag = SERVO_CLAW_CLOSE_TAG;
        bytes = write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &value, 1 );
        printf("%d bytes written to claw servo\n", bytes );
    } else {
        printf("I don't know what you mean by that.\n");
        printf("Attempting to send state: %d defined in robot_defines under claw state.\n", state );
    }
}
