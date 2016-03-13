#ifndef __LOCOMOTION_DEFINES_
#define __LOCOMOTION_DEFINES_

#include "robot_defines.h"

void setWheelSpeed( int wheel, unsigned char speed );
void driveWheelSteps( int wheel, int steps, int runtime );
//negative steps implies driving backwards
//negative distance implies driving backwards

void turn( int angle, int runtime );
void var_turn( int angle, int runtime );
void drive( float distance, int runtime );
void stop();

void claw( int state );



#endif
