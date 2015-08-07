#include "stdio.h"
#include "utils.h"

char* buffercpy(char *newBuffer, char *buffer, int size) {
    for(int i = 0; i < size; i++) {
        newBuffer[i] = buffer[i];
    }
    return newBuffer;
}

