#ifndef SENSORS_H
#define SENSORS_H

//return approximate distance
float left_sensor();
float right_sensor();
float front_sensor();
float back_sensor();

float map_voltage_to_distance( int voltage );

void temporary_sensor_request( int serial_port );
int poll_sensors( int serial_port, int sensor );

//return average voltage from Arduino
int poll_left_sensor( int serial_port );
int poll_right_sensor( int serial_port );
int poll_front_sensor( int serial_port );
int poll_back_sensor( int serial_port );

#endif