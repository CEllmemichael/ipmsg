#ifndef TOOL_H
#define TOOL_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "tools.h"
#define pf printf
#define UDP_PORT 6550
#define TCP_PORT 8000

void *Udp_res(void *p ) ;

void UDP_11(int *login,char *myname,char *a)

void UDP_11(int *login,char *myname,char *append);

void transcode(cmds *object, char *buffer, int len);

#endif//TOOL_h