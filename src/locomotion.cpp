#include "../include/locomotion.h"
#include "../include/robot_defines.h"
//#include "../include/logger.h"
#include "../include/sensors.h"
#include "../include/serial.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>

//#define printf LOG

void setWheelSpeed( int wheel, unsigned char speed )
{
    unsigned char motor_flag;
    int bytes; // To make gcc shut up about ignoring the return value

    if ( wheel == RIGHT )
    {
        //printf("Writing %d to right motor.\n", speed);
        motor_flag = RIGHT_MOTOR_FLAG;
        bytes = bytes + write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to motor\n", bytes );
    } else if ( wheel == LEFT )
    {
        //printf("Writing %d to left motor.\n", speed);
        motor_flag = LEFT_MOTOR_FLAG;
        bytes = bytes + write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to motor\n", bytes );
    } else if ( wheel == BOTH )
    {
        //printf("Writing %d to both motors.\n", speed);
        //printf("Writing %d to right motor.\n", speed);
        motor_flag = RIGHT_MOTOR_FLAG;
        bytes = bytes + write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to motor\n", bytes );

        //printf("Writing %d to left motor.\n", speed);
        motor_flag = LEFT_MOTOR_FLAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &speed, 1 );
        //printf("wrote %d bytes to left motor\n", bytes );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line %d\n", wheel);
    }
}

void driveWheelSteps( int wheel, int steps, int runtime )
{
    //printf("Driving wheel %d %d steps in %d seconds\n", wheel, steps, runtime);
    int n = 0;
    unsigned char motor_flag = 0;

    if ( wheel == RIGHT )
    {
        //printf("Writing to right motor.\n");
        motor_flag = RIGHT_MOTOR_STEPS_FLAG;
        n = n + write( send_port, &motor_flag, 1 );
        n = n + write( send_port, &steps, sizeof(steps) );
        n = n + write( send_port, &runtime, sizeof(runtime) );
    } else if ( wheel == LEFT )
    {
        //printf("Writing to left motor.\n");
        motor_flag = LEFT_MOTOR_STEPS_FLAG;
        n = n + write( send_port, &motor_flag, 1 );
        n = n + write( send_port, &steps, sizeof(steps) );
        n = n + write( send_port, &runtime, sizeof(runtime) );
    } else if ( wheel == BOTH )
    {
        //printf("Writing to both motors.\n");
        driveWheelSteps( RIGHT, steps, runtime );
        driveWheelSteps( LEFT, steps, runtime );
    } else
    {
        printf("Can't write to a wheel that isn't there.\n");
        printf("Hopefully you never see this line. wheel: %d\n", wheel);
    }
}

void turn( int angle, int runtime )
{
    double arc_length = M_PI * WHEEL_BASE_MM * ( angle / 360.0 );
    int steps = round( STEPS_PER_MM * arc_length );

    printf("Turning %d degrees in %d seconds. arc length = %f and %d steps per wheel\n", angle, runtime, arc_length, steps );
    driveWheelSteps( RIGHT, -steps, runtime );
    driveWheelSteps( LEFT, steps, runtime );
}

void var_turn( int angle, int runtime )
{
    //pivots on one wheel. If turning right, pivot on right wheel, etc
    double arc_length;
    unsigned int steps;

    arc_length = 2 * M_PI * WHEEL_BASE_MM * ( angle / 360.0 );
    steps = round( STEPS_PER_MM * arc_length );

    printf("Var-turning %d degrees in %d seconds. arc length = %f and %d steps.\n", angle, runtime, arc_length, steps );
    stop();
    if ( angle < 0 ) // turning left
        driveWheelSteps( RIGHT, steps, runtime );
    else if ( angle > 0 ) // turning right
        driveWheelSteps( LEFT, steps, runtime );
    stop();
}

void drive( float distance, int runtime )
{
    int steps = round( STEPS_PER_CM * distance );
    driveWheelSteps( BOTH, steps, runtime );
}

void stop()
{
    setWheelSpeed( BOTH, 127 );
}

