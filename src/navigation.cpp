#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/sensors.h"
#include "../include/logger.h"

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

//#define printf LOG

void start_to_cp( )
{
    claw( OPEN );
    drive( 32, 4 );
    sleep(4);
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);
    forward_until_obstacle( 190, 0.0 );
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);
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
    stop();
}

void cp_to_red()
{
    // Forward 6"
    drive( 17, 2 );
    sleep(2);

    // 90 degree turn left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Forward until 6" from the wall
    drive( 35, 3);
    sleep(3);
    forward_until_obstacle( 210, 9.5 );

    // 90 degree turn left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Drive until in the "hospital"
    drive( 20, 3 );
    sleep(3);
    follow_left_wall_until_obstacle( 210, 9.5, 4.0 );

    // Dropoff victim
    claw(OPEN);
    claw(RAISE);

    // Reverse out and close the claw
    drive( -35, 3 );
    sleep(3);
    claw(CLOSE);


    turn( RIGHT_180, 3 );
    sleep(3);

    // Returning to CP now...
    follow_right_wall_until_end( 210, 9.5 );

    // After passing end of the right wall, move forward 6"
    drive( 17, 2 );
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
    stop();
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
    stop();
}

void retrieve_victim_1()
{
    drive( 18, 3 );
    sleep(3);
    follow_right_wall_until_obstacle( 200, 5.5, 4.0 );
    claw( CLOSE );
    usleep(500*1000); //0.5 sec
    claw( RAISE );
    turn( LEFT_180, 4 );
    sleep(4);
    var_test_follow_left_wall_until_end( 200, 5.0 );
    claw( LOWER );
    stop();
    //a debug sleep
    sleep(2);

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
}

void retreive_victim_2()
{
    follow_left_wall_until_end( 190, WALL_FOLLOW_TARGET );
    drive( 15, 2 );
    sleep(2);
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);
    drive( 30, 4 );
    sleep(4);
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);
    drive( 10, 2 );
    sleep(2);
    follow_left_wall_until_obstacle( 190, 5.0, 10.0 );
    claw( CLOSE );
    usleep( 500*1000 );
    claw( RAISE );
    turn( RIGHT_180, 4 );
    sleep(4);
    follow_right_wall_until_end( 190, 5.0 );
    drive( 15, 2 );
    sleep(2);
    turn( FULL_RIGHT_TURN, 2 );
    claw( LOWER );
    sleep(2);
    forward_until_obstacle( 190, 10 );
    turn( FULL_RIGHT_TURN, 2 );
    claw( RAISE );
    sleep(2);
    follow_left_wall_until_end( 190, WALL_FOLLOW_TARGET );
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
}

void retreive_victim_3()
{
    // Start following right wall until Left gap
    sleep(2);
    follow_right_wall_until_left_open( 210, 6.0 );

    // drive across the gap
    sleep(2);
    drive( 35, 3 );

    // Start following right wall until Left gap
    sleep(2);
    follow_right_wall_until_left_open( 210, 6.0 );

    // 90 degrees left
    sleep(2);
    turn( FULL_LEFT_TURN, 2 );    

    // Drive forward towards grass
    sleep(2);
    drive( 40, 3 );   

    // 45 degrees to the right
    sleep(3);
    turn(HALF_RIGHT_TURN, 2);

    // Drive forward 
    sleep(2);
    drive( 40, 3 );   

    // 45 degrees to the left
    sleep(3);
    turn(HALF_LEFT_TURN, 2); 

    // Set victim location (A or B)
    sleep(2);
    //SetVictimLocation(); //This is a void function that determines whether there is a victim in front of us or not


    sleep(2);
    if (true/*A*/)
    {
        // Follow right wall until a victim is in front of us
        getVictim();
    }
    else
    {
        // Running to the end of the map
        follow_right_wall_until_obstacle( 200, 6.0, 10.0 );

        // 90 degree left
        sleep(2);
        turn( FULL_LEFT_TURN, 2 ); 

        // Follow right wall until a victim is in front of us
        sleep(2);
        getVictim();
    }

    ////////////////////// RETURNING //////////////////////

    // Flip around
    sleep(2);
    turn( LEFT_180, 2 );

    // Running to the end of the map
    sleep(2);
    follow_left_wall_until_obstacle( 200, 6.0, 10.0 );

    // 90 degree right turn
    sleep(2);
    turn( FULL_RIGHT_TURN, 2 );

    // Running to the end of the map
    sleep(2);
    follow_left_wall_until_obstacle( 200, 6.0, 10.0 );   

    // 90 degree right turn
    sleep(2);
    turn( FULL_RIGHT_TURN, 2 );  

    // Forward 12"
    sleep(2);
    drive(31, 3);

    // Set victim 4's location
    sleep(3);
    SetVictimLocation();

    // 90 degree left turn
    sleep(2);
    turn( FULL_LEFT_TURN, 2 );  

    // Forward until the wall
    sleep(2);
    forward_until_obstacle( 190, 1 );

    // 90 degree right turn
    sleep(2);
    turn( FULL_RIGHT_TURN, 2 );  

    // Follow wall till CP
    sleep(2);
    follow_left_wall_until_end( 200, 5.0 );

    sleep(5);
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
}

