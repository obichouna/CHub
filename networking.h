#ifndef NETWORKING_H
#define NETWORKING_H

#define BUFFER_SIZE 1000

void error_check( int i, char *s );

int server_setup();
int client_setup(char *);
int server_connect(int sd);

int client_connect( char *host );

int send_data(int connection_descript,void *data);
int receive_data(int connection_descript,void *data);



#endif

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

