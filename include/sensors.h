#ifndef SENSORS_H
#define SENSORS_H

//returns distance in cm
double left_sensor();
double right_sensor();
double front_sensor();
double back_sensor();
double vic_sensor();

//return average voltage, in units that are really hard to work with.
//0 to 1023 "steps" to represent 0 to 5 volts. That corresponds to
//4.9 mV per "step", or 205 "steps" per volt.
double map_voltage_to_distance( int voltage );

void poll_sensors();
void sensors( double *vic, double *back, double *front, double *left, double *right );

//returns an int "voltage"
int poll_left_sensor();
int poll_right_sensor();
int poll_front_sensor();
int poll_back_sensor();
int poll_vic_sensor();

//This wille eventually move to a camera code file
void SetVictimLocation();

#endif