void claw( int state )
{
    unsigned char motor_flag;
    int bytes = 0;
    unsigned char value;

    if ( state == RAISE )
    {
        value = RAISE_VAL;
        printf("Raising claws. Victim begins to scream.\n");
        motor_flag = SERVO_CLAW_RAISE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else if (state == LOWER )
    {
        value = LOWER_VAL;
        printf("Lowering claws. Victim breathes a sigh of relief.\n");
        motor_flag = SERVO_CLAW_RAISE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else if ( state == OPEN )
    {
        value = OPEN_VAL;
        printf("Opening claws. Victim barely escapes with their life.\n");
        motor_flag = SERVO_CLAW_CLOSE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else if ( state == CLOSE )
    {
        value = CLOSE_VAL;
        printf("Closing claws. Crushing victim.\n");
        motor_flag = SERVO_CLAW_CLOSE_TAG;
        bytes = write( send_port, &motor_flag, 1 );
        bytes = bytes + write( send_port, &value, 1 );
    } else {
        printf("I don't know what you mean by that.\n");
        printf("Attempting to send state: %d defined in robot_defines under claw state.\n", state );
    }
}

//////////////////////////////////////////////////
// Here's the wall following locomotion code
//////////////////////////////////////////////////

void forward_until_obstacle( unsigned char speed, float tolerance )
{
    printf("function: %s\n", __func__ );
    double front_value = front_sensor();
    printf("front: %.1f\n", front_value);
    setWheelSpeed( BOTH, speed );

    while ( front_value > SIX_INCHES + tolerance )
    {
        front_value = front_sensor();
        printf("Not hitting wall yet.\nfront: %.1f\n", front_value);
    }
    stop();
    clear_buffer();
}

void forward_until_left_end( unsigned char speed )
{
    printf("function: %s\n", __func__ );
    double left_value = 0;
    double last_pos = 0;
    double delta = 0;
    setWheelSpeed( BOTH, speed );

    while ( delta < WALL_FOLLOW_DELTA )
    {
        last_pos = left_value;
        left_value = left_sensor();
        delta = abs(left_value - last_pos);
        printf("Following left wall.\n");
        printf("left: %.1f\n", left_value );
        printf("delta: %.3f\n", delta );
    }
    stop();
    clear_buffer();
}

void forward_until_right_end( unsigned char speed )
{
    printf("function: %s\n", __func__ );
    double right_value = 0;
    double last_pos = 0;
    double delta = 0;
    setWheelSpeed( BOTH, speed );

    while ( delta < WALL_FOLLOW_DELTA )
    {
        last_pos = right_value;
        right_value = right_sensor();
        delta = abs(right_value - last_pos);
        printf("Following right wall.\n");
        printf("right: %.1f\n", right_value );
        printf("delta: %.3f\n", delta );
    }
    stop();
    clear_buffer();
}

void follow_left_wall_until_end( unsigned char speed, float target )
{
    printf("function: %s\n", __func__ );
    double vic_value = 0;
    double back_value = 0;
    double front_value = 0;
    double left_value = 0;
    double right_value = 0;
    
    sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );

    double last_pos = 0;
    double delta = 0;

    unsigned char speed_mod = 0;
    setWheelSpeed( BOTH, speed );

    while ( delta < WALL_FOLLOW_DELTA )
    {
        last_pos = left_value;
        sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );
        delta = abs(left_value - last_pos);
        printf("left: %.1f\n", left_value );
        printf("delta: %.3f\n", delta );

        if ( left_value < target - WALL_FOLLOW_TOLERANCE ) //too close
        {
            printf("Too close to left wall. Go away.\n");
            if ( left_value < target/2.0 )
            {
                //In danger zone, do something more extreme 
                speed_mod = speed/10 + 10;
                setWheelSpeed( RIGHT, speed-speed_mod );
            } else 
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( RIGHT, speed-speed_mod );
            }
        } else if ( left_value > target + WALL_FOLLOW_TOLERANCE ) // too far away
        {
            printf("Too far away from left wall. </3\n");
            if ( left_value > (target*2.0 - 0.5*10*WHEEL_BASE_MM) ) // Shift the right danger zone boundary by one half the wheel base
            {
                //In danger zone, do something more extreme
                speed_mod = speed/10 + 10;
                setWheelSpeed( RIGHT, speed+speed_mod );
            } else
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( RIGHT, speed+speed_mod );
            }
        } else 
        {
            printf("Goldilocks *would* love you if she was real, but she's not, so she doesn't.\n");
            if ( (last_pos-left_value) < 0 && (abs(last_pos-left_value) > 0.05) ) // approaching goldilocks zone from the left
            {
                // RIGHT wheel will be going speed-speed_mod coming into the goldilocks zone, speed it up
                setWheelSpeed( RIGHT, speed+speed_mod-5 );
            } else if ( (last_pos-left_value) > 0 && (abs(last_pos-left_value) > 0.05) ) // approaching goldilocks zone from the right
            {
                // RIGHT wheel will be going speed+speed_mod coming into the goldilocks zone, slow it down
                setWheelSpeed( RIGHT, speed-speed_mod+5 );
            } else // change in direction wasn't very severe, just go straight
            {
                setWheelSpeed( BOTH, speed );
            }
        }
    }
    stop();
    clear_buffer();
}

