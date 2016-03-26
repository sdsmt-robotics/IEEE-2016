#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/sensors.h"
#include "../include/logger.h"

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

#define printf LOG

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
    // Forward 6"
    drive( 15, 2 );
    sleep(2);

    // 90 degree turn left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Forward until 6" from the wall
    drive( 35, 3);
    sleep(3);
    forward_until_obstacle( 210, 8 );

    // 90 degree turn left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Drive until in the "hospital"
    drive( 20, 3 );
    sleep(3);
    follow_left_wall_until_obstacle( 210, 5.5, 4 );

    // Dropoff victim
    claw(OPEN);
    claw(RAISE);

    // Reverse out and close the claw
    drive( -35, 3 );
    sleep(3);
    claw(CLOSE);

    // Rotate 90 degrees about right wheel then turn the other 90 like normal
    var_turn( FULL_RIGHT_TURN, 3 );
    sleep(3);
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    // Returning to CP now...
    follow_right_wall_until_end( 210, 6.0 );

    // After passing end of the right wall, move forward 6"
    drive( SIX_INCHES, 2 );
    sleep(2);

    // Turn 90 degrees right
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    // Drive forward until we hit a wall
    drive( SIX_INCHES, 2);
    sleep(2);
    forward_until_obstacle( 210, 0 );

    // Make a right turn
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    // We are now back in CP
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
    follow_right_wall_until_obstacle( 200, 6.0, 10.0 );
    claw( CLOSE );
    usleep(500*1000); //0.5 sec
    claw( RAISE );
    turn( LEFT_180, 4 );
    sleep(4);
    follow_left_wall_until_end( 200, 5.0 );
    claw( LOWER );
    stop();

    if ( victim_color == YELLOW )
    {
        cp_to_yellow();
    } else if ( victim_color == RED )
    {
        cp_to_red();
    } else if ( victim_color == UNKNOWN_COLOR )
    {
        printf("Crap. UNKNOWN_COLOR. What are you, blind?\n");
        cp_to_yellow();
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
