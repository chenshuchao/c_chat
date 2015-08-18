#include "server.h"

int server_init(int port) {
    printf("%s\n", "server init");

    int serverSock;
    struct sockaddr_in serverAddr;
    serverSock = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(5011);
 
    if (bind(serverSock,(struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    return serverSock;    
}

char* server_recv(int serSock, int bufferSize, int *recvSize) {
    printf("%s\n","recving...");

    struct sockaddr_in cliAddr;
    socklen_t cliAddrLen = sizeof(cliAddr);    
    char *buffer = (char*)malloc(bufferSize);

    if((*recvSize = recvfrom(serSock, (void*)buffer, bufferSize, 0,
                       (struct sockaddr*)&cliAddr, &cliAddrLen)) < 0) {
        perror("recvfrom error");
    }

    return buffer;
}

