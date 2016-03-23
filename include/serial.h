#ifndef __SERIAL_DEFINES_
#define __SERIAL_DEFINES_

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

int serial_init(const char* serialport, int baud);
void clearPort(int port);
void s_write( int port, int val, int bytes); // generic multipurpose write
int s_read_until(int fd, char* buf, char until);

void clear_buffer();

#endif
