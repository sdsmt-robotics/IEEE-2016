#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

#define LEFT_MOTOR_FLAG 0x52
#define RIGHT_MOTOR_FLAG 0x4C






int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
    
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}

int serialport_read_until(int fd, char* buf, char until)
{
    unsigned char in = 0;
    int i = 0;
    int n = 0;
    buf[0] = '\0';
    printf("Received: ");
    
    while(in != until)
    {
        n = read(fd, &in, 1);  // read a char at a time
        if( n==-1) 
        {
            printf("Unable to read.");
            return -1;    // couldn't read
        }
        if( n==0 )
            usleep( 10 * 1000 ); // wait 100 msec try again
        if(n > 0)
        {
            buf[i] = in;
            printf("|%#4x|", in);
            i++;
        }
    }
    //printf("ending recevived. \n");

    buf[i] = '\0';  // null terminate the string
    return 1;
}

void sendPacket(unsigned char speed, int port)
{
	unsigned char packet[8] = {0};
    int i;

	packet[0] = 0x02; //start
    packet[1] = 0x4C; //'L'
    packet[2] = speed;
    for(i = 3; i < 6; i++) //fills the rest with zeros
        packet[i] = 0;
    unsigned char checksum = 0;
    for(i = 1; i < 6; i++)
        checksum = checksum ^ packet[i]; //generate checksum
    packet[6] = checksum; //checksum
    packet[7] = 0x03; //end

    printf("    Sent: ");
    for(i = 0; i <= 7; i++)
    {
        write(port, &packet[i], 1);
        printf("|%#4x|", packet[i]);
    }
    printf("\n");
    return;
}

void clearPort(int port)
{
    int n = 1;
    char nothing = 0;
    while(n > 0)
        n = read(port, &nothing, 1);
    return;
}

void setLeftMotorSpeed(unsigned char speed, int port)
{
    unsigned char flag = LEFT_MOTOR_FLAG;
    write(port, &flag, 1); //identify left motor
    write(port, &speed, 1); //send the speed necessitated
    return;
}

void setRightMotorSpeed(unsigned char speed, int port)
{
    unsigned char flag = RIGHT_MOTOR_FLAG;
    write(port, &flag, 1); //identify left motor
    write(port, &speed, 1); //send the speed necessitated
    return;
}

int main(int argc, char *argv[])
{
	int port = 0;
	char portName[128] = "/dev/robot/arduino"; //default port location
	int baud = B115200; //baud unless specified
	unsigned int speed = 0; //speed variable
	char input[128] = {0}; //input buffer

    port = serialport_init(portName, baud); //init port with certain baud

    if(port < 0)
    {
        printf("Unable to open port.");
        exit(1);
    }

    clearPort(port);

	while(1)
	{
        memset(input, '\0', sizeof(input)); //wipes input buffer
		printf("\nEnter a # 0-255 for both motors speed: ");
		scanf("%u", &speed);
        setLeftMotorSpeed( speed, port);
        setRightMotorSpeed( speed, port);
		//sendPacket((unsigned char)speed, port);
        //serialport_read_until(port, input, '\n');
        //printf("Received: %s", input);
	}
}