void follow_right_wall_until_end( unsigned char speed, float target )
{
    printf("function: %s\n", __func__ );
    double vic_value = 0;
    double back_value = 0;
    double front_value = 0;
    double left_value = 0;
    double right_value = 0;
    
    sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );

    double last_pos = 0;
    double delta = 0;

    unsigned char speed_mod = 0;
    setWheelSpeed( BOTH, speed );

    while ( delta < WALL_FOLLOW_DELTA )
    {
        last_pos = right_value;
        sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );

        delta = abs(right_value - last_pos);
        printf("right: %.1f\n", right_value );
        printf("delta: %.3f\n", delta );

        if ( right_value < target - WALL_FOLLOW_TOLERANCE ) //too close
        {
            printf("Too close to right wall. Go away.\n");
            if ( right_value < target/2.0 )
            {
                //In danger zone, do something more extreme 
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed-speed_mod );
            } else 
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed-speed_mod );
            }
        } else if ( right_value > target + WALL_FOLLOW_TOLERANCE ) // too far away
        {
            printf("Too far away from right wall. </3\n");
            if ( right_value > (target*2.0 - 0.5*10*WHEEL_BASE_MM) ) // Shift the right danger zone boundary by one half the wheel base
            {
                //In danger zone, do something more extreme
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed+speed_mod );
            } else
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed+speed_mod );
            }
        } else 
        {
            printf("Goldilocks *would* love you if she was real, but she's not, so she doesn't.\n");
            if ( (last_pos-right_value) < 0 && (abs(last_pos-right_value) > 0.05) ) // approaching goldilocks zone from the left
            {
                // LEFT wheel will be going speed-speed_mod coming into the goldilocks zone, speed it up
                setWheelSpeed( LEFT, speed+speed_mod-5 );
            } else if ( (last_pos-right_value) > 0 && (abs(last_pos-right_value) > 0.05) ) // approaching goldilocks zone from the right
            {
                // LEFT wheel will be going speed+speed_mod coming into the goldilocks zone, slow it down
                setWheelSpeed( LEFT, speed-speed_mod+5 );
            } else // change in direction wasn't very severe, just go straight
            {
                setWheelSpeed( BOTH, speed );
            }
        }
    }
    stop();
    clear_buffer();
}

void follow_right_wall_until_left_open( unsigned char speed, float target )
{
    printf("function: %s\n", __func__ );
    double vic_value = 0;
    double back_value = 0;
    double front_value = 0;
    double left_value = 0;
    double right_value = 0;
    
    sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );

    double last_right_pos = 0;
    double last_left_pos = 0;
    double delta = 0;

    unsigned char speed_mod = 0;
    setWheelSpeed( BOTH, speed );

    while ( delta < WALL_FOLLOW_DELTA )
    {
        last_right_pos = right_value;
        last_left_pos = left_value;
        sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );
        delta = abs(left_value - last_left_pos);
        printf("right: %.1f\n", right_value );
        printf("delta: %.3f\n", delta );

        if ( right_value < target - WALL_FOLLOW_TOLERANCE ) //too close
        {
            printf("Too close to right wall. Go away.\n");
            if ( right_value < target/2.0 )
            {
                //In danger zone, do something more extreme 
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed-speed_mod );
            } else 
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed-speed_mod );
            }
        } else if ( right_value > target + WALL_FOLLOW_TOLERANCE ) // too far away
        {
            printf("Too far away from right wall. </3\n");
            if ( right_value > (target*2.0 - 0.5*10*WHEEL_BASE_MM) ) // Shift the right danger zone boundary by one half the wheel base
            {
                //In danger zone, do something more extreme
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed+speed_mod );
            } else
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed+speed_mod );
            }
        } else 
        {
            printf("Goldilocks *would* love you if she was real, but she's not, so she doesn't.\n");
            if ( (last_right_pos-right_value) < 0 && (abs(last_right_pos-right_value) > 0.05) ) // approaching goldilocks zone from the left
            {
                // LEFT wheel will be going speed-speed_mod coming into the goldilocks zone, speed it up
                setWheelSpeed( LEFT, speed+speed_mod-5 );
            } else if ( (last_right_pos-right_value) > 0 && (abs(last_right_pos-right_value) > 0.05) ) // approaching goldilocks zone from the right
            {
                // LEFT wheel will be going speed+speed_mod coming into the goldilocks zone, slow it down
                setWheelSpeed( LEFT, speed-speed_mod+5 );
            } else // change in direction wasn't very severe, just go straight
            {
                setWheelSpeed( BOTH, speed );
            }
        }
    }
    stop();
    clear_buffer();
}

