#include "stdio.h"
#include "utils.h"

char* buffercpy(char *newBuffer, char *buffer, int size) {
    printf("buffercpy size:%d\n", size);
    for(int i = 0; i < size; i++) {
        newBuffer[i] = buffer[i];
    }
    printf("%s\n", "buffercpy end");
    return newBuffer;
}


