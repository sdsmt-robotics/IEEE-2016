#ifndef __ROBOT_DEFINES_
#define __ROBOT_DEFINES_

#define ROBOT_NAME "Anne Droid"

// right/left defines. Doesn't matter what these values are as long as they're unique
#define LEFT 0
#define RIGHT 1
#define BOTH 2
#define FRONT 3
#define BACK 4
#define VIC 5

#define YELLOW 1
#define RED 2
#define UNKNOWN_COLOR 3

// motor defines
#define LEFT_MOTOR_FLAG        0x4C
#define RIGHT_MOTOR_FLAG       0x52

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
#define RIGHT_180 180
#define LEFT_180 -180

//communication defines
#define ROBOT_BAUDRATE 115200
#define ARDUINO_COMM_LOCATION "/dev/robot/arduino"
#define SENSORS_COMM_LOCATION "/dev/robot/sensors"
#define ARDUINO_RECEIVED_BYTE 0x13

#define SENSOR_REQUEST       0x60
#define LEFT_SENSOR_REQUEST  0x61
#define RIGHT_SENSOR_REQUEST 0x62
#define FRONT_SENSOR_REQUEST 0x63
#define BACK_SENSOR_REQUEST  0x64
#define VIC_SENSOR_REQUEST   0x65

#define IR_RIGHT_TAG         0x71
#define IR_FRONT_TAG         0x72
#define IR_LEFT_TAG          0x73
#define IR_PACKET_END        0x74

#define SENSOR_PROC_DELAY_US 50000

//For blue, black paint, to make our voltage mapping function work
#define IR_DISTANCE_SCALAR 0.5

//Physical information about the robot
#define WHEEL_BASE_MM 153.0

//Blue Wheels with wimpy-ass steppers:
//#define STEPS_PER_CM 26
//#define STEPS_PER_MM 2.6

//Blue Wheels with geared steppers:
#define STEPS_PER_CM 132.0
#define STEPS_PER_MM 13.2

//Green Wheels with wimpy-ass steppers:
//#define STEPS_PER_CM 35
//#define STEPS_PER_MM 3.5

//Green Wheels with gears steppers:
//#define STEPS_PER_CM 182
//#define STEPS_PER_MM 18.2

//Sensor Stuff
#define INF_DISTANCE 15.0
#define SIX_INCHES 15.0

#define VIC_DISTANCE 4.0 	// UNKNOWN, will be based on where the sensor is
//Stopping tolerance depends on paint color
#define FRONT_STOPPING_TOLERANCE 0

//want to be 6 cm away from wall
#define WALL_FOLLOW_DELTA 0.5
#define WALL_FOLLOW_TARGET 6.0
#define WALL_FOLLOW_TOLERANCE 0.3

//Timing delays
#define TEN_MS 10000
#define TWENTY_MS 20000



//global serial port variables so we can avoid hours of pain when we forget
//to pass this to a function and wonder wtf is happening.
//Also, why does `gcc -Wall` compile function calls that don't have the
//correct number of arguments? Seriouslly, wtf?
extern int send_port;
extern int receive_port;
extern int victim_color;

#endif
