#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/sensors.h"
#include "../include/logger.h"
#include "../include/vision.h"


#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#define printf LOG

void start_to_cp( )
{
    //drives the robot from the start point to the central point in front of the yellow hospital
    printf("function: %s\n", __func__);

    claw( OPEN );
    claw( RAISE );

    drive( 32, 4 );
    sleep(4);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    forward_until_obstacle( 220, 0.0 );
    turn( FULL_RIGHT_TURN, 2 );
    
    sleep(2);
    claw( LOWER );

    stop();
}

void cp_to_start()
{
    //drives the robot from the central point in front of the yellow hospital to the starting point
    printf("function: %s\n", __func__);

    turn( 80, 2 );
    sleep(2);

    forward_until_obstacle( 220, 2.0 );

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( -35, 2 );
    sleep(2);

    claw( OPEN );
    claw( LOWER );
    usleep( 500*1000 );
    claw( CLOSE );
    usleep( 500*1000 );
    claw( OPEN );
    usleep( 500*1000 );
    claw( CLOSE );
    usleep( 500*1000 );
    claw( CLOSE );
    usleep( 500*1000 );
    claw( OPEN );
    usleep( 500*1000 );
    claw( CLOSE );

    stop();
}

void cp_to_red()
{
    //drops a victim off in the red hospital and returns to the central point
    printf("function: %s\n", __func__);

    // Forward 6"
    drive( 17, 2 );
    sleep(2);

    // 90 degree turn left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Forward until 6" from the wall
    drive( 35, 2 );
    claw( LOWER );
    sleep(2);

    forward_until_obstacle( 220, 12.5 );

    // 90 degree turn left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Drive until in the "hospital"
    drive( 20, 2 );
    sleep(2);
    follow_left_wall_until_obstacle( 220, 11.5, 4.0 );

    // Dropoff victim
    claw(OPEN);
    claw(RAISE);

    // Reverse out and close the claw
    drive( -35, 3 );
    sleep(3);

    turn( RIGHT_180, 3 );
    sleep(3);

    // Returning to CP now...
    follow_right_wall_until_end( 220, 9.5 );

    // After passing end of the right wall, move forward 6"
    drive( 17, 2 );
    sleep(2);

    // Turn 90 degrees right
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    // Drive forward until we hit a wall
    drive( SIX_INCHES, 2 );
    sleep(2);
    forward_until_obstacle( 220, 0.0 );

    // Make a right turn
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);
    claw( LOWER );

    // We are now back in CP
    stop();
}


void cp_to_yellow()
{
    //drops a victim off in the yellow hospital and returns to the central point
    printf("function: %s\n", __func__);

    claw( LOWER );
    usleep(500*1000); // enough time for the claws to lower before we look at the front sensor

    forward_until_obstacle( 220, 0.0 );

    claw( OPEN );
    claw( RAISE );

    drive( -25, 3 );
    sleep(3);

    turn( RIGHT_180, 3 );
    sleep(3);

    stop();
}

void retrieve_victim_1()
{
    //goes from the CP, grabs victim one, and returns back to the CP
    printf("function: %s\n", __func__);

    drive( 18, 2 );
    sleep(2);

    follow_right_wall_until_obstacle( 220, 5.5, 5.0 );

    drive( -10, 2 );
    sleep(2);

    set_global_colors();

    drive( 10, 2 );
    sleep(2);

    claw( CLOSE );
    usleep(500*1000); //0.5 sec
    claw( RAISE );

    turn( LEFT_180, 4 );
    sleep(4);

    follow_left_wall_until_end( 220, 5.0 );
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
        cp_to_red();
    }

    clear_global_colors();
    stop();
}

void retrieve_victim_2()
{
    //grabs victim 2 and returns to the CP
    printf("function: %s\n", __func__);

    follow_left_wall_until_end( 200, 8.0 );

    drive( 17, 2 );
    sleep(2);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 31, 4 );
    sleep(4);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 10, 2 );
    claw( OPEN );
    sleep(2);

    follow_left_wall_until_obstacle( 200, 7.0, 3.0 );

    drive( -10, 2 );
    sleep(2);

    set_global_colors();

    drive( 10, 2 );
    sleep(2);

    claw( CLOSE );
    usleep( 500*1000 );
    claw( RAISE );

    turn( RIGHT_180, 4 );
    sleep(4);

    follow_right_wall_until_end( 200, 5.0 );

    drive( 16, 2 );
    sleep(2);

    turn( FULL_RIGHT_TURN, 2 );
    claw( LOWER );
    sleep(2);

    forward_until_obstacle( 200, 2.0 );

    turn( FULL_RIGHT_TURN, 2 );
    claw( RAISE );
    sleep(2);

    follow_left_wall_until_end( 200, 7.5 );
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
        cp_to_red();
    }

    clear_global_colors();
    stop();
}

