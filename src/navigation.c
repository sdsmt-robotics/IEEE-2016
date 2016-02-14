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


/*
* This function will run functions necessary for the current state.
*/
void runState(int state)
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
*This function will handle getting the robot to the reference point.
*/
void initialize()
{
    /*
    *update distances, determine if the path is straight ahead or if the robot
    *needs to turn.
    */
    update();
    /*logic to determine which way the robot is facing when it starts.*/

    /*once the robot is facing out:*/
    moveForward(startDistance);
    turnLeft(90degrees);
    moveForwardUntilWall();
    turnRight(90degrees);
    /*
    *Robot should now be in the reference position facing away from the yellow
    *hospital.
    */
}

/*
*This function will handle retrieving the first person in the city, determining
*their color, and returning them to the appropriate hospital.
*/
void retrievePerson1()
{
    /*This function should handle getting the robot to the person*/
    /*TODO: Rewrite this to handle things without dead reckoning*/
    /*TODO: This section will change depending on how we poll the sensors.*/
    moveForward(firstPersonDistance/*approx 6ft*/);
    approachPerson();
    determineColor();
    grabPerson();
    turnRight(180degrees); /*Turn the robot around*/
    if(color == yellow)
    {
        /*Rewrite to use return to reference point.*/
        moveForwardUntilWall(); /*it shouldn't be necessary to know distance*/
        dropOffPerson();
        /*this function can use current location to determine how to get back.*/
        returnToReferencePoint(yellowHospital);
    }
    else
    {
        /*get back to the reference point from where the first person is*/
        returnToReferencePoint(firstPerson);
        turnLeft(90degrees);
        moveForwardUntilWall();
        turnLeft(90degrees);
        moveForwardUntilWall();
        dropOffPerson();
        returnToReferencePoint(redHospital);
    }
}

/*
*This function will handle getting the second city person.
*/
void retrievePerson2()
{
    /*This part is a bit dicey and needs to be tested before revisions are made*/
    while(distanceLeft <= wallFollowDistance)
    {
        moveForward(); /*ideally works with while loop in this form*/
    }
    if(distanceLeft > wallFollowDistance && distanceRight <= wallFollowDistance
       && distanceCenter > wallFollowDistance)
    {
       turnLeft(90degrees); /*the robot should be facing the grass area.*/
    }
    if (distanceLeft > wallFollowDistance
        && distanceRight > wallFollowDistance
        && distanceCenter > wallFollowDistance)
    {
        moveForward(thirdRowDistance/*~1-2 feet*/);
        turnLeft(90degrees);
    }
    moveForward(secondPersonDistance/*less than 3ft*/);
    approachPerson();
    determineColor();
    grabPerson();
    turnRight(180degrees);
    /*consider placing this code in the returnToReferencePoint function*/
    /*
    while(distanceLeft <= wallFollowDistance
          && distanceRight <= wallFollowDistance)
    {
        moveForward();
    }
    turnRight(90degrees);
    moveForwardUntilWall();
    turnRight(90degrees);
    moveForwardUntilWall();
    */
    returnToReferencePoint(secondPerson);
    if (color == yellow)
    {
        /*
        *assuming that the returnToReferencePoint function sets the robot
        *facing away from yellowHospital
        */
        turnRight(180degrees);
        moveForwardUntilWall();
        dropOffPerson();
        returnToReferencePoint(yellowHospital);
    }
    else
    {
        turnRight(90degrees);
        moveForwardUntilWall();
        turnLeft(90degrees);
        moveForwardUntilWall();
        dropOffPerson();
        returnToReferencePoint(redHospital);
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
