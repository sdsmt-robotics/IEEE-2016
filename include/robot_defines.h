#ifndef __ROBOT_DEFINES_
#define __ROBOT_DEFINES_

//right/left defines
#define BOTH 2
#define RIGHT 1
#define LEFT 0
#define FRONT 3

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


//communication defines
#define ROBOT_BAUDRATE 115200
#define ARDUINO_COMM_LOCATION "/dev/robot/arduino"
#define ARDUINO_RECEIVED_BYTE 0x13
#define SENSOR_REQUEST 0x37

#define IR_RIGHT_TAG  0x80
#define IR_FRONT_TAG 0x86
#define IR_LEFT_TAG   0x88
#define IR_PACKET_END 0x70

//Physical information
#define WHEEL_BASE_MM 158.2
#define STEPS_PER_CM 26
#define STEPS_PER_MM 2.6

//Sensor Stuff
#define NUM_SENSORS 3
#define INF 1
#define SIX_INCHES 1
#define ZERO 0
#define TOLERANCE 1

#endif
