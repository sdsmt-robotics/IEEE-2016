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


void start_to_cp( int serial_port )
{
    int steps;
    int runtime;



/* ********** BEGIN FORWARD TO EDGE OF WALL ********** */
    while (Left_IR() < SIX_INCHES)
    {
        // Fix time
        time = 1;
        steps = 10;
        // We need to move left
        if (Left_IR() > ZERO + TOLERANCE)
        {
            
            driveWheelSteps(RIGHT, steps*2, runtime, serial_port );
            driveWheelSteps(BOTH, steps, runtime, serial_port );
            driveWheelSteps(LEFT, steps*2, runtime, serial_port );
        }
        else if (Left_IR() < ZERO - TOLERANCE)
        {
            driveWheelSteps(LEFT, steps*2, runtime, serial_port );
            driveWheelSteps(BOTH, steps, runtime, serial_port );
            driveWheelSteps(RIGHT, steps*2, runtime, serial_port );
        }
        else
        {
            driveWheelSteps(BOTH, steps, runtime, serial_port );
        }
    }
/* ********** END FORWARD TO EDGE OF WALL ********** */



/* ********** BEGIN FORWARD 6 INCHES ********** */
    // 396 steps ~ 6 inches
    driveWheelSteps(BOTH, 396, runtime, serial_port );
/* ********** END FORWARD 6 INCHES ********** */



/* ********** BEGIN LEFT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, -steps, runtime, serial_port );
    driveWheelSteps(RIGHT, steps, runtime, serial_port );

/* ********** END LEFT 90 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD UNTIL 6 INCHES FROM WALL ********** */
    steps = 10;
    while (FORWARD_IR() > SIX_INCHES)
    {
        driveWheelSteps(BOTH, steps, runtime, serial_port );
    }
/* ********** END FORWARD UNTIL 6 INCHES FROM WALL ********** */



/* ********** BEGIN RIGHT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, steps, runtime, serial_port );
    driveWheelSteps(RIGHT,-steps, runtime, serial_port );


/* ********** END RIGHT 90 DEGREE ROTATION ********** */

// We are now in CP
}

void get_to_cp( int serial_port )
{
    //stuff here
}

void cp_to_start( int serial_port )
{
    int steps;
    int runtime = 1;

/* ********** BEGIN RIGHT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, steps, runtime, serial_port );
    driveWheelSteps(RIGHT,-steps, runtime, serial_port );


/* ********** END RIGHT 90 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD UNTIL 6 INCHES FROM WALL ********** */
    steps = 10;
    while (FORWARD_IR() > SIX_INCHES)
    {
        driveWheelSteps(BOTH, steps, runtime, serial_port );
    }
/* ********** END FORWARD UNTIL 6 INCHES FROM WALL ********** */



/* ********** BEGIN LEFT 90 DEGREE ROTATION ********** */

    steps = 200;
    driveWheelSteps(LEFT, -steps, runtime, serial_port );
    driveWheelSteps(RIGHT, steps, runtime, serial_port );

/* ********** END LEFT 90 DEGREE ROTATION ********** */



/* ********** BEGIN REVERSE TO 4cm FROM WALL ********** */

    
    // Reverse 6.5 inches
    driveWheelSteps(BOTH, -430, runtime, serial_port );

    steps = 10;
    // While rear sensor reads further than 4cm
    // Difference between this and left edge detect is the Backward_IR() and TOLERANCE
    while (Backward_IR() > 4)
    {
        // Fix time
        time = 1;
        // We need to move left
        if (Left_IR() > ZERO + TOLERANCE)
        {
            
            driveWheelSteps(RIGHT, steps*2, runtime, serial_port );
            driveWheelSteps(BOTH, steps, runtime, serial_port );
            driveWheelSteps(LEFT, steps*2, runtime, serial_port );
        }
        else if (Left_IR() < ZERO - TOLERANCE)
        {
            driveWheelSteps(LEFT, steps*2, runtime, serial_port );
            driveWheelSteps(BOTH, steps, runtime, serial_port );
            driveWheelSteps(RIGHT, steps*2, runtime, serial_port );
        }
        else
        {
            driveWheelSteps(BOTH, steps, runtime, serial_port );
        }
    }
    
/* ********** END REVERSE TO 4cm FROM WALL ********** */



/* ********** BEGIN CLAW OPEN & LOWER ********** */
    claw(serial_port, OPEN);
    claw(serial_port, LOWER);
/* ********** END CLAW OPEN & LOWER ********** */



    // We are now in the start location
}

void cp_to_red( int serial_port )
{
    //rotate 90
    //forawrd 30 cm
    //rotate -90
    //forward 183.5 cm
    //open claws
    //reverse 183.5
    //rotate -90
    //forward 30 cm
    //rotate 90
}

void cp_to_yellow( int serial_port )
{
    //rotate 180
    //forward 15 cm
    //open claws
    //reverse 15 cm
    //rotate 180

/* ********** BEGIN RIGHT 180 DEGREE ROTATION ********** */

    steps = 400;
    driveWheelSteps(LEFT, steps, runtime, serial_port );
    driveWheelSteps(RIGHT, -steps, runtime, serial_port );

/* ********** END RIGHT 180 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD 12 INCHES ********** */
    // 791 steps ~ 12 inches
    driveWheelSteps(BOTH, 791, runtime, serial_port );
/* ********** END FORWARD 12 INCHES ********** */



/* ********** BEGIN DROPOFF VICTIM ********** */
    claw(serial_port, LOWER);
    claw(serial_port, OPEN);
    claw(serial_port, RAISE);
    // Note: May have to reverse first!
    claw(serial_port, CLOSE);
/* ********** END DROPOFF VICTIM ********** */



/* ********** BEGIN REVERSE 12 INCHES ********** */
    // 791 steps ~ 12 inches
    driveWheelSteps(BOTH, -791, runtime, serial_port );
/* ********** END REVERSE 12 INCHES ********** */



/* ********** BEGIN LEFT 180 DEGREE ROTATION ********** */

    steps = 400;
    driveWheelSteps(LEFT, -steps, runtime, serial_port );
    driveWheelSteps(RIGHT, steps, runtime, serial_port );

/* ********** END LEFT 180 DEGREE ROTATION ********** */

}


bool retreive_victim_1( int serial_port )
{
    //open claws
    //forward 183.5cm
    
    //grab victim

    //reverse 183.5 cm
    //call appropriate cp_to_hospital() (Peeks at glocal color variable, or status struct)
    //return to cp
}

bool retreive_victim_2( int serial_port )
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
}

bool retreive_victim_3( int serial_port )
{
    //forward 24 in
    //rotate -90
    //forward 12.5 in
    //rotate 90
    //forward 25 in
    //rotate -90
    //
}

bool retreive_victim_4( int serial_port )
{

}