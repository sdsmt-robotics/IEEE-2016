#ifndef __CONTROLLERMAP_H
#define __CONTROLLER_H


// Input type definitions: stick, pad, and 
// trigger are 2; buttons are 1 
#define TYPE_STICK   2
#define TYPE_DPAD    2
#define TYPE_TRIGGER 2
#define TYPE_BUTTON  1

#define TYPE_NOT_BUTTON 2

// "Axis" definitions. Down (trigger pulled in, stick 
// down) and right are positive values
#define AXIS_LEFT_STICK_VERTICAL    1
#define AXIS_LEFT_STICK_HORIZONTAL  0
#define AXIS_RIGHT_STICK_VERTICAL   4
#define AXIS_RIGHT_STICK_HORIZONTAL 3

#define AXIS_LEFT_TRIGGER           2
#define AXIS_RIGHT_TRIGGER          5

#define AXIS_COUNT 6

//  Button definitions. Down is 1, up is 0
#define BUTTON_A            0
#define BUTTON_B            1
#define BUTTON_X            2
#define BUTTON_Y            3
#define BUTTON_BACK         6
#define BUTTON_START        7
#define BUTTON_XBOX         8
#define BUTTON_LEFT_BUMPER  4
#define BUTTON_RIGHT_BUMPER 5
#define BUTTON_LEFT_CLICK   9
#define BUTTON_RIGHT_CLICK  10

#define BUTTON_COUNT 11

#define DEADZONE 20

#endif
