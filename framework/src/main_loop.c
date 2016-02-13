#include "main_loop.h"

void main_loop( int &serial_port, int &joystick )
{

    int  update_received; // file IDs for the joystick, serial ports
	char joy_address[32] = "/dev/input/";
    // intialize all buttons to "off" (0) this array is checked against for
    // button updates and if an update is found the update is sent. Axis stuff
    // is the same.
	int  old_axis_values[8] = {0};
	int  old_button_values[11] = {0};
	int  new_button_values[11] = {0};
	int  new_axis_values[8] = {0};

	struct js_event jse; // stores the joystick data

    /*************************************************************************
     * Begin main loop.
     *************************************************************************/
    while (true)
    {
        printf("Entering main loop.\n")
        // check for a joystick update
        update_received = read_joystick_event( &jse );
        // check for updates every 1ms
        usleep(1000);
        if (update_recieved)
        {
            switch(jse.type)
            {
                case TYPE_BUTTON:
                    // update the new button array
                    button_update( &jse, new_button_values );
                    break;
                case TYPE_NOT_BUTTON:
                    // udpate the new axis values array
                    axis_update( &jse, new_axis_values );
                    break;
            }
        }
        // checks both the old and new button arrays for differences,
        // if it finds one then an update is sent
        send_button_updates(old_button_values, new_button_values, serial_file);
        send_axis_updates(old_axis_values, new_axis_values, serial_file);
    }
}
