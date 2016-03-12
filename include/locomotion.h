#ifndef __LOCOMOTION_DEFINES_
#define __LOCOMOTION_DEFINES_

#include "robot_defines.h"

void setWheelSpeed( int wheel, unsigned char speed, int serial_port );
void driveWheelSteps( int wheel, int steps, int runtime, int serial_port );
// negative steps means driving backwards
//negative distance implies moving backwards

void turn( int serial_port, int angle, int runtime );
void drive( int serial_port, float distance, int runtime );
void stop( int serial_port );


void claw( int serial_port, int state );

void var_turn( int serial_port, int angle, int runtime );




#endif
