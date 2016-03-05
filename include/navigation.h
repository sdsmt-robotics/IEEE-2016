#ifndef __NAV_DEFINES_
#define __NAV_DEFINES_

#include <stdbool.h>

/*GetDistanceAhead();
GetDistanceBehind();
GetDistanceLeft(); //For going straight
GetDistanceRight(); //For going straight
//GetHeading(); // From IMU
*/
/*
*These are the state proceedures, they are executed and in turn execute the code
*needed to complete the state's goals.
*/
/*
void runState(int state);
void initialize();
void retrievePerson1();
void retrievePerson2();
*/
/*
*These functions will handle determining what distance the walls are from the
*robot.
*/
/*
void update (int sensor1, int sensor2, int sensor2);
int sensorInputToDistance(int input);
*/

void start_to_cp( int serial_port );
void get_to_cp( int serial_port );
void cp_to_start( int serial_port );

bool retreive_victim_1( int serial_port );
bool retreive_victim_2( int serial_port );
bool retreive_victim_3( int serial_port );
bool retreive_victim_4( int serial_port );


#endif
