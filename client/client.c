#include "client.h"

struct Client* client_init(char ip[], int port) {
    printf("%s", "client init\n");
    struct Client *client = (struct Client*)malloc(sizeof(struct Client));
    if(( client->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    bzero(&client->addr, sizeof(client->addr));
    client->addr.sin_family = AF_INET;
    client->addr.sin_port = htons(5011);
    client->addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    return client;
}

void client_send(struct Client *client, char *buffer, int bufferSize) {
    printf("%s", "sending...\n");
    socklen_t addr_len = sizeof(struct sockaddr_in);

    if(sendto(client->sock, (void*)buffer, bufferSize, 0, (struct sockaddr*)&client->addr, addr_len) < 0) {
        perror("sendto error");
        exit(1);
    }
}

