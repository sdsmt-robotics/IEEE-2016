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

void setWheelSpeed( int wheel, unsigned char speed, int serial_port );
void driveWheelSteps( int wheel, int steps, float time, int serial_port );
// negative steps means driving backwards

void turn( int serial_port, float angle, float time );
void drive( int serial_port, float distance, float time );
//negative distance implies moving backwards





#endif
