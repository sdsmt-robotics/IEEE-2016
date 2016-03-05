#ifndef __ROBOT_DEFINES_
#define __ROBOT_DEFINES_

//right/left defines
#define BOTH 2
#define RIGHT 1
#define LEFT 0

#define TRUE 1
#define FALSE 0

// motor defines
#define LEFT_MOTOR_FLAG  0x4C
#define RIGHT_MOTOR_FLAG 0x52

 // Turn angle defines
 #define FULL_RIGHT_TURN 90
 #define FULL_LEFT_TURN -90
 #define HALF_RIGHT_TURN 45
 #define HALF_LEFT_TURN -45
 #define QUARTER_RIGHT_TURN 22.5
 #define QUARTER_LEFT_TURN -22.5


//communication defines
#define ROBOT_BAUDRATE 115200
#define ARDUINO_COMM_LOCATION "/dev/robot/arduino"
#define ARDUINO_RECEIVED_BYTE 0x13

//Physical information
#define WHEEL_BASE_MM 158.2
#define STEPS_PER_CM 26
#define STEPS_PER_MM 2.6

#endif
