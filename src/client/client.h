#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct Client {
    int sock;
    struct sockaddr_in addr;
};

struct Client* client_init(char ip[], int port);

char* client_recv(int serSock, int bufferSize, int *recvSize);

void client_send(struct Client *client, char *buffer, int bufferSize);

#endif
