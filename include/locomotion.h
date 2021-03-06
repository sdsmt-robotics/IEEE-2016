#ifndef __LOCOMOTION_DEFINES_
#define __LOCOMOTION_DEFINES_

//negative steps implies driving backwards
//negative distance implies driving backwards
void setWheelSpeed( int wheel, unsigned char speed );
void driveWheelSteps( int wheel, int steps, int runtime );

void turn( int angle, int runtime );
void var_turn( int angle, int runtime );
void drive( float distance, int runtime );
void stop();

void claw( int state );

void forward_until_left_end( unsigned char speed );
void forward_until_right_end( unsigned char speed );
void forward_until_obstacle( unsigned char speed, float tolerance );

void follow_left_wall_until_end( unsigned char speed, float target );
void follow_left_wall_until_obstacle( unsigned char speed, float target, float tolerance );

//For testing
void test_follow_left_wall_until_end( unsigned char speed, float target );
void var_test_follow_left_wall_until_end( unsigned char speed, float target );

void follow_right_wall_until_end( unsigned char speed, float target );
void follow_right_wall_until_obstacle( unsigned char speed, float target, float tolerance );
void follow_right_wall_until_left_open( unsigned char speed, float target );
void follow_right_wall_until_VS( unsigned char speed, float target );

void getVictim();

#endif
