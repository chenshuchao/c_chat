#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "cache.h"

struct ListNode* listnode_new(int size) {
    struct ListNode *node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->buffer = (char*)malloc(size);
    bzero(node->buffer, size);
    node->bufferSize = size;
    node->next = NULL;
    return node;
}

void listnode_free(struct ListNode *node) {
    if(node == NULL) return;
    if(node->buffer != NULL) {
        free(node->buffer);
        node->buffer = NULL;
    }
    //free(node);
    //node = NULL;
}

struct Cache* cache_new() {
    struct Cache *cache = (struct Cache*)malloc(sizeof(struct Cache));
    cache->chuncks = 0;
    cache->readIndex = NULL;
    cache->writeIndex = NULL;
    if(pthread_rwlock_init(&cache->lock, NULL) != 0) {
        perror("Lock init error!");
        exit(1);
    }
    
    return cache;
}

void cache_write(struct Cache *cache, char *buffer, int size) {
    struct ListNode *p = listnode_new(size);
    buffercpy(p->buffer, buffer, size);
    // lock
    pthread_rwlock_wrlock(&cache->lock); 
    if(cache->writeIndex == NULL) {
        cache->writeIndex = p;
    } else {
        cache->writeIndex->next = p;
        cache->writeIndex = cache->writeIndex->next;
    }

    if(cache->readIndex == NULL) cache->readIndex = cache->writeIndex;
    
    cache->chuncks++;
    cache->chuncks = cache->chuncks % 10000;

    pthread_rwlock_unlock(&cache->lock); 
}

bool cache_read(struct Cache *cache, char **buffer, int *readSize) {
    pthread_rwlock_wrlock(&cache->lock); 

    bool readStatus = false; 
    if(is_cache_readable(cache) == 1) {
	
        readStatus = true;
        *readSize = cache->readIndex->bufferSize;
        *buffer = (char*)malloc(*readSize);
        ListNode *t = cache->readIndex;
        buffercpy(*buffer, cache->readIndex->buffer, *readSize);
        cache->readIndex = cache->readIndex->next;

        listnode_free(t);

    }
    pthread_rwlock_unlock(&cache->lock); 

    return readStatus;
}

int is_cache_readable(struct Cache *cache) {
    return (cache->readIndex == NULL) ? 0 : 1;
}

void cache_free(struct Cache *cache) {
    pthread_rwlock_destroy(&cache->lock);
    while(cache->readIndex) {
        listnode_free(cache->readIndex);
    }

    free(cache);
    cache = NULL;
}

