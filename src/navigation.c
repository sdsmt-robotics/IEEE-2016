#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/sensors.h"

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

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
