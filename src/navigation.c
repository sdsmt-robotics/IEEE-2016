/*
* Description: This file will govern our navigation logic, and will handle
* the speed of our wheels using the input from our sensors.
*
* Expected Inputs: Currently we have 3 IR sensors; left, right, and center.
*
* Outputs: We will command the speed of the stepper-motors for two wheels.
*
* Requirements: We have to be able to handle driving straight ahead, turning
* left and right in place, and driving in reverse.
*/
#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/sensors.h"

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

void forward_until_obstacle( unsigned char speed, int tolerance )
{
    setWheelSpeed( BOTH, speed );
    double front_value = 200;

    while ( front_value > SIX_INCHES + tolerance )
    {
        front_value = front_sensor();
        printf("Not hitting wall yet.\n");
        printf("front: %.2f\n", front_value );
        usleep( 20*1000 );
    }
}

void forward_until_left_end( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double left_value = 0;

    while ( left_value < INF_DISTANCE )
    {
        printf("Following left wall.\n");
        left_value = left_sensor();
        usleep( 20*1000 );
    }
}

void forward_until_right_end( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double right_value = 0;

    while ( right_value < INF_DISTANCE )
    {
        printf("Following right wall.\n");
        right_value = right_sensor();
        usleep( 20*1000 );
    }
    stop();
}

/*  I commented out the original (Kyle)
void follow_left_wall_until_end( unsigned char speed, int target )
{
    setWheelSpeed( BOTH, speed );
    double left_value = 0;
    while ( left_value < INF_DISTANCE )
    {
        left_value = left_sensor();
        poll_sensors();
        printf("======\n");
        printf("left: %.1f\n", left_value );
        printf("======\n");
        if ( left_value > target + WALL_FOLLOW_TOLERANCE )
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( RIGHT, speed + (speed/10.0 - 9) );
        }
        else if ( left_value < target - WALL_FOLLOW_TOLERANCE )
        {
            printf("Too close to wall.\n");
            setWheelSpeed( RIGHT, speed - (speed/10.0 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        usleep( 20*1000 ); // 10 mS
    }
    stop();
}*/


void follow_left_wall_until_end( unsigned char speed, int target )
{
    setWheelSpeed( BOTH, speed );
    double left_value = left_sensor();

    while ( left_value < INF_DISTANCE )
    {
        // We need to ensure we're not adjusting for a wall that's not there.
        // It may have been turning itself towards the wall and reading again,
        // seeing the wall still (since it pointed itself at the wall).
        
        poll_sensors();     // Why is this here??
        printf("======\n");
        printf("left: %.1f\n", left_value );
        printf("======\n");
        if ( left_value > target + WALL_FOLLOW_TOLERANCE )
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( RIGHT, speed + (speed/10.0 - 9) );
        }
        else if ( left_value < target - WALL_FOLLOW_TOLERANCE )
        {
            printf("Too close to wall.\n");
            setWheelSpeed( RIGHT, speed - (speed/10.0 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        usleep( 20*1000 ); // 10 mS
        left_value = left_sensor();
        // You could instead check the sensor here
    }
    stop();
}

void follow_right_wall_until_end( unsigned char speed, int target )
{
    setWheelSpeed( BOTH, speed );
    double right_value = 0;
    while ( right_value < INF_DISTANCE )
    {
        right_value = right_sensor(); // has 10 mS delay inside
        printf("right: %.2f\n", right_value);
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
        usleep( 20*1000 ); // 10 mS
    }
    stop();
}

void follow_left_wall_until_obstacle( unsigned char speed, int target, int tolerance )
{
    setWheelSpeed( BOTH, speed );
    double left_value = 0;
    double front_value = 200;

    while ( front_value > SIX_INCHES + tolerance )
    {
        front_value = front_sensor();
        left_value = left_sensor();

        printf("front_value: %.2f\n", front_value);
        printf("left_value: %.2f\n", left_value);
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
        usleep( 20*1000 );
    }
    stop();
}

void follow_right_wall_until_obstacle( unsigned char speed, int target, int tolerance )
{
    setWheelSpeed( BOTH, speed );
    double right_value = 0;
    double front_value = 200;

    while ( front_value > SIX_INCHES + tolerance )
    {
        front_value = front_sensor();
        right_value = right_sensor();

        printf("front: %.2f\n", front_value );
        printf("right: %.2f\n", right_value );

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
        usleep( 20*1000 );
    }
    stop();
}

void start_to_cp( )
{
    follow_left_wall_until_end( 190, WALL_FOLLOW_TARGET );  // This is 6, below it is 5???
    drive( SIX_INCHES, 2 ); // drive forward six inches in 2 seconds
    sleep(3);
    turn( FULL_LEFT_TURN, 2 );
    sleep(3);
    forward_until_obstacle( 190, 1 );
    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);
}

void cp_to_start()
{
    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);
    forward_until_obstacle( 190, 1 );

    turn( FULL_LEFT_TURN, 2 );
    sleep(3);

    // Reverse 6.5 inches
    drive( -18, 2 );
    sleep(3);

    claw( OPEN );
    claw( LOWER );

}