void retreive_victim_4()
{
    /*
    * instructions for if the fourth person is on the near side of the river:
    * wall follow left until break in wall
    * turn left and move forward until the green section has been reached
    * turn left and approach the fourth victim
    * grab victim and turn aroiund
    * retrace steps to CP
    */
    if ( true /*A*/)
    {
        //wall follow from CP to the first break in the left wall
        follow_left_wall_until_end ( 190, WALL_FOLLOW_TARGET );
        //turn left 90 degrees
        turn ( FULL_LEFT_TURN, 2 );
        sleep (2);
        //drive forward to the green area
        drive (64, 8);
        sleep (8);
        //turn left 90 degrees
        turn ( FULL_LEFT_TURN, 2);
        sleep (2);
        //drive forward to the victim
        drive (72.5, 10);
        sleep (10);
        //grab the victim and lift it up
        claw (CLOSE);
        claw (RAISE);
        //turn around
        turn (RIGHT_180, 4);
        sleep (4);
        //drive back the way we came
        drive (72.5, 10);
        sleep (10);
        //turn right 90 degrees
        turn ( FULL_RIGHT_TURN, 2);
        sleep (2);
        //drive back into the city area
        drive (64, 8);
        sleep (8);
        //turn right 90 degrees
        turn ( FULL_RIGHT_TURN, 2);
        sleep (2);
        //return to the CP
        follow_left_wall_until_end ( 190, WALL_FOLLOW_TARGET);
    }
    
    /*
    * instructions for if the fourth person is on the far side of the river:
    * wall follow right until left gap
    * wall follow right until left gap
    * turn left and move forward to the point marked on the blue tape
    * make dead reckoning angled turn to get to the second marked point
    * wall follow right until the corner is reached
    * turn left and wall follow right until corner
    * turn left and move forward until victim 4 is reached
    * grab victim 4 and turn around.
    * retrace steps to CP
    */
    else
    {
        //navigate past the first left break
        follow_right_wall_until_left_open ( 190, WALL_FOLLOW_TARGET );
        //get past the break, should now be able to read the left wall
        drive (35, 3);
        sleep (3);
        //navigate to the second left break
        follow_right_wall_until_left_open ( 190, WALL_FOLLOW_TARGET );
        //turn 90 degrees to the left
        turn ( FULL_LEFT_TURN, 2);
        sleep (2);
        //drive forward to the edge of the green area
        drive ( 51, 8);
        sleep (8);
        //turn at a 45 degree angle to get past the blocks
        turn ( HALF_RIGHT_TURN, 1);
        sleep (1);
        //drive towards the wall.
        drive (43.2, 6);
        sleep (6);
        //turn to be parallel with the wall
        turn ( HALF_LEFT_TURN, 1);
        sleep (1);
        //wall follow until the corner is reached
        follow_right_wall_until_obstacle ( 190, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn ( FULL_LEFT_TURN, 2);
        sleep (2);
        //wall follow until the corner is reached
        follow_right_wall_until_obstacle ( 190, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn ( FULL_LEFT_TURN, 2);
        //drive to the victim
        drive ( 17.8, 2);
        sleep (2);
        //grab and lift the victim
        claw (CLOSE);
        claw (RAISE);
        //turn around
        turn ( LEFT_180, 4);
        sleep (4);
        //wall follow to the corner again
        follow_left_wall_until_obstacle ( 190, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn ( FULL_RIGHT_TURN, 2);
        sleep (2);
        //wall follow until the corner is reached
        follow_left_wall_until_obstacle ( 190, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn ( FULL_RIGHT_TURN, 2);
        sleep (2);
        //drive back to the point where the obstacles were avoided
        drive ( 94, 7);
        //make a 45 degree turn right
        turn ( HALF_RIGHT_TURN, 1);
        //drive off of the green area
        drive ( 43.2, 6);
        sleep (6);
        //turn left 45 degrees
        turn ( HALF_LEFT_TURN, 1);
        sleep (1);
        //drive back into the city section
        drive (51, 8);
        sleep (8);
        //turn right 90 degrees
        turn ( FULL_RIGHT_TURN, 2);
        sleep (2);
        //wall follow back to CP
        follow_left_wall_until_end (190, WALL_FOLLOW_TARGET);
    }
    
    
    /*
    * instructions for after reaching CP:
    * determine color
    * call the appropriate drop off function
    * After the victim has been dropped off, return to CP
    * After returning to CP, return to initial zone
    * power off.
    */
}

void get_to_cp(   )
{
    //stuff here
}
