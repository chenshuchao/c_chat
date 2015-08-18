#include <pthread.h>
#include "lib/utils.h"
#include "lib/sound.h"
#include "lib/cache.h"
#include "client.h"

// 读取缓存并发送
void read_cache_and_send(struct Cache *cache) {
    printf("%s", "read_cache_and_send\n");

    char *buffer;
    int recvSize = 0;
    struct Client *client = client_init("127.0.0.1", 5011);

    while(1) {

        if(!cache_read(cache, &buffer, &recvSize)) continue;
        if(recvSize) {
            client_send(client, buffer, recvSize);
        }
        free(buffer);
        buffer = NULL;
    }
}

// 从声卡采集信息并写入缓存结构
void* record_and_write_cache(void *cache1) {
    printf("%s", "record_and_write_cache\n");

    struct Cache *cache = (struct Cache*)cache1;
    struct SoundInfo *soundInfo = sound_info_record_new();

    while(1) {
        bzero(soundInfo->buffer, soundInfo->bufferSize);
        sound_info_record(soundInfo);
        cache_write(cache, soundInfo->buffer, soundInfo->bufferSize);
    }

    sound_info_free(soundInfo);
}

int main() {
    struct Cache *cache = cache_new();
    
    struct SoundInfo *soundInfo = sound_info_record_new();

    pthread_t p1;
    pthread_create(&p1, NULL, record_and_write_cache, (void*)cache);

    read_cache_and_send(cache);

    cache_free(cache);
    
    return 0;
}

