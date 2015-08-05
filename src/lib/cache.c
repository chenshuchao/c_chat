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
    cache->readIndex = NULL;
    cache->writeIndex = NULL;
    return cache;
}

void cache_write(struct Cache *cache, char *buffer, int size) {
    struct ListNode *p = listnode_new(size);
    printf("%p\n", p->buffer);
    if(p->buffer == NULL) {
        printf("%s\n", "paaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        exit(1);
    }
    printf("%s\n", "buffercpy in cache_write");
    buffercpy(p->buffer, buffer, size);
     
    printf("-------%d--------------------%c\n", size, p->buffer[1]);
    if(cache->writeIndex == NULL) {
        cache->writeIndex = p;
        printf("%s\n", "hiIamNULL");
    } else {
        cache->writeIndex->next = p;
        cache->writeIndex = cache->writeIndex->next;
        printf("%s\n", "hiIamnotNULL");
    }

    if(cache->writeIndex->buffer == NULL) {
        printf("%s\n", "writeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        exit(1);
    }

    if(cache->readIndex == NULL) cache->readIndex = cache->writeIndex;
    printf("cache write: %d\n", size);
}

char* cache_read(struct Cache *cache, int *readSize) {
    //if(cache->readIndex == NULL) return;

    *readSize = cache->readIndex->bufferSize;
    char *buffer = (char*)malloc(*readSize);
    printf("%s\n", "buffercpy in cache_read");
    ListNode *t = cache->readIndex;

    if(cache->readIndex->buffer == NULL) {
        printf("%s\n", "buffer NULL");
    }

    buffercpy(buffer, cache->readIndex->buffer, *readSize);
    cache->readIndex = cache->readIndex->next;

    listnode_free(t);
    printf("cache read: %d\n", *readSize);
    return buffer;
}

int is_cache_readable(struct Cache *cache) {
    return (cache->readIndex == NULL) ? 0 : 1;
}

void cache_free(struct Cache *cache) {
    while(cache->readIndex) {
        listnode_free(cache->readIndex);
    }
    free(cache);
    cache = NULL;
}

