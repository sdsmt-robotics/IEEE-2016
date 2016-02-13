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
#include "navigation.h"


/*
* This function will make final decisions about how to move.
*/
void navigate()
{
    /*
    *SpecialMovementLogic indicates that it is ok to be near a wall/object
    *such as when grabbing a person.
    */
    /*if (specialMovementLogic == FALSE)
    {
        if (distanceLeft <= minDist)
        {
            moveAwayFromLeft();
        }
        else if ( distanceRight <= minDist)
        {
            moveAwayFromRight();
        }
        else if (distanceCenter <= minDist)
        {
            moveAwayFromFront();
        }
    }
    else if (grabbingPerson == TRUE)
    {
        if (personInRange == TRUE)
        {
            grabPerson();
            ReturnToBase == TRUE;
        }
        else
        {
            moveToPerson();
        }
    }
    else if (ReturnToBase == TRUE)
    {
        ReturnToBaseMovementLogic();
    }
    else
    {
        HandleOtherSpecialMovement();
    }*/
}

/*
* This function will update the current distances with the sensor input.
*/
void update (int sensor1, int sensor2, int sensor2)
{
    /*These variables are placeholders*/
    int distanceLeft = sensorInputToDistance(sensor1);
    int distanceRight = sensorInputToDistance(sensor2);
    int distanceCenter = sensorInputToDistance(sensor3);
}

/*
* This function will turn the value that the IR sensors provide into a usable
* distance.
*/
int sensorInputToDistance(int input)
{
    /*TODO: determine what the numeric sensor output means as a distance*/
}