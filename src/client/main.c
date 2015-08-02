#include <pthread.h>
#include "lib/utils.h"
#include "lib/sound.h"
#include "lib/cache.h"
#include "client.h"

// 读取缓存数据并发送
void read_cache_and_send(struct Cache *cache) {
    printf("%s", "read_cache_and_send\n");
    char *buffer;
    //int bufferSize = 4096;
    //buffer = (char*)malloc(bufferSize);
    int recvSize = 0;
    struct Client *client = client_init("127.0.0.1", 5011);
    while(1) {
        if(is_cache_readable(cache) == 0) continue;

	printf("%s\n", "readable");
        //bzero(buffer, bufferSize);
        buffer = cache_read(cache, &recvSize);
        if(recvSize) {
            client_send(client, buffer, recvSize);
        }
        free(buffer);
        buffer = NULL;
    }
}

// 从声卡读取数据并写入缓存
void* record_and_write_cache(void *cache1) {
    printf("%s", "record_and_write_cache\n");

    struct Cache *cache = (struct Cache*)cache1;
    struct SoundInfo *soundInfo = sound_info_new();

    while(1) {
        bzero(soundInfo->buffer, soundInfo->bufferSize);
        sound_info_record(soundInfo);
        printf("soundInfo bufferSize: %d\n", soundInfo->bufferSize);
        cache_write(cache, soundInfo->buffer, soundInfo->bufferSize);
    }
}

int main() {
    struct Cache *cache = cache_new();

    pthread_t p1;
    pthread_create(&p1, NULL, record_and_write_cache, (void*)cache);

    read_cache_and_send(cache);

    cache_free(cache);
    
    printf("%s\n", "main end");
    return 0;
}

