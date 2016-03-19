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
        // We need to ensure we're not adjusting for a wall that's not there.
        // It may have been turning itself towards the wall and reading again,
        // seeing the wall still (since it pointed itself at the wall).

        
        // poll_sensors();     // Why is this here??

        if ( left_value > target + WALL_FOLLOW_TOLERANCE )
        {
            printf("Too far away from left wall.\n");
            setWheelSpeed( RIGHT, speed + (speed/10.0 - 9) );

            // // Actually move towards the wall a little
            // usleep( TEN_MS );
            // setWheelSpeed( BOTH, speed + (speed/10.0 - 9) );

            // // Correct orientation
            // usleep( TEN_MS ); 
            // setWheelSpeed( RIGHT, speed - (speed/10.0 - 9) );
            // usleep( TEN_MS ); 

        }
        else if ( left_value < target - WALL_FOLLOW_TOLERANCE )
        {
            printf("Too close to left wall.\n");
            setWheelSpeed( RIGHT, speed - (speed/10.0 - 9) );

            // // Actually move towards the wall a little
            // usleep( TEN_MS ); 
            // setWheelSpeed( BOTH, speed + (speed/10.0 - 9) );
            
            // // Correct orientation
            // usleep( TEN_MS ); 
            // setWheelSpeed( RIGHT, speed + (speed/10.0 - 9) );
            // usleep( TEN_MS ); 
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, speed );
        }
        
        // usleep( TEN_MS );
        left_value = left_sensor();
        printf("Sensor value: %.2f.\n",left_value);
        // You could instead check the sensor here

    }
    setWheelSpeed( BOTH, speed );
    stop();
    int bytes = read( serial_port, &buffer, sizeof(buffer) );
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
        // usleep( 5*1000 ); // 10 mS
        right_value = right_sensor();
    }
    stop();
    int bytes = read( serial_port, &buffer, sizeof(buffer) );
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
        front_value = front_sensor();
        left_value = left_sensor();
        // usleep( TWENTY_MS );
    }
    stop();
    int bytes = read( serial_port, &buffer, sizeof(buffer) );
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
        front_value = front_sensor();
        right_value = right_sensor();
    }
    stop();
    int bytes = read( serial_port, &buffer, sizeof(buffer) );
    if ( bytes > 0 )
    {
        buffer[bytes] = '\0';
        printf("buffer (%d bytes): %s\n", bytes, buffer );
        fflush(stdout);
    }
}

void start_to_cp( )
{
    drive( 32, 4 );
    sleep(5);
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
    //drive forward 17 cm then turn
    drive( 15, 2 );
    sleep(2);
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 35, 3);
    sleep(3);
    forward_until_obstacle( 210, 8 );

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 20, 3 );
    sleep(3);
    follow_left_wall_until_obstacle( 210, 5.5, 4 );

    claw(OPEN);
    claw(RAISE);

    drive( -35, 3 );
    sleep(3);
    claw(CLOSE);

    var_turn( FULL_RIGHT_TURN, 3 );
    sleep(3);
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    follow_right_wall_until_end( 210, 6.0 );

    drive( SIX_INCHES, 2 );
    sleep(2);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    drive( SIX_INCHES, 2);
    sleep(2);
    forward_until_obstacle( 210, 0 );
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);
}


void cp_to_yellow()
{
    drive( 34, 3 );
    sleep(3);
    claw( OPEN );
    claw( RAISE );
    drive( -25, 3 );
    sleep(3);
    turn( LEFT_180, 4 );
    claw( CLOSE );
    sleep(1); //these sleeps add to 4
    claw( OPEN );
    sleep(1);
    claw( CLOSE );
    sleep(1);
    claw( OPEN );
    sleep(1);
    claw( CLOSE );
}

bool retrieve_victim_1()
{
    claw( OPEN );
    start_to_cp();
    drive( 18, 3 );
    sleep(3);
    follow_right_wall_until_obstacle( 210, 6.0, 6 );
    claw( CLOSE );
    usleep(500*1000); //0.5 sec
    claw( RAISE );
    turn( LEFT_180, 4 );
    sleep(4);
    follow_left_wall_until_end( 210, 5.0 );
    claw( LOWER );
    stop();

    if ( victim_color == YELLOW )
    {
        // cp_to_yellow();
    } else if ( victim_color == RED )
    {
        // cp_to_red();
    } else 
    {
        printf("crap\n");
    }
    stop();
   
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
