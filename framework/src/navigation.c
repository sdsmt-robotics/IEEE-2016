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
* This function will run functions necessary for the current state.
*/
void runState()
{
    switch(state)
    {
    case 0:
        initialize();
        break;
    case 1:
        retrievePerson1();
        break;
    case 2:
        retrievePerson2();
        break;
    case 3:
        retrievePerson3();
        break;
    case 4:
        retrievePerson4();
        break;
    case 5:
        terminate();
        break;
    default:
        handleNonStandardMovement();
    }
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