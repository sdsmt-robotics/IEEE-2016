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

#include <stdbool.h>


void start_to_cp( int serial_port )
{
	int steps;
	int time;



/* ********** BEGIN FORWARD TO EDGE OF WALL ********** */
	while (Left_IR() < SIX_INCHES)
	{
		// Fix time
		time = 1;
		steps = 10;
		// We need to move left
		if (Left_IR() > ZERO + TOLERANCE)
		{
			
			driveWheelSteps(RIGHT, steps*2, time, int serial_port );
			driveWheelSteps(BOTH, steps, time, int serial_port );
			driveWheelSteps(LEFT, steps*2, time, int serial_port );
		}
		else if (Left_IR() < ZERO - TOLERANCE)
		{
			driveWheelSteps(LEFT, steps*2, time, int serial_port );
			driveWheelSteps(BOTH, steps, time, int serial_port );
			driveWheelSteps(RIGHT, steps*2, time, int serial_port );
		}
		else
		{
			driveWheelSteps(BOTH, steps, time, int serial_port );
		}
	}
/* ********** END FORWARD TO EDGE OF WALL ********** */



/* ********** BEGIN FORWARD 6 INCHES ********** */
	// 396 steps ~ 6 inches
	driveWheelSteps(BOTH, 396, time, int serial_port );
/* ********** END FORWARD 6 INCHES ********** */



/* ********** BEGIN LEFT 90 DEGREE ROTATION ********** */

	steps = 200;
	driveWheelSteps(LEFT, -steps, time, int serial_port );
	driveWheelSteps(RIGHT, steps, time, int serial_port );

/* ********** END LEFT 90 DEGREE ROTATION ********** */



/* ********** BEGIN FORWARD UNTIL 6 INCHES FROM WALL ********** */
	steps = 10;
	while (FORWARD_IR() > SIX_INCHES)
	{
		driveWheelSteps(BOTH, steps, time, int serial_port );
	}
/* ********** END FORWARD UNTIL 6 INCHES FROM WALL ********** */



/* ********** BEGIN RIGHT 90 DEGREE ROTATION ********** */

	steps = 200;
	driveWheelSteps(LEFT, steps, time, int serial_port );
	driveWheelSteps(RIGHT,-steps, time, int serial_port );


/* ********** END RIGHT 90 DEGREE ROTATION ********** */

// We are now in CP
}

void get_to_cp( int serial_port )
{
    //stuff here
}

void cp_to_start( int serial_port )
{
    //rotate 90 deg
    //forward 30 cm
    //rotate 90 deg
    //possibly open claws
    //forward 31 cm
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