void follow_left_wall_until_obstacle( unsigned char speed, float target, float tolerance )
{
    printf("function: %s\n", __func__ );
    double vic_value = 0;
    double back_value = 0;
    double front_value = 0;
    double left_value = 0;
    double right_value = 0;
    
    sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );

    double last_pos = 0;

    unsigned char speed_mod = 0;
    setWheelSpeed( BOTH, speed );

    while ( front_value > SIX_INCHES + tolerance )
    {
        last_pos = left_value;
        sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );
        printf("left: %.1f\n", left_value );
        printf("front: %.1f\n", front_value );

        if ( left_value < target - WALL_FOLLOW_TOLERANCE ) //too close
        {
            printf("Too close to left wall. Go away.\n");
            if ( left_value < target/2.0 )
            {
                //In danger zone, do something more extreme 
                speed_mod = speed/10 + 10;
                setWheelSpeed( RIGHT, speed-speed_mod );
            } else 
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( RIGHT, speed-speed_mod );
            }
        } else if ( left_value > target + WALL_FOLLOW_TOLERANCE ) // too far away
        {
            printf("Too far away from left wall. </3\n");
            if ( left_value > (target*2.0 - 0.5*10*WHEEL_BASE_MM) ) // Shift the right danger zone boundary by one half the wheel base
            {
                //In danger zone, do something more extreme
                speed_mod = speed/10 + 10;
                setWheelSpeed( RIGHT, speed+speed_mod );
            } else
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( RIGHT, speed+speed_mod );
            }
        } else 
        {
            printf("Goldilocks *would* love you if she was real, but she's not, so she doesn't.\n");
            if ( (last_pos-left_value) < 0 && (abs(last_pos-left_value) > 0.05) ) // approaching goldilocks zone from the left
            {
                // RIGHT wheel will be going speed-speed_mod coming into the goldilocks zone, speed it up
                setWheelSpeed( RIGHT, speed+speed_mod-5 );
            } else if ( (last_pos-left_value) > 0 && (abs(last_pos-left_value) > 0.05) ) // approaching goldilocks zone from the right
            {
                // RIGHT wheel will be going speed+speed_mod coming into the goldilocks zone, slow it down
                setWheelSpeed( RIGHT, speed-speed_mod+5 );
            } else // change in direction wasn't very severe, just go straight
            {
                setWheelSpeed( BOTH, speed );
            }
        }
    }
    stop();
    clear_buffer();
}

void follow_right_wall_until_obstacle( unsigned char speed, float target, float tolerance )
{
    printf("function: %s\n", __func__ );
    double vic_value = 0;
    double back_value = 0;
    double front_value = 0;
    double left_value = 0;
    double right_value = 0;
    
    sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );

    double last_pos = 0;

    unsigned char speed_mod = 0;
    setWheelSpeed( BOTH, speed );

    while ( front_value > SIX_INCHES + tolerance )
    {
        last_pos = right_value;
        sensors( &vic_value, &back_value, &front_value, &left_value, &right_value );
        printf("right: %.1f\n", right_value );
        printf("front: %.1f\n", front_value );

        if ( right_value < target - WALL_FOLLOW_TOLERANCE ) //too close
        {
            printf("Too close to right wall. Go away.\n");
            if ( right_value < target/2.0 )
            {
                //In danger zone, do something more extreme 
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed-speed_mod );
            } else 
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed-speed_mod );
            }
        } else if ( right_value > target + WALL_FOLLOW_TOLERANCE ) // too far away
        {
            printf("Too far away from right wall. </3\n");
            if ( right_value > (target*2.0 - 0.5*10*WHEEL_BASE_MM) ) // Shift the right danger zone boundary by one half the wheel base
            {
                //In danger zone, do something more extreme
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed+speed_mod );
            } else
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed+speed_mod );
            }
        } else 
        {
            printf("Goldilocks *would* love you if she was real, but she's not, so she doesn't.\n");
            if ( (last_pos-right_value) < 0 && (abs(last_pos-right_value) > 0.05) ) // approaching goldilocks zone from the left
            {
                // LEFT wheel will be going speed-speed_mod coming into the goldilocks zone, speed it up
                setWheelSpeed( LEFT, speed+speed_mod-5 );
            } else if ( (last_pos-right_value) > 0 && (abs(last_pos-right_value) > 0.05) ) // approaching goldilocks zone from the right
            {
                // LEFT wheel will be going speed+speed_mod coming into the goldilocks zone, slow it down
                setWheelSpeed( LEFT, speed-speed_mod+5 );
            } else // change in direction wasn't very severe, just go straight
            {
                setWheelSpeed( BOTH, speed );
            }
        }
    }
    stop();
    clear_buffer();
}

