#ifndef __CACHE_H__
#define __CACHE_H__

#include <stdlib.h>

struct ListNode {
    char* buffer;
    int bufferSize;
    struct ListNode *next;
};

struct ListNode* listnode_new(int size);


struct Cache {
    struct ListNode *readIndex;
    struct ListNode *writeIndex;
};

struct Cache* cache_new();

void cache_write(struct Cache *cache, char *buffer, int bufferSize);

char* cache_read(struct Cache *cache, int *readSize);

int is_cache_readable(struct Cache *cache);

void cache_free(struct Cache *cache);

#endif

