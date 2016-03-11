#ifndef __ROBOT_DEFINES_
#define __ROBOT_DEFINES_

//right/left defines
#define BOTH 2
#define RIGHT 1
#define LEFT 0
#define FRONT 3
#define BACK 4

// motor defines
#define LEFT_MOTOR_FLAG  0x4C
#define RIGHT_MOTOR_FLAG 0x52

#define LEFT_MOTOR_STEPS_FLAG  0x39
#define RIGHT_MOTOR_STEPS_FLAG 0x40
#define SERVO_CLAW_CLOSE_TAG   0x41
#define SERVO_CLAW_RAISE_TAG   0x42

//claw states. Doesn't really matter what these values are as long as they're unique
#define OPEN 1
#define CLOSE 2
#define RAISE 3
#define LOWER 4

//claw opn/close/raise/lower values to send to arduino
#define OPEN_VAL 0
#define CLOSE_VAL 255
#define RAISE_VAL 255
#define LOWER_VAL 0

// Turn angle defines
#define FULL_RIGHT_TURN 90
#define FULL_LEFT_TURN -90
#define HALF_RIGHT_TURN 45
#define HALF_LEFT_TURN -45
//Possibility of needing to truncate the decimal point on these
#define QUARTER_RIGHT_TURN 22.5
//Possibility of needing to truncate the decimal point on these
#define QUARTER_LEFT_TURN -22.5
#define RIGHT_180 180
#define LEFT_180 -180


//communication defines
#define ROBOT_BAUDRATE 115200
#define ARDUINO_COMM_LOCATION "/dev/robot/arduino"
#define ARDUINO_RECEIVED_BYTE 0x13
#define SENSOR_REQUEST 0x60
#define LEFT_SENSOR_REQUEST 0x61
#define RIGHT_SENSOR_REQUEST 0x62
#define FRONT_SENSOR_REQUEST 0x63
#define BACK_SENSOR_REQUEST 0x64

#define IR_RIGHT_TAG  0x71
#define IR_FRONT_TAG 0x72
#define IR_LEFT_TAG   0x73
#define IR_PACKET_END 0x74

#define SENSOR_PROC_DELAY_US 10000

//Physical information
//#define WHEEL_BASE_MM 158.2
//with camber:
#define WHEEL_BASE_MM 149

//Blue Wheels:
//#define STEPS_PER_CM 26
//#define STEPS_PER_MM 2.6

//Green Wheels:
#define STEPS_PER_CM 35
#define STEPS_PER_MM 3.5

//Sensor Stuff
#define INF 1
#define SIX_INCHES 1
#define ZERO 0
#define TOLERANCE 1

#endif