/*
* This version of the left wall following function should damp the "sine wave"
* pattern we have been noticing. This should reduce irregularities in the
* performance of the robot while pathing. If this method works well we should
* be able to apply this to the right wall following function as well.
*/
void test_follow_left_wall_until_end (unsigned char speed, float target)
{
    printf("function: %s\n", __func__ );
    double left_value = left_sensor();
    unsigned char speed_mod = speed/10 - 10;
    int divis_val = 1;
    int i = 0;

    setWheelSpeed(BOTH, speed);
    while ( left_value < INF_DISTANCE )
    {
        while( (left_value > target + WALL_FOLLOW_TOLERANCE) && (left_value < INF_DISTANCE) )
        {
            printf("Too far away from left wall.\n");
            i++;
            if ( i % 10 && (abs(left_value-target) > 1) )
            {
                speed_mod = speed/10;
            }else if ( i % 5 && (speed_mod > 5) )
            {
                speed_mod -= divis_val;
            }
            left_value = left_sensor();
            printf("left: %.1f speed_mod: %d\n", left_value, speed_mod);
            // increase speed of right wheel
            setWheelSpeed( RIGHT, speed + speed_mod );
        }
        //reset the things before we need them again
        i = 0;
        speed_mod = speed/10 - 10;

        while( (left_value < target - WALL_FOLLOW_TOLERANCE) && (left_value < INF_DISTANCE) )
        {
            printf("Too close to left wall.\n");
            i++;
            if ( i % 10 && (abs(left_value-target) > 1) )
            {
                speed_mod = speed/10;
            }else if ( i % 5 && (speed_mod > 5) )
            {
                speed_mod -= divis_val;
            }
            left_value = left_sensor();
            printf("left: %.1f speed_mod: %d\n", left_value, speed_mod);
            // decrease speed of right wheel
            setWheelSpeed( RIGHT, speed - speed_mod );
        }
        left_value = left_sensor();
        printf("left: %.1f.\n", left_value);
        //reset the things before we need them again
        i = 0;
        speed_mod = speed/10 - 10;
    }
    stop();
    clear_buffer();
}

