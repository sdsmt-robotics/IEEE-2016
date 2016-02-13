#include "main_loop.h"

void main_loop( int &serial_port, int &joystick )
{

    int  joy_file, received, serial_file; // file IDs for the joy, in serial, and out serial ports
	char joy_address[32] = "/dev/input/";
	int  old_axis_values[8] = {0};     // intialize all buttons to "off" (0) this array is check edagainst for button updates and if an update is found the update is sent. Axis stuff is the same
	int  old_button_values[11] = {0};  // array as mentioned above
	int  new_button_values[11] = {0};  // just look above
	int  new_axis_values[8] = {0};     // just look above the above

	struct js_event jse;               // stores the joystick data



    while (true) {
        update_received = read_joystick_event(&jse); // check for a joystick update
        //printf("Reading...\n");
        usleep(1000); // check for updates every 1mS
        if (update_recieved) {
            switch(jse.type)
            {
                case TYPE_BUTTON:
                    //printf("NOT_BUTTON\n");
                    button_update(&jse, new_button_values); // update teh new button array
                    break;
                case TYPE_NOT_BUTTON:
                    //printf("BUTTON\n");
                    axis_update(&jse, new_axis_values); // udpate the new axis values array
                    break;
            }
        }
        send_button_updates(old_button_values, new_button_values, serial_file); // checks bot the old and new button arrays for differences, if it finds one then an update is sent
        send_axis_updates(old_axis_values, new_axis_values, serial_file); // ditto from above
    }
}
