#ifndef __LOCOMOTION_DEFINES_
#define __LOCOMOTION_DEFINES_

#include "robot_defines.h"

/*Arduino locomotion wrapper*/

//`step` means microstep

/*
 * 127 is the center point. rate > 127 is forward. rate < 127 is reverse
 */

// time in seconds
// STEPS_PER_CM is defined in robot_defines.h

//void setWheelSpeed( int wheel, unsigned char speed, int serial_port );
void driveWheelSteps( int wheel, int steps, int time, int serial_port );
// negative steps means driving backwards

void turn( int serial_port, int angle, int time );
void drive( int serial_port, float distance, int time );
void stop( int serial_port );
//negative distance implies moving backwards

void close_claws( int serial_port );
void raise_claws( int serial_port );



#endif
