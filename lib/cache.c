#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "cache.h"

// 新建缓存链表结点
struct ListNode* listnode_new(int size) {
    struct ListNode *node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->buffer = (char*)malloc(size);
    bzero(node->buffer, size);
    node->bufferSize = size;
    node->next = NULL;
    return node;
}

// 销毁缓存链表结点
void listnode_free(struct ListNode *node) {
    if(node == NULL) return;
    if(node->buffer != NULL) {
        free(node->buffer);
        node->buffer = NULL;
    }
    free(node);
    node = NULL;
}

// 新建缓存链表
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

// 将数据写入缓存链表尾部
void cache_write(struct Cache *cache, char *buffer, int size) {
    struct ListNode *p = listnode_new(size);
    buffercpy(p->buffer, buffer, size);

    // 加读写锁
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

// 从缓存结构链表中读取数据，并销毁被读取过的结点
int cache_read(struct Cache *cache, char **buffer, int *readSize) {

    // 加读写锁
    pthread_rwlock_wrlock(&cache->lock); 

    int readStatus = 0; 
    if(is_cache_readable(cache) == 1) {
	
        readStatus = 1;
        *readSize = cache->readIndex->bufferSize;
        *buffer = (char*)malloc(*readSize);
        struct ListNode *t = cache->readIndex;
        buffercpy(*buffer, cache->readIndex->buffer, *readSize);
        cache->readIndex = cache->readIndex->next;
         
        if(t == cache->writeIndex) cache->writeIndex = NULL;
        listnode_free(t);

    }
    pthread_rwlock_unlock(&cache->lock); 

    return readStatus;
}

// 判断缓存链表中是否有数据
int is_cache_readable(struct Cache *cache) {
    return (cache->readIndex == NULL) ? 0 : 1;
}

// 缓存一段后再播放 todo
int is_cache_buffer_enough(struct Cache *cache) {
    return (cache->chuncks > 50) ? 1 : 0;
}

// 销毁缓存链表
void cache_free(struct Cache *cache) {
    pthread_rwlock_destroy(&cache->lock);
    while(cache->readIndex) {
        listnode_free(cache->readIndex);
    }

    free(cache);
    cache = NULL;
}

