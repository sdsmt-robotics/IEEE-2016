#ifndef SENSORS_H
#define SENSORS_H

//returns distance in cm
double left_sensor();
double right_sensor();
double front_sensor();
double back_sensor();

double map_voltage_to_distance( int voltage );

void temporary_sensor_request();
int poll_sensors( int sensor );

//return average voltage, in units that are really hard to work with.
//0 to 1023 "steps" to represent 0 to 5 volts. That corresponds to
//4.9 mV/"step", or 205 "steps" per volt.
int poll_left_sensor();
int poll_right_sensor();
int poll_front_sensor();
int poll_back_sensor();

#endif
