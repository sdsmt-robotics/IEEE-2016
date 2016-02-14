#ifndef __NAV_DEFINES_
#define __NAV_DEFINES_

#include "robot_defines.h"

/*
*These are the state proceedures, they are executed and in turn execute the code
*needed to complete the state's goals.
*/
void runState(int state);
void initialize();
void retrievePerson1();
void retrievePerson2();

/*
*These functions will handle determining what distance the walls are from the
*robot.
*/
void update (int sensor1, int sensor2, int sensor2);
int sensorInputToDistance(int input);

#endif
