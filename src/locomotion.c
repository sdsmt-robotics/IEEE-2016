#include "../include/locomotion.h"
#include "../include/robot_defines.h"
//#include "../include/localization.h"
#include "../include/logger.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define printf LOG

void setWheelSpeed( int wheel, unsigned char speed, int serial_port )
{
    unsigned char motor_flag = 0;
    int bytes = 0; // To make gcc shut up about ignoring the return value

    if ( wheel == RIGHT )
    {
        printf("Writing %d to right motor.\n", speed);
        motor_flag = RIGHT_MOTOR_FLAG;
        bytes = bytes + write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &speed, 1 );
        printf("wrote %d bytes to motor\n", bytes );
    } else if ( wheel == LEFT )
    {
        printf("Writing %d to left motor.\n", speed);
        motor_flag = LEFT_MOTOR_FLAG;
        bytes = bytes + write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &speed, 1 );
        printf("wrote %d bytes to motor\n", bytes );
    } else if ( wheel == BOTH )
    {
        printf("Writing %d to right motor.\n", speed);
        motor_flag = RIGHT_MOTOR_FLAG;
        bytes = bytes + write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &speed, 1 );
        printf("wrote %d bytes to motor\n", bytes );

        printf("Writing %d to left motor.\n", speed);
        motor_flag = LEFT_MOTOR_FLAG;
        bytes = bytes + write( serial_port, &motor_flag, 1 );
        bytes = bytes + write( serial_port, &speed, 1 );
        printf("wrote %d bytes to motor\n", bytes );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line %d\n", wheel);
    }
}

void driveWheelSteps( int wheel, int steps, int runtime, int serial_port )
{
    printf("Driving wheel %d %d steps in %d seconds\n", wheel, steps, runtime);
    int n = 0;
    int m = 0;
    unsigned char motor_flag = 0;
    char buffer[512] = "";

    if ( wheel == RIGHT )
    {
        printf("Writing to right motor.\n");
        motor_flag = RIGHT_MOTOR_STEPS_FLAG;
        n = n + write( serial_port, &motor_flag, 1 );
        n = n + write( serial_port, &steps, sizeof(steps) );
        n = n + write( serial_port, &runtime, sizeof(runtime) );
        printf("%d bytes written to right wheel\n", n );
        
        m = read( serial_port, &buffer, sizeof(buffer) );

        if( m > 0 )
        {
            buffer[m] = '\0';
            printf( "Buffer (%d bytes):\n=======\n%s\n=======\n", m, buffer );
            fflush(stdout);
        }

    } else if ( wheel == LEFT )
    {
        printf("Writing to left motor.\n");
        motor_flag = LEFT_MOTOR_STEPS_FLAG;
        n = n + write( serial_port, &motor_flag, 1 );
        n = n + write( serial_port, &steps, sizeof(steps) );
        n = n + write( serial_port, &runtime, sizeof(runtime) );
        printf("%d bytes written to left wheel\n", n );
        
        m = read( serial_port, &buffer, sizeof(buffer) );

        if( m > 0 )
        {
            buffer[m] = '\0';
            printf( "Buffer (%d bytes):\n=======\n%s\n=======\n", m, buffer );
            fflush(stdout);
        }

    } else if ( wheel == BOTH )
    {
        printf("Writing to both motors.\n");
        driveWheelSteps( RIGHT, steps, runtime, serial_port );
        driveWheelSteps( LEFT, steps, runtime, serial_port );

    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line. wheel: %d\n", wheel);
    }
}

void turn( int serial_port, int angle, int runtime )
{
    double arc_length;
    int steps;
    // arc length = 2 * PI * R * (theta/360)
    arc_length = M_PI * WHEEL_BASE_MM * ( angle / 360.0 );

    steps = round( STEPS_PER_MM * arc_length );

    printf("Turning %d degrees in %d seconds. arc length = %f and %d steps per wheel\n", angle, runtime, arc_length, steps );

    driveWheelSteps( RIGHT, -steps, runtime, serial_port );
    driveWheelSteps( LEFT, steps, runtime, serial_port );
}

void drive( int serial_port, float distance, int runtime )
{
    // Drives the robot forward at `speed` for `distance`, in cm.

    int steps = 0;

    steps = round( STEPS_PER_CM * distance );
    printf("Driving straight %d steps\n", steps );

    driveWheelSteps( BOTH, steps, runtime, serial_port );
}

void stop( int serial_port )
{
    driveWheelSteps( BOTH, 0, 0, serial_port );
}

void claw( int serial_port, int state )
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

void var_turn( int serial_port, int angle, int runtime )
{
    //pivots on one wheel. If turning right, pivot on right wheel, etc
    double arc_length;
    unsigned int steps;
    // arc length = 2 * PI * R * (theta/360)
    arc_length = 2 * M_PI * WHEEL_BASE_MM * ( angle / 360.0 ); //in mm

    steps = round( STEPS_PER_MM * arc_length );

    printf("Turning %d degrees in %d seconds. arc length = %f and %d steps.\n", angle, runtime, arc_length, steps );

    if ( angle < 0 ) // turning left
    {
        driveWheelSteps( RIGHT, steps, runtime, serial_port );
    } else if ( angle > 0 ) // turning right
    {
        driveWheelSteps( LEFT, steps, runtime, serial_port );
    } else
    {
        stop( serial_port );
    }
}