void retrieve_victim_3()
{
    //hopefully grabs victim 3 and returns to the CP
    printf("function: %s\n", __func__);
    // Start following right wall until Left gap

    follow_right_wall_until_left_open( 220, 6.0 );

    // drive across the gap
    drive( 35, 3 );
    sleep(3);

    // Start following right wall until Left gap
    follow_right_wall_until_left_open( 220, 6.0 );

    // 90 degrees left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Drive forward towards grass
    drive( 40, 4 );
    sleep(4);

    // 45 degrees to the right
    turn(HALF_RIGHT_TURN, 1);
    sleep(1);

    // Drive forward 
    drive( 40, 3 );
    sleep(3);

    // 45 degrees to the left
    turn(HALF_LEFT_TURN, 2);
    sleep(2);

    // Set victim location (A or B)
    //SetVictimLocation(); //This is a void function that determines whether there is a victim in front of us or not
    if (true/*A*/)
    {
        // Follow right wall until a victim is in front of us
        getVictim();
    }
    else
    {
        // Running to the end of the map
        follow_right_wall_until_obstacle( 220, 6.0, 10.0 );

        // 90 degree left
        turn( FULL_LEFT_TURN, 2 ); 
        sleep(2);

        // Follow right wall until a victim is in front of us
        getVictim();
    }

    // Flip around
    turn( LEFT_180, 2 );
    sleep(2);

    // Running to the end of the map
    follow_left_wall_until_obstacle( 220, 6.0, 10.0 );

    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    // Running to the end of the map
    follow_left_wall_until_obstacle( 220, 6.0, 10.0 );   

    // 90 degree right turn
    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    // Forward 12"
    drive(31, 3);
    sleep(3);

    // Set victim 4's location
    SetVictimLocation();

    // 90 degree left turn
    
    turn( FULL_LEFT_TURN, 2 );  
    sleep(2);

    // Forward until the wall
    
    forward_until_obstacle( 220, 1 );
    sleep(2);

    // 90 degree right turn
    
    turn( FULL_RIGHT_TURN, 2 );  
    sleep(2);

    // Follow wall till CP
    
    follow_left_wall_until_end( 220, 5.0 );


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

void retrieve_victim_4()
{
    //definitely grabs victim 4 and returns the poor bastard to the CP
    printf("function: %s\n", __func__);
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
        follow_left_wall_until_end( 220, WALL_FOLLOW_TARGET );
        //turn left 90 degrees
        turn( FULL_LEFT_TURN, 2 );
        sleep(2);
        //drive forward to the green area
        drive( 64, 8 );
        sleep(8);
        //turn left 90 degrees
        turn( FULL_LEFT_TURN, 2 );
        sleep(2);
        //drive forward to the victim
        drive( 72.5, 10 );
        sleep(10);
        //grab the victim and lift it up
        claw( CLOSE );
        claw( RAISE );
        //turn around
        turn( RIGHT_180, 4 );
        sleep(4);
        //drive back the way we came
        drive( 72.5, 10 );
        sleep(10);
        //turn right 90 degrees
        turn( FULL_RIGHT_TURN, 2 );
        sleep(2);
        //drive back into the city area
        drive( 64, 8 );
        sleep(8);
        //turn right 90 degrees
        turn( FULL_RIGHT_TURN, 2 );
        sleep(2);
        //return to the CP
        follow_left_wall_until_end( 220, WALL_FOLLOW_TARGET );
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
        follow_right_wall_until_left_open( 220, WALL_FOLLOW_TARGET );
        //get past the break, should now be able to read the left wall
        drive( 35, 3 );
        sleep(3);
        //navigate to the second left break
        follow_right_wall_until_left_open( 220, WALL_FOLLOW_TARGET );
        //turn 90 degrees to the left
        turn( FULL_LEFT_TURN, 2 );
        sleep(2);
        //drive forward to the edge of the green area
        drive( 51, 8 );
        sleep(8);
        //turn at a 45 degree angle to get past the blocks
        turn( HALF_RIGHT_TURN, 1 );
        sleep(1);
        //drive towards the wall.
        drive( 43.2, 6 );
        sleep(6);
        //turn to be parallel with the wall
        turn( HALF_LEFT_TURN, 1 );
        sleep(1);
        //wall follow until the corner is reached
        follow_right_wall_until_obstacle( 220, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn( FULL_LEFT_TURN, 2 );
        sleep(2);
        //wall follow until the corner is reached
        follow_right_wall_until_obstacle( 220, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn( FULL_LEFT_TURN, 2 );
        //drive to the victim
        drive( 17.8, 2 );
        sleep(2);
        //grab and lift the victim
        claw( CLOSE );
        claw( RAISE );
        //turn around
        turn( LEFT_180, 4 );
        sleep(4);
        //wall follow to the corner again
        follow_left_wall_until_obstacle( 220, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn( FULL_RIGHT_TURN, 2 );
        sleep(2);
        //wall follow until the corner is reached
        follow_left_wall_until_obstacle( 220, WALL_FOLLOW_TARGET, 10.0 );
        //turn 90 degrees at the corner
        turn( FULL_RIGHT_TURN, 2 );
        sleep(2);
        //drive back to the point where the obstacles were avoided
        drive( 94, 7);
        //make a 45 degree turn right
        turn( HALF_RIGHT_TURN, 1 );
        //drive off of the green area
        drive( 43.2, 6 );
        sleep(6);
        //turn left 45 degrees
        turn( HALF_LEFT_TURN, 1 );
        sleep(1);
        //drive back into the city section
        drive( 51, 8 );
        sleep(8);
        //turn right 90 degrees
        turn( FULL_RIGHT_TURN, 2 );
        sleep(2);
        //wall follow back to CP
        follow_left_wall_until_end( 220, WALL_FOLLOW_TARGET );
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
