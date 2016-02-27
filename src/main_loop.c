#include "../include/main_loop.h"
#include "../include/locomotion.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"

#include <stdio.h>

#define printf LOG //for logging goodness

void main_loop( int *serial_port )
{

    /*************************************************************************
     * Begin main loop.
     *************************************************************************/
    while ( TRUE )
    {
        printf("Entering main loop\n");
        printf("test %d\n", 2);
        printf("float %f\n", 2.3);
        printf("u %u\n", 2);

        break;
    }
}
