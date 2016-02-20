#ifndef __LOCOMOTION_DEFINES_
#define __LOCOMOTION_DEFINES_

#include "robot_defines.h"

/*Arduino locomotion wrapper*/


/*
 * 127 is the center point. rate > 127 is forward. rate < 127 is reverse
 */
void setWheelSpeed( int wheel, unsigned char speed, int serial_port );
void driveWheelSteps( int wheel, int steps, unsigned char speed, int serial_port );
void turn( int serial_port, unsigned char speed, float angle ); //angle from x axis
void drive( int serial_port, float distance, unsigned char speed );


#endif
