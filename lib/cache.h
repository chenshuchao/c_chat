#ifndef __CACHE_H__
#define __CACHE_H__

#include <stdlib.h>
#include <pthread.h>

struct ListNode {
    char* buffer;
    int bufferSize;
    struct ListNode *next;
};

struct ListNode* listnode_new(int size);

struct Cache {
    int chuncks;
    struct ListNode *readIndex;
    struct ListNode *writeIndex;
    pthread_rwlock_t lock;
};

struct Cache* cache_new();

void cache_write(struct Cache *cache, char *buffer, int bufferSize);

int cache_read(struct Cache *cache, char **buffer, int *readSize);

int is_cache_readable(struct Cache *cache);

int is_cache_buffer_enough(struct Cache *cache);

void cache_free(struct Cache *cache);

#endif

