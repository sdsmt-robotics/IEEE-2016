#ifndef SENSORS_H
#define SENSORS_H

int Left_IR();
int Right_IR();
int Forward_IR();
int Front_Right_IR();
int Front_Left_IR();
int Backward_IR();

void temporary_sensor_request( int serial_port );
int extract_sensor_data( int serial_port, int sensor );

#endif