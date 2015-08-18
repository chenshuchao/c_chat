#ifndef __SERVER_H__
#define __SERVER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

struct Server {
    int port;
};

int server_init(int port);

void server_send(struct Server *ser);

char* server_recv(int serSock, int bufferSize, int *recvSize);

#endif

