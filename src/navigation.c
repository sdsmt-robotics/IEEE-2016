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

void follow_left_wall_until_end()
{
    setWheelSpeed( BOTH, 190 );
    int left_value = 0;
    while ( left_value < INF_DISTANCE )
    {
        left_value = left_sensor();
        if ( left_value > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( RIGHT, 195 );
            //setWheelSpeed( LEFT, 180 );
        }
        else if ( left_value < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            //setWheelSpeed( LEFT, 200 );
            setWheelSpeed( RIGHT, 185 );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, 190 );
        }
        usleep( 10*1000 ); // 10 mS
    }
    stop();
}

void follow_right_wall_until_end()
{
    setWheelSpeed( BOTH, 190 );
    int right_value = 0;
    while ( right_value < INF_DISTANCE )
    {
        right_value = right_sensor(); // has 10 mS delay inside
        if ( right_value > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            printf("Too far away from wall.\n");
            setWheelSpeed( LEFT, 195 );
            //setWheelSpeed( RIGHT, 180 );
        }
        else if ( right_value < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            printf("Too close to wall.\n");
            //setWheelSpeed( RIGHT, 200 );
            setWheelSpeed( LEFT, 185 );
        }
        else
        {
            printf("Goldilocks zone.\n");
            setWheelSpeed( BOTH, 190 );
        }
        usleep( 20*1000 ); // 10 mS
    }
    stop();
}

void follow_left_wall_until_obstacle()
{

}

void follow_right_wall_until_obstacle()
{

}

void start_to_cp( )
{
/* ********** BEGIN FORWARD TO EDGE OF WALL ********** */
    //wall follow until left sensor reading goes high
    


// /* ********** BEGIN FORWARD 6 INCHES ********** */
//     drive( , 15.24, 2 ); // drive forward 15.24 cm in 2 seconds
// /* ********** END FORWARD 6 INCHES ********** */



// /* ********** BEGIN LEFT 90 DEGREE ROTATION ********** */

//     turn( , FULL_LEFT_TURN, 2 ); // 2 seconds

// /* ********** END LEFT 90 DEGREE ROTATION ********** */



//  ********** BEGIN FORWARD UNTIL 6 INCHES FROM WALL ********** 
//     steps = 10;
//     while (front_sensor() > SIX_INCHES)
//     {
//         driveWheelSteps(BOTH, steps, runtime,  );
//         usleep(1000*runtime);
//     }
// /* ********** END FORWARD UNTIL 6 INCHES FROM WALL ********** */



// /* ********** BEGIN RIGHT 90 DEGREE ROTATION ********** */

//     turn( , FULL_RIGHT_TURN, 2000 ); // 2 seconds


// /* ********** END RIGHT 90 DEGREE ROTATION ********** */

// We are now in CP
}






void cp_to_start()
{
    int steps;
    int runtime = 1;

/* ********** BEGIN RIGHT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, steps, runtime  );
    driveWheelSteps(RIGHT,-steps, runtime  );

/* ********** END RIGHT 90 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD UNTIL 6 INCHES FROM WALL ********** */
    steps = 10;
    while (front_sensor() > SIX_INCHES)
    {
        driveWheelSteps(BOTH, steps, runtime  );
    }
/* ********** END FORWARD UNTIL 6 INCHES FROM WALL ********** */



/* ********** BEGIN LEFT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, -steps, runtime );
    driveWheelSteps(RIGHT, steps, runtime );

/* ********** END LEFT 90 DEGREE ROTATION ********** */



/* ********** BEGIN REVERSE TO 4cm FROM WALL ********** */

    
    // Reverse 6.5 inches
    driveWheelSteps(BOTH, -430, runtime  );

    steps = 10;
    // While rear sensor reads further than 4cm
    // Difference between this and left edge detect is the Backward_IR() and WALL_FOLLOW_TOLERANCE
    while (back_sensor() > 4)
    {
        // We need to move left
        if (left_sensor() > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            
            driveWheelSteps(RIGHT, steps*2, runtime  );
            driveWheelSteps(BOTH, steps, runtime  );
            driveWheelSteps(LEFT, steps*2, runtime  );
        }
        else if (left_sensor() < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            driveWheelSteps(LEFT, steps*2, runtime  );
            driveWheelSteps(BOTH, steps, runtime  );
            driveWheelSteps(RIGHT, steps*2, runtime  );
        }
        else
        {
            driveWheelSteps(BOTH, steps, runtime  );
        }
    }
    
/* ********** END REVERSE TO 4cm FROM WALL ********** */



/* ********** BEGIN CLAW OPEN & LOWER ********** */
    claw( OPEN);
    claw( LOWER);
/* ********** END CLAW OPEN & LOWER ********** */

    // We are now in the start location
}






