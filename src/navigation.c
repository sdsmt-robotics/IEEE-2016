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

void forward_until_obstacle( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double front_value = 200;

    while ( front_value > SIX_INCHES + FRONT_STOPPING_TOLERANCE )
    {
        front_value = front_sensor();
        printf("Not hitting wall yet.\n");
        //printf("front: %f\n", front_value );
        usleep( 10*1000 );
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
        usleep( 10*1000 );
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
        usleep( 10*1000 );
    }
    stop();
}

void follow_left_wall_until_end( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double left_value = 0;
    while ( left_value < INF_DISTANCE )
    {
        left_value = left_sensor();
        if ( left_value > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( RIGHT, speed + (speed/10 - 9) );
        }
        else if ( left_value < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            setWheelSpeed( RIGHT, speed - (speed/10 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        usleep( 10*1000 ); // 10 mS
    }
    stop();
}

void follow_right_wall_until_end( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double right_value = 0;
    while ( right_value < INF_DISTANCE )
    {
        right_value = right_sensor(); // has 10 mS delay inside
        if ( right_value > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( LEFT, speed + (speed/10 - 9) );
        }
        else if ( right_value < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            setWheelSpeed( LEFT, speed - (speed/10 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        usleep( 10*1000 ); // 10 mS
    }
    stop();
}

void follow_left_wall_until_obstacle( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double left_value = 0;
    double front_value = 200;

    while ( front_value > SIX_INCHES + FRONT_STOPPING_TOLERANCE )
    {
        front_value = front_sensor();
        left_value = left_sensor();
        if ( left_value > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( RIGHT, speed + (speed/10 - 9) );
        }
        else if ( left_value < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            setWheelSpeed( RIGHT, speed - (speed/10 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        usleep( 10*1000 ); // 10 mS
    }
    stop();
}

void follow_right_wall_until_obstacle( unsigned char speed )
{
    setWheelSpeed( BOTH, speed );
    double right_value = 0;
    double front_value = 200;

    while ( front_value > SIX_INCHES + FRONT_STOPPING_TOLERANCE )
    {
        front_value = front_sensor();
        right_value = right_sensor();
        if ( right_value > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( LEFT, speed + (speed/10 - 9) );
        }
        else if ( right_value < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            setWheelSpeed( LEFT, speed - (speed/10 - 9) );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        usleep( 10*1000 ); // 10 mS
    }
    stop();
}

void start_to_cp( )
{
    follow_left_wall_until_end( 190 );
    drive( SIX_INCHES, 2 ); // drive forward six inches in 2 seconds
    sleep(3);
    turn( FULL_LEFT_TURN, 2 );
    sleep(3);
    forward_until_obstacle( 190 );
    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);
}

void cp_to_start()
{
    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);
    forward_until_obstacle( 190 );

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

    forward_until_obstacle( 190 );

    turn( FULL_LEFT_TURN, 2 );
    sleep(3);
    follow_left_wall_until_obstacle( 190 );

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

    follow_right_wall_until_end( 190 );

    drive( SIX_INCHES, 2 );
    sleep(3);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);

    forward_until_obstacle( 190 );
    turn( FULL_RIGHT_TURN, 2 );
    sleep(3);
}


void cp_to_yellow( )
{
    turn( RIGHT_180, 4 );
    sleep(5);

    forward_until_obstacle( 190 );
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






bool retreive_victim_1(   )
{
    //open claws
    //forward 183.5cm

    //grab victim

    //reverse 183.5 cm
    //call appropriate cp_to_hospital() (Peeks at glocal color variable, or status struct)
    //return to cp
    return false;
}

bool retreive_victim_2(   )
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

bool retreive_victim_3(   )
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

bool retreive_victim_4(   )
{

    return false;
}

void get_to_cp(   )
{
    //stuff here
}
