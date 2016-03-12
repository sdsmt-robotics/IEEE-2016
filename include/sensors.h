#ifndef SENSORS_H
#define SENSORS_H

//return approximate distance
double left_sensor();
double right_sensor();
double front_sensor();
double back_sensor();

double map_voltage_to_distance( int voltage );

void temporary_sensor_request();
int poll_sensors( int sensor );

//return average voltage from Arduino
int poll_left_sensor();
int poll_right_sensor();
int poll_front_sensor();
int poll_back_sensor();

#endif