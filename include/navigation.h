#ifndef __NAV_DEFINES_
#define __NAV_DEFINES_

#include <stdbool.h>

void forward_until_left_end( unsigned char speed );
void forward_until_right_end( unsigned char speed );
void forward_until_obstacle( unsigned char speed, int tolerance );

void follow_left_wall_until_end( unsigned char speed, int target );
void follow_right_wall_until_end( unsigned char speed, int target );
void follow_left_wall_until_obstacle( unsigned char speed, int target, int tolerance );
void follow_right_wall_until_obstacle( unsigned char speed, int target, int tolerance );

void start_to_cp();
void get_to_cp();
void cp_to_start();

bool pick_up_victim_1();
bool drop_off_victim_1();
bool retreive_victim_2();
bool retreive_victim_3();
bool retreive_victim_4();


#endif
