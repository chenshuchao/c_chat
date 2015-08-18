#include <pthread.h>
#include "lib/utils.h"
#include "lib/sound.h"
#include "lib/cache.h"
#include "server.h"

// 接收数据并写入缓存
void* recv_and_write_cache(void *cache1) {
    printf("%s", "recv_and_write_cache\n");

    struct Cache *cache = (struct Cache*)cache1;
    int serSock = server_init(5011);
    int bufferSize = 4096;
    int recvSize = 0;

    while(1) {
        char *buffer = server_recv(serSock, bufferSize, &recvSize);
        if(recvSize) {
            cache_write(cache, buffer, recvSize);
        }
    }
}

// 从缓存读取数据并写入声卡
void read_cache_and_play(struct Cache *cache) {
    printf("%s", "read_cache_and_play\n");

    int readSize;
    char *buffer;
    struct SoundInfo *soundInfo = sound_info_play_new();

    while(1) {
        
        if(!cache_read(cache, &buffer, &readSize) || !is_cache_buffer_enough(cache)) continue;
        bzero(soundInfo->buffer, soundInfo->bufferSize);
        buffercpy(soundInfo->buffer, buffer, readSize);
        sound_info_play(soundInfo);
    
        free(buffer);
        buffer = NULL;
    }

    sound_info_free(soundInfo);

    return;
}

int main() {
    printf("%s", "starting...\n");

    struct Cache *cache = cache_new();

    pthread_t p1;
    pthread_create(&p1, NULL, recv_and_write_cache, (void*)cache);

    read_cache_and_play(cache);

    cache_free(cache);

    return 0;
}

