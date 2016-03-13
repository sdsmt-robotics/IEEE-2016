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

void follow_left_wall_until_end( unsigned char speed );
void follow_right_wall_until_end( unsigned char speed );
void follow_left_wall_until_obstacle( unsigned char speed );
void follow_right_wall_until_obstacle( unsigned char speed );

void start_to_cp();
void get_to_cp();
void cp_to_start();

bool retreive_victim_1();
bool retreive_victim_2();
bool retreive_victim_3();
bool retreive_victim_4();


#endif
