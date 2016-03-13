#include "../include/serial.h"
#include "../include/logger.h"
#include "../include/robot_defines.h"

#include <stdio.h>
#include <unistd.h>

#define printf LOG
#define perror LOG

void serial_write( int port, int val, int bytes) // generic multipurpose write
{
    if ( write( port, &val, bytes ) )
    {
        printf("%d written to port %d successfully\n", val, port );
    } else {
        printf("%d bytes of %d NOT written to port %d successfully\n", bytes, val, port );
    }
}

void clearPort( int port )
{
    int n = 1;
    char nothing = 0;
    while(n > 0)
        n = read(port, &nothing, 1);
    return;
}

int serial_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;

    printf("serial_init: opening port %s @ %d bps\n", serialport, baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("serial_init: Unable to open port.\n");
        return -1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        perror("serial_init: Couldn't get term attributes.\n");
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
        perror("serial_init: Couldn't set term attributes\n");
        return -1;
    }

    return fd;
}

int s_read_until(int fd, char* buf, char until)
{
    unsigned char in = 0;
    int i = 0;
    int n = 0;
    buf[0] = '\0';
    //printf("Received: ");

    while(in != until)
    {
        n = read(fd, &in, 1);  // read a char at a time
        if( n==-1)
        {
            perror("Unable to read from port\n");
            return -1;    // couldn't read
        }
        if( n==0 )
            usleep( 1 * 1000 ); // wait 1 msec try again
        if(n > 0)
        {
            buf[i] = in;
            //printf("|%#4x|", in);
            i++;
        }
    }
  //printf("ending recevived. \n");
  buf[i] = '\0';  // null terminate the string
  return strlen(buf);
}
