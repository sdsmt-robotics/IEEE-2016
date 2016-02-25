#ifndef LOCALIZATION_H
#define LOCALIZATION_H

typedef struct Point {
	int x;
	int y;
} Point;


//Definitions of points for easy access.
//We don't necessarily need to need these. It sounds like we are going to use
//sensor input to localize, not dead-reckoning between Points. Either way, we
//can define these for convinience.
const Point START = { 1, 1 };
const Point CENTRAL_POINT = { 1, 1 };
const Point RED_HOSPITAL = { 1, 1 };
const Point YELLOW_HOSPITAL = { 1, 1 };
const Point GATE_1 = { 1, 1 };
const Point GATE_2 = { 1, 1 };
const Point CORNER = { 1, 1 }; // Corner intersect P4A and P3A (approx where the field changes from urban to rural)
const Point VICTIM_1 = { 1, 1 };
const Point VICTIM_2 = { 1, 1 };
const Point VICTIM_3A = { 1, 1 };
const Point VICTIM_3B = { 1, 1 };
const Point VICTIM_4A = { 1, 1 };
const Point VICTIM_4B = { 1, 1 };

Point CURR_LOC = { 0, 0 };
int CURR_HEADING = 0;

//The direction the robot faces on startup is positive x direction
//The left side of the robot points in the positive y direction
//Initial heading is 0 degrees
//Distance sensors will set CURR_LOC on startup
#endif

