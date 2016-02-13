#ifndef __LOCOMOTION_DEFINES_
#define __LOCOMOTION_DEFINES_

/*Arduino locomotion wrapper*/

#define RIGHT 1
#define LEFT 0
/*
 * 127 is the center point. rate > 127 is forward. rate < 127 is reverse
 */
void setWheelSpeed( side wheel, unsigned char speed, int serial_port );

#endif