// This version is essentially a copy of https://www.cs.hmc.edu/~dodds/projects/RobS01/Assignment2/Fixed_HTML/wfcode.html
// Modified slightly for our use case.
void var_test_follow_left_wall_until_end( unsigned char speed, float target )
{
    printf("function: %s\n", __func__ );
    // float error = 0;
    // float error_last = 0;
    double vic = 0;
    double back = 0;
    double front = 0;
    double left_value = 0;
    double right = 0;

    sensors( &vic, &back, &front, &left_value, &right );
    double last_pos = left_value;
    double delta = abs(left_value - last_pos);

    // float kp = 0;
    // float kd = 0;

    unsigned char speed_mod = 0;
    // unsigned char temp = 0;

    setWheelSpeed( BOTH, speed );

    while ( delta < WALL_FOLLOW_DELTA ) //if the distance drops too much, stop
    {
        last_pos = left_value;
        sensors( &vic, &back, &front, &left_value, &right );
        delta = abs(left_value - last_pos);
        printf("left: %.1f\n", left_value );
        printf("delta: %.3f\n", delta);

        if ( left_value < (target - WALL_FOLLOW_TOLERANCE) ) //too close
        {
            printf("Too close to left wall. Go away.\n");
            if ( left_value < target/2.0 )
            {
                //In danger zone, do something more extreme 
                speed_mod = speed/10 + 10;
                setWheelSpeed( RIGHT, speed-speed_mod );
            } else 
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( RIGHT, speed-speed_mod );
            }
            //adjust inside wheel, ignore for now
            // temp = kp*error + kd*(error - error_last);
            //setWheelSpeed( LEFT, temp );
        } else if ( left_value > (target + WALL_FOLLOW_TOLERANCE) ) // too far away
        {
            printf("Too far away from left wall. </3\n");
            if ( left_value > (target*2.0 - 0.5*10*WHEEL_BASE_MM) ) // Shift the right danger zone boundary by one half the wheel base
            {
                //In danger zone, do something more extreme
                speed_mod = speed/10 + 10;
                setWheelSpeed( RIGHT, speed+speed_mod );
            } else
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( RIGHT, speed+speed_mod );
            }
            //adjust inside wheel, ignore for now
            // temp = kp*error + kd*(error - error_last);
            //setWheelSpeed( LEFT, temp );
        } else 
        {
            printf("Goldilocks *would* love you if she was real, but she's not, so she doesn't.\n");
            // setWheelSpeed( BOTH, speed );
            if ( (last_pos-left_value) < 0 && (abs(last_pos-left_value) > 0.05) ) // approaching goldilocks zone from the left
            {
                // RIGHT wheel will be going speed-speed_mod coming into the goldilocks zone, speed it up
                setWheelSpeed( RIGHT, speed+speed_mod-5 );
            } else if ( (last_pos-left_value) > 0 && (abs(last_pos-left_value) > 0.05) ) // approaching goldilocks zone from the right
            {
                // RIGHT wheel will be going speed+speed_mod coming into the goldilocks zone, slow it down
                setWheelSpeed( RIGHT, speed-speed_mod+5 );
            } else // change in direction wasn't very severe, just go straight
            {
                setWheelSpeed( BOTH, speed );
            }
        }
    }
    stop();
    clear_buffer();
}

// Follows wall until the new vic Sensor detects a victim 
void follow_right_wall_until_VS( unsigned char speed, float target )
{
    printf("function: %s\n", __func__ );
    double right_value = right_sensor();
    double last_pos = 0;
    unsigned char speed_mod = 0;
    setWheelSpeed( BOTH, speed );

    while ( vic_sensor() < VIC_DISTANCE )
    {
        last_pos = right_value;
        right_value = right_sensor();
        printf("right: %.1f\n", right_value );

        if ( right_value < target - WALL_FOLLOW_TOLERANCE ) //too close
        {
            printf("Too close to right wall. Go away.\n");
            if ( right_value < target/2.0 )
            {
                //In danger zone, do something more extreme 
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed-speed_mod );
            } else 
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed-speed_mod );
            }
        } else if ( right_value > target + WALL_FOLLOW_TOLERANCE ) // too far away
        {
            printf("Too far away from right wall. </3\n");
            if ( right_value > (target*2.0 - 0.5*10*WHEEL_BASE_MM) ) // Shift the right danger zone boundary by one half the wheel base
            {
                //In danger zone, do something more extreme
                speed_mod = speed/10 + 10;
                setWheelSpeed( LEFT, speed+speed_mod );
            } else
            {
                //Between danger zone and target zone
                speed_mod = speed/10;
                setWheelSpeed( LEFT, speed+speed_mod );
            }
        } else 
        {
            printf("Goldilocks *would* love you if she was real, but she's not, so she doesn't.\n");
            if ( (last_pos-right_value) < 0 && (abs(last_pos-right_value) > 0.05) ) // approaching goldilocks zone from the left
            {
                // LEFT wheel will be going speed-speed_mod coming into the goldilocks zone, speed it up
                setWheelSpeed( LEFT, speed+speed_mod-5 );
            } else if ( (last_pos-right_value) > 0 && (abs(last_pos-right_value) > 0.05) ) // approaching goldilocks zone from the right
            {
                // LEFT wheel will be going speed+speed_mod coming into the goldilocks zone, slow it down
                setWheelSpeed( LEFT, speed-speed_mod+5 );
            } else // change in direction wasn't very severe, just go straight
            {
                setWheelSpeed( BOTH, speed );
            }
        }
    }
    stop();
    clear_buffer();
}


// Follows the right wall until the NEW front sensor detects a victim
void getVictim()
{
    printf("function: %s\n", __func__ );
    stop();
    clear_buffer();
}
