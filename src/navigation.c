#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/sensors.h"
#include "../include/logger.h"


#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

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
    drive( -1, 1 );
    sleep(1);

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

    claw( LOWER );
    drive( 18, 2 );
    sleep(2);

    follow_right_wall_until_obstacle( 220, 5.5, 2.0 );
    
    victim_color = YELLOW;

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
    stop();
}

void retrieve_victim_2()
{
    //grabs victim 2 and returns to the CP
    printf("function: %s\n", __func__);

    follow_left_wall_until_end( 220, 8.0 );

    drive( 17, 2 );
    sleep(2);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 31, 4 );
    sleep(4);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 10, 2 );
    claw( LOWER );
    claw( OPEN );
    sleep(2);

    follow_left_wall_until_obstacle( 220, 7.0, 2.0 );

    victim_color = YELLOW;

    claw( CLOSE );
    usleep( 500*1000 );
    claw( RAISE );

    turn( RIGHT_180, 4 );
    sleep(4);

    follow_right_wall_until_end( 220, 5.0 );

    drive( 16, 2 );
    sleep(2);

    turn( FULL_RIGHT_TURN, 2 );
    claw( LOWER );
    sleep(2);

    forward_until_obstacle( 220, 2.0 );

    turn( FULL_RIGHT_TURN, 2 );
    claw( RAISE );
    sleep(2);

    follow_left_wall_until_end( 220, 7.5 );
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
    stop();
}

void retrieve_victim_3()
{
    //hopefully grabs victim 3 and returns to the CP
    printf("function: %s\n", __func__);
    // Start following right wall until Left gap

//    follow_right_wall_until_left_open( 180, 6.0 );

    // drive across the gap
    drive( 50, 4 );
    sleep(4);

    // Start following right wall until Left gap
    follow_right_wall_until_left_open( 220, 6.0 );

    // 90 degrees left
    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    // Drive forward towards grass
    drive( 40, 4 );
    sleep(4);

    // 45 degrees to the right
    turn(HALF_RIGHT_TURN, 2);
    sleep(2);

    // Drive forward 
    drive( 40, 3 );
    sleep(3);

    // 45 degrees to the left
    turn(HALF_LEFT_TURN, 2);
    sleep(2);

    // Push victim around
    claw( OPEN );
    claw( LOWER );

    //check for color


    drive(54, 4);
    sleep(4);

    victim_color = YELLOW;


    claw(CLOSE);
    
    forward_until_obstacle( 220, 10.0 );
    drive(-1, 1);
    sleep(1);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    claw(OPEN);


    drive(48, 4);
    sleep(4);

    claw(CLOSE);

    // Running to the end of the map
    turn( LEFT_180, 2 );
    sleep(2);
    

    forward_until_obstacle( 220, 10.0 );
    drive(-1, 1);
    sleep(1);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    forward_until_obstacle( 220, 10.0 );
    drive(-1, 1);
    sleep(1);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    drive(30, 3);
    sleep(3);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    forward_until_obstacle( 220, 10.0 );
    drive(-1, 1);
    sleep(1);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    follow_left_wall_until_end( 220, 6.0 );

    cp_to_yellow();
    
    stop();
}

void retrieve_victim_4()
{
    //definitely grabs victim 4 and returns the poor bastard to the CP
    printf("function: %s\n", __func__);

    //V4b
    follow_left_wall_until_end( 220, 8.0 );
    drive( 18, 2 );
    sleep(2);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 62, 4 );
    sleep(4);

    turn( FULL_LEFT_TURN, 2 );
    sleep(2);

    drive( 17, 1 );
    sleep(1);

    claw(OPEN);
    claw(LOWER);
    follow_left_wall_until_obstacle( 200, 8.0, 2.0 );

    claw(CLOSE);
    victim_color = YELLOW;
    
    usleep(500*1000);
    claw( RAISE );

    turn( RIGHT_180, 4 );
    sleep(4);

    follow_right_wall_until_end( 220, 7.0 );

    drive( 17.0, 2 );
    sleep(2);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    forward_until_obstacle( 220, 10.0 );
    drive(-1, 1);
    sleep(1);

    turn( FULL_RIGHT_TURN, 2 );
    sleep(2);

    follow_left_wall_until_end( 220, 7.5 );
    stop();
    

    cp_to_yellow();
    stop();


}
