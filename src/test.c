#include "../include/main.h"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
//#include <fcntl.h>

#define printf LOG //To log to logfile AND console

int main( int argc, char* argv[] )
{
    int serial_file = sys_init();
    char buffer[512] = "";
    int n = 0;
    int steps = 400;
    int time = 1;
    unsigned char motor_flag = LEFT_MOTOR_STEPS_FLAG;
    
    while ( 1 )
    {
        
        write( serial_file, &motor_flag, 1 );
        
        n = read(serial_file, &buffer, sizeof(buffer));
        printf("num bytes read: %d\n", n);
        if( n > 0 )
        {
            
            buffer[n+1] = '\0';
            printf("buffer: %s\n", buffer);  // print the part of the buffer that had stuff in it
            fflush(stdout);
        }
        
        write( serial_file, &steps, sizeof(steps) );
        
        n = read(serial_file, &buffer, sizeof(buffer));
        printf("num bytes read: %d\n", n);
        if( n > 0 )
        {
            
            buffer[n+1] = '\0';
            printf("buffer: %s\n", buffer);  // print the part of the buffer that had stuff in it
            fflush(stdout);
        }
        
        write( serial_file, &time, sizeof(time) );
        
        n = read(serial_file, &buffer, sizeof(buffer));
        printf("num bytes read: %d\n", n);
        if( n > 0 )
        {
            
            buffer[n+1] = '\0';
            printf("buffer: %s\n", buffer);  // print the part of the buffer that had stuff in it
            fflush(stdout);
        }
        
        sleep(1);

    }
    


    return 0;
}

int sys_init( )
{
    int serial_file = serialport_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE); // attempts to open the connection to the arduino with the BAUDRATE specified in the ROBOT_DEFINITIONS.h
    
    if(serial_file < 0)
    {
        while(serial_file < 0)
        {
            printf("Can't open serial port, trying again in 1 sec.\n"); // arduino not located, please stop breaking things
            sleep(1);
            serial_file = serialport_init(ARDUINO_COMM_LOCATION, ROBOT_BAUDRATE);
        }
    }

    clearPort(serial_file);
    printf("Serial successfully initialized. File handle: %d\n", serial_file );

    return serial_file;
}
