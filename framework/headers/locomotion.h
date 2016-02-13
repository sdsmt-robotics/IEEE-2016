#ifndef __LOCOMOTION_DEFINES_
#define __LOCOMOTION_DEFINES_

/*Arduino locomotion wrapper*/
typedef side int;

side LEFT = 1;
side RIGHT = 0;

void wheelSpeed( side wheel, int speed );

#endif
