#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/sensors.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>

//#define printf LOG

void setWheelSpeed( int wheel, unsigned char speed )
{
    unsigned char motor_flag = 0;
    int bytes = 0; // To make gcc shut up about ignoring the return value

    if ( wheel == RIGHT )
    {
        //printf("Writing %d to right motor.\n", speed);
        motor_flag = RIGHT_MOTOR_FLAG;
        bytes = bytes + write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to motor\n", bytes );
    } else if ( wheel == LEFT )
    {
        //printf("Writing %d to left motor.\n", speed);
        motor_flag = LEFT_MOTOR_FLAG;
        bytes = bytes + write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to motor\n", bytes );
    } else if ( wheel == BOTH )
    {
        //printf("Writing %d to both motors.\n", speed);
        //printf("Writing %d to right motor.\n", speed);
        motor_flag = RIGHT_MOTOR_FLAG;
        bytes = bytes + write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to motor\n", bytes );

        //printf("Writing %d to left motor.\n", speed);
        motor_flag = LEFT_MOTOR_FLAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to left motor\n", bytes );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line %d\n", wheel);
    }
}

void driveWheelSteps( int wheel, int steps, int runtime )
{
    //printf("Driving wheel %d %d steps in %d seconds\n", wheel, steps, runtime);
    int n = 0;
    //int m = 0;
    unsigned char motor_flag = 0;
    //char buffer[512] = "";

    if ( wheel == RIGHT )
    {
        //printf("Writing to right motor.\n");
        motor_flag = RIGHT_MOTOR_STEPS_FLAG;
        n = n + write( send_port, &motor_flag, 1 );
        n = n + write( send_port, &steps, sizeof(steps) );
        n = n + write( send_port, &runtime, sizeof(runtime) );
    } else if ( wheel == LEFT )
    {
        //printf("Writing to left motor.\n");
        motor_flag = LEFT_MOTOR_STEPS_FLAG;
        n = n + write( send_port, &motor_flag, 1 );
        n = n + write( send_port, &steps, sizeof(steps) );
        n = n + write( send_port, &runtime, sizeof(runtime) );
    } else if ( wheel == BOTH )
    {
        //printf("Writing to both motors.\n");
        driveWheelSteps( RIGHT, steps, runtime );
        driveWheelSteps( LEFT, steps, runtime );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line. wheel: %d\n", wheel);
    }
}

void turn( int angle, int runtime )
{
    double arc_length = M_PI * WHEEL_BASE_MM * ( angle / 360.0 );
    int steps = round( STEPS_PER_MM * arc_length );

    //printf("Turning %d degrees in %d seconds. arc length = %f and %d steps per wheel\n", angle, runtime, arc_length, steps );
    driveWheelSteps( RIGHT, -steps, runtime );
    driveWheelSteps( LEFT, steps, runtime );
}

void var_turn( int angle, int runtime )
{
    //pivots on one wheel. If turning right, pivot on right wheel, etc
    double arc_length;
    unsigned int steps;

    arc_length = 2 * M_PI * WHEEL_BASE_MM * ( angle / 360.0 );
    steps = round( STEPS_PER_MM * arc_length );

    //printf("Turning %d degrees in %d seconds. arc length = %f and %d steps.\n", angle, runtime, arc_length, steps );
    stop();
    if ( angle < 0 ) // turning left
    {
        driveWheelSteps( RIGHT, steps, runtime );
    } else if ( angle > 0 ) // turning right
    {
        driveWheelSteps( LEFT, steps, runtime );
    } else
    {
        stop();
    }
    stop();
}

void drive( float distance, int runtime )
{
    int steps = round( STEPS_PER_CM * distance );

    driveWheelSteps( BOTH, steps, runtime );
}

void stop()
{
    setWheelSpeed( BOTH, 127 );
}