void cp_to_red()
{
    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);

    forward_until_obstacle( 190, 0 );

    turn( FULL_LEFT_TURN, 2 );
    sleep(3);
    follow_left_wall_until_obstacle( 190, 0, WALL_FOLLOW_TARGET );

    claw(LOWER);
    claw(OPEN);
    sleep(1);
    claw(RAISE);
    sleep(1);
    // Note: May have to reverse first!
    claw(CLOSE);

    sleep(1);

    drive( -35, 3 );
    sleep(4);
    turn( RIGHT_180, 3 );
    sleep(4);

    follow_right_wall_until_end( 190, WALL_FOLLOW_TARGET );

    drive( SIX_INCHES, 2 );
    sleep(3);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);

    forward_until_obstacle( 190, 0 );
    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);
}


void cp_to_yellow( )
{
    turn( RIGHT_180, 4 );
    sleep(5);

    forward_until_obstacle( 190, 0 );
    sleep(1);

    claw(LOWER);
    claw(OPEN);
    claw(RAISE);
    // Note: May have to reverse first!
    claw(CLOSE);

    drive( -2*SIX_INCHES, 4 );
    sleep(5);

    turn( LEFT_180, 4 );
    sleep(5);
}

bool pick_up_victim_1()
{
    claw( OPEN );
    start_to_cp();
    sleep(1);
    // follow_left_wall_until_end( 190, 10 );
    drive( 18, 3 );
    sleep(4);
    follow_right_wall_until_obstacle( 210, 6.0, 10 );
    claw( CLOSE );
    sleep(1);
    claw( RAISE );
    turn( LEFT_180, 4 );
    sleep(5);
    stop();

    return true;
}

bool drop_off_victim_1()
{
    follow_left_wall_until_end( 180, 5.0 );     // Should this be 5 or 6???
    claw( LOWER );
    sleep(1);
    forward_until_obstacle( 190, 0 );
    claw( OPEN );
    drive( -1.8*SIX_INCHES, 3 );
    sleep(4);
    turn( LEFT_180, 4 );
    claw( CLOSE );
    sleep(1);
    claw( OPEN );
    sleep(1);
    claw( CLOSE );
    sleep(1);
    claw( OPEN );
    sleep(1);
    claw( CLOSE );

    return true;
}

bool retreive_victim_2()
{
    //get to person 2:
        //62cm from cp
        //rotate -90
        //forward 30 cm
        //rotate -90
        //forward 91 cm
    //grab person 2
    //go to cp
    //call appropriate cp_to_hospital() (this function will peek at the global color var)
    //return to cp
    return false;
}

bool retreive_victim_3()
{
    //forward 24 in
    //rotate -90
    //forward 12.5 in
    //rotate 90
    //forward 25 in
    //rotate -90
    //
    return false;
}

bool retreive_victim_4()
{

    return false;
}

void get_to_cp(   )
{
    //stuff here
}