void cp_to_red()
{

/* ********** BEGIN RIGHT 90 DEGREE ROTATION ********** */

    int steps = 200;
    int runtime = 5;
    driveWheelSteps(LEFT, steps, runtime  );
    driveWheelSteps(RIGHT,-steps, runtime  );

/* ********** END RIGHT 90 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD UNTIL 6 INCHES FROM WALL ********** */
    steps = 10;
    while (front_sensor() > SIX_INCHES)
    {
        driveWheelSteps(BOTH, steps, runtime  );
    }
/* ********** END FORWARD UNTIL 6 INCHES FROM WALL ********** */



/* ********** BEGIN LEFT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, -steps, runtime  );
    driveWheelSteps(RIGHT, steps, runtime  );

/* ********** END LEFT 90 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD TO RED HOSPITAL ********** */

    steps = 10;
    while (front_sensor() < SIX_INCHES)
    {
        // We need to move left
        if (left_sensor() > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            
            driveWheelSteps(RIGHT, steps*2, runtime  );
            driveWheelSteps(BOTH, steps, runtime  );
            driveWheelSteps(LEFT, steps*2, runtime  );
        }
        else if (left_sensor() < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            driveWheelSteps(LEFT, steps*2, runtime  );
            driveWheelSteps(BOTH, steps, runtime  );
            driveWheelSteps(RIGHT, steps*2, runtime  );
        }
        else
        {
            driveWheelSteps(BOTH, steps, runtime  );
        }
    }
/* ********** END FORWARD TO RED HOSPITAL ********** */



/* ********** BEGIN DROPOFF VICTIM ********** */
    claw(LOWER);
    claw(OPEN);
    claw(RAISE);
    // Note: May have to reverse first!
    claw(CLOSE);
/* ********** END DROPOFF VICTIM ********** */



/* ********** BEGIN REVERSE TO EDGE OF WALL ********** */
    steps = -10;
    while (left_sensor() < SIX_INCHES)
    {
        // We need to move left
        if (left_sensor() > WALL_FOLLOW_TARGET + WALL_FOLLOW_TOLERANCE)
        {
            
            driveWheelSteps(RIGHT, steps*2, runtime  );
            driveWheelSteps(BOTH, steps, runtime  );
            driveWheelSteps(LEFT, steps*2, runtime  );
        }
        else if (left_sensor() < WALL_FOLLOW_TARGET - WALL_FOLLOW_TOLERANCE)
        {
            driveWheelSteps(LEFT, steps*2, runtime  );
            driveWheelSteps(BOTH, steps, runtime  );
            driveWheelSteps(RIGHT, steps*2, runtime  );
        }
        else
        {
            driveWheelSteps(BOTH, steps, runtime  );
        }
    }
/* ********** END REVERSE TO EDGE OF WALL ********** */



/* ********** BEGIN REVERSE 6 INCHES ********** */
    // 396 steps ~ 6 inches
    driveWheelSteps(BOTH, -396, runtime  );
/* ********** END REVERSE 6 INCHES ********** */



/* ********** BEGIN RIGHT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, steps, runtime  );
    driveWheelSteps(RIGHT,-steps, runtime  );

/* ********** END RIGHT 90 DEGREE ROTATION ********** */



/* ********** BEGIN REVERSE TO 4cm FROM WALL ********** */

    while (back_sensor() > 4)
    {
        driveWheelSteps(BOTH, steps, runtime  );
    }
    
/* ********** END REVERSE TO 4cm FROM WALL ********** */



/* ********** BEGIN LEFT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, -steps, runtime  );
    driveWheelSteps(RIGHT,steps, runtime  );

/* ********** END LEFT 90 DEGREE ROTATION ********** */

    // We are now at CP
}






void cp_to_yellow( )
{
/* ********** BEGIN RIGHT 180 DEGREE ROTATION ********** */

    int steps = 400;
    int runtime = 5;
    driveWheelSteps(LEFT, steps, runtime  );
    driveWheelSteps(RIGHT, -steps, runtime  );

/* ********** END RIGHT 180 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD 12 INCHES ********** */
    // 791 steps ~ 12 inches
    driveWheelSteps(BOTH, 791, runtime  );
/* ********** END FORWARD 12 INCHES ********** */



/* ********** BEGIN DROPOFF VICTIM ********** */
    claw(LOWER);
    claw(OPEN);
    claw(RAISE);
    // Note: May have to reverse first!
    claw(CLOSE);
/* ********** END DROPOFF VICTIM ********** */



/* ********** BEGIN REVERSE 12 INCHES ********** */
    // 791 steps ~ 12 inches
    driveWheelSteps(BOTH, -791, runtime);
/* ********** END REVERSE 12 INCHES ********** */



/* ********** BEGIN LEFT 180 DEGREE ROTATION ********** */

    steps = 400;
    driveWheelSteps(LEFT, -steps, runtime);
    driveWheelSteps(RIGHT, steps, runtime);

/* ********** END LEFT 180 DEGREE ROTATION ********** */
    // We are now at CP
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