void claw( int state )
{
    unsigned char motor_flag;
    int bytes = 0;
    unsigned char value;

    if ( state == RAISE )
    {
        value = RAISE_VAL;
        printf("Raising claws. Victim begins to scream.\n");
        motor_flag = SERVO_CLAW_RAISE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else if (state == LOWER )
    {
        value = LOWER_VAL;
        printf("Lowering claws. Victim breathes a sigh of relief.\n");
        motor_flag = SERVO_CLAW_RAISE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else if ( state == OPEN )
    {
        value = OPEN_VAL;
        printf("Opening claws. Victim barely escapes with their life.\n");
        motor_flag = SERVO_CLAW_CLOSE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else if ( state == CLOSE )
    {
        value = CLOSE_VAL;
        printf("Closing claws. Crushing victim.\n");
        motor_flag = SERVO_CLAW_CLOSE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else {
        printf("I don't know what you mean by that.\n");
        printf("Attempting to send state: %d defined in robot_defines under claw state.\n", state );
    }
}

void forward_until_obstacle( unsigned char speed, int tolerance )
{
    setWheelSpeed( BOTH, speed );
    double front_value = front_sensor();

    while ( front_value > SIX_INCHES + tolerance )
    {
        printf("Not hitting wall yet.\n");
        printf("front: %.2f\n", front_value );
        usleep( TWENTY_MS );
        front_value = front_sensor();
    }
    stop();
}

void forward_until_left_end( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double left_value = 0;

    while ( left_value < INF_DISTANCE )
    {
        printf("Following left wall.\n");
        left_value = left_sensor();
        usleep( TWENTY_MS );
    }
    stop();
}

void forward_until_right_end( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double right_value = 0;

    while ( right_value < INF_DISTANCE )
    {
        printf("Following right wall.\n");
        right_value = right_sensor();
        usleep( TWENTY_MS );
    }
    stop();
}

void follow_left_wall_until_end( unsigned char speed, int target )
{
    setWheelSpeed( BOTH, speed );
    double left_value = left_sensor();
    char buffer[512] = "";

    while ( left_value < 6.75 )
    {
        if ( left_value > target + WALL_FOLLOW_TOLERANCE )
        {
            printf("Too far away from left wall.\n");
            setWheelSpeed( RIGHT, speed + (speed/10.0 - 9) );
        }
        else if ( left_value < target - WALL_FOLLOW_TOLERANCE )
        {
            printf("Too close to left wall.\n");
            setWheelSpeed( RIGHT, speed - (speed/10.0 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        left_value = left_sensor();
        printf("Sensor value: %.2f.\n",left_value);
    }
    setWheelSpeed( BOTH, speed );
    stop();
    int bytes = read( receive_port, &buffer, sizeof(buffer) );
    if ( bytes > 0 )
    {
        buffer[bytes] = '\0';
        printf("buffer (%d bytes): %s\n", bytes, buffer );
        fflush(stdout);
    }
}

void follow_right_wall_until_end( unsigned char speed, int target )
{
    setWheelSpeed( BOTH, speed );
    double right_value = right_sensor();
    char buffer[512] = "";

    while ( right_value < 10.0 )
    {
        
        if ( right_value > target + WALL_FOLLOW_TOLERANCE )
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( LEFT, speed + (speed/10 - 9) );
        }
        else if ( right_value < target - WALL_FOLLOW_TOLERANCE )
        {
            printf("Too close to wall.\n");
            setWheelSpeed( LEFT, speed - (speed/10 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        right_value = right_sensor();
        printf("right: %.2f\n", right_value);
    }
    stop();
    int bytes = read( receive_port, &buffer, sizeof(buffer) );
    if ( bytes > 0 )
    {
        buffer[bytes] = '\0';
        printf("buffer (%d bytes): %s\n", bytes, buffer );
        fflush(stdout);
    }
}

void follow_left_wall_until_obstacle( unsigned char speed, int target, int tolerance )
{
    setWheelSpeed( BOTH, speed );
    double front_value = front_sensor();
    double left_value = left_sensor();
    char buffer[512] = "";

    while ( front_value > SIX_INCHES + tolerance )
    {
        if ( left_value > target + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( RIGHT, speed + (speed/10 - 9) );
        }
        else if ( left_value < target - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            setWheelSpeed( RIGHT, speed - (speed/10 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        front_value = front_sensor();
        left_value = left_sensor();
        // With no delay in here, it's possible that front_value could actually hold left_value, and vise versa
        printf("front_value: %.2f\n", front_value);
        printf("left_value: %.2f\n", left_value);
    }
    stop();
    int bytes = read( receive_port, &buffer, sizeof(buffer) );
    if ( bytes > 0 )
    {
        buffer[bytes] = '\0';
        printf("buffer (%d bytes): %s\n", bytes, buffer );
        fflush(stdout);
    }
}

void follow_right_wall_until_obstacle( unsigned char speed, int target, int tolerance )
{
    setWheelSpeed( BOTH, speed );
    double front_value = front_sensor();
    double right_value = right_sensor();
    char buffer[512] = "";

    while ( front_value > SIX_INCHES + tolerance )
    {
        if ( right_value > target + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( LEFT, speed + (speed/10.0 - 9) );
        }
        else if ( right_value < target - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            setWheelSpeed( LEFT, speed - (speed/10.0 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        front_value = front_sensor();
        right_value = right_sensor();
        // With no delay in here, it's possible that front_value could actually hold left_value, and vise versa
        printf("front: %.2f\n", front_value );
        printf("right: %.2f\n", right_value );
    }
    stop();
    int bytes = read( receive_port, &buffer, sizeof(buffer) );
    if ( bytes > 0 )
    {
        buffer[bytes] = '\0';
        printf("buffer (%d bytes): %s\n", bytes, buffer );
        fflush(stdout);
    }
}

/*
* This version of the left wall following function should damp the "sine wave"
* pattern we have been noticing. This should reduce irregularities in the
* performance of the robot while pathing. If this method works well we should
* be able to apply this to the right wall following function as well.
*/
void test_follow_left_wall_until_end (unsigned char speed, int target)
{
    double left_value = left_sensor();
    char buffer[512] = "";
    char speed_mod = speed/10;
    int bytes_read = 0;
    double divis_val = 2;
    setWheelSpeed(BOTH, speed);
    printf("left: %.1f\n", left_value );
    while ( left_value < 6.5 ) // while the wall is there, 6.5 is "infinite"
    {
        while( left_value > target + WALL_FOLLOW_TOLERANCE )
        {
            printf("Too far away from left wall.\n");
            // increase speed of right wheel
            setWheelSpeed( RIGHT, speed + speed_mod );
            left_value = left_sensor();
            printf("left: %.1f.\n", left_value);
            speed_mod /= divis_val; //play with this value, 2 seems high if this is going to loop repeatedly
        }
        while( left_value < target - WALL_FOLLOW_TOLERANCE )
        {
            printf("Too close to left wall.\n");
            // decrease speed of right wheel
            setWheelSpeed( RIGHT, speed - speed_mod );
            left_value = left_sensor();
            printf("left: %.1f.\n", left_value);
            speed_mod /= divis_val; //play with this value, 2 seems high if this is going to loop repeatedly
        }
        left_value = left_sensor();
        printf("left: %.1f.\n", left_value);
    }
    setWheelSpeed( BOTH, speed );
    stop();

    bytes_read = read( receive_port, &buffer, sizeof(buffer) );
    if ( bytes_read > 0 )
    {
        buffer[bytes_read] = '\0';
        printf("buffer (%d bytes): %s\n", bytes_read, buffer );
        fflush(stdout);
    }
}
