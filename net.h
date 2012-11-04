#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "user.h"
#include "net.h"

#define PORT 1234
#define CLIENTNUM 100
#define DATELEN 128*2
#define IP "106.187.46.158"


int SerNetInit(void);
int CliNetInit(void);
int Accept(int fp,struct sockaddr_in *addr);
int Recv(int fp,char *date);
int Send(int fp,char *date);

#endif
