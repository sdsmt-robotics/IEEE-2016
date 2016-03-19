/*
* This is a planning document for the navigation code. It is intended to be used
* for figuring out improvements to the nav code and not to be linked to by other
* files.
*/
#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/sensors.h"

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>



/*
* This version of the left wall following function should damp the "sine wave"
* pattern we have been noticing. This should reduce irregularities in the
* performance of the robot while pathing. If this method works well we should
* be able to apply this to the right wall following function as well.
*/
follow_left_wall_until_end (unsigned char speed, int target)
{
    set WheelSpeed(BOTH, speed); //start moving forward
    double left_value = left_sensor(); // poll sensor
    char buffer[512] = "";
    char speed_mod = speed/10; // one tenth of given speed
    
    while (left_value < 6.5) // while the wall is there, 6.5 is "infinite"
    {
        //if we exceed desired dist, move left
        while(left_value > target + WALL_FOLLOW_TOLERANCE)
        {
            // increase speed of right wheel
            setWheelSpeed( RIGHT, speed + speed_mod );
            left_value = left_sensor(); // poll sensor
            speed_mod /= 2; //example reduction
        }
        //if we get under desired dist, move right
        while(left_value < target - WALL_FOLLOW_TOLERANCE)
        {
            // decrease speed of right wheel
            setWheelSpeed( RIGHT, speed - speed_mod);
            left_value = left_sensor(); //poll sensor
            speed_mod /= 2; //example reduction
        }
        left_value = left_sensor();
        printf("Sensor value: %.2f.\n",left_value);
        // You could instead check the sensor here

    }
    setWheelSpeed( BOTH, speed );
    stop();
    int bytes = read( serial_port, &buffer, sizeof(buffer) );
    if ( bytes > 0 )
    {
        buffer[bytes] = '\0';
        printf("buffer (%d bytes): %s\n", bytes, buffer );
        fflush(stdout);
    }
}