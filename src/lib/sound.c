//#include <stdio.h>
#include <alsa/asoundlib.h>
#include "sound.h"

struct SoundInfo* sound_info_new() {
    struct SoundInfo *info = (struct SoundInfo*)malloc(sizeof(struct SoundInfo));
    int rc;
    int dir = 0;

    info->channels = 2; 
    info->frequency = 44100;
    info->bit = 16;
    rc=snd_pcm_open(&(info->handle), "default", SND_PCM_STREAM_PLAYBACK, 0);
    if(rc<0) {
        perror("\nopen PCM device failed:");
        exit(1);
    }

    //分配params结构体
    snd_pcm_hw_params_alloca(&(info->params)); 
    if(rc<0) {
        perror("\nsnd_pcm_hw_params_alloca:");
        exit(1);
    }

    //初始化params
    rc=snd_pcm_hw_params_any(info->handle, info->params);
    if(rc<0) {
        perror("\nsnd_pcm_hw_params_any:");
        exit(1);
    }

    //初始化访问权限
    rc=snd_pcm_hw_params_set_access(info->handle, info->params, SND_PCM_ACCESS_RW_INTERLEAVED); 
    if(rc<0) {
        perror("\nsed_pcm_hw_set_access:");
        exit(1);
    }

    //采样位数
    switch(info->bit/8) {
        case 1:
            snd_pcm_hw_params_set_format(info->handle, info->params, SND_PCM_FORMAT_U8);
            break;
        case 2:
            snd_pcm_hw_params_set_format(info->handle, info->params, SND_PCM_FORMAT_S16_LE);
            break;
        case 3:
            snd_pcm_hw_params_set_format(info->handle, info->params, SND_PCM_FORMAT_S24_LE);
            break;
    }

    //设置声道,1表示单声道，2表示立体声
    rc=snd_pcm_hw_params_set_channels(info->handle, info->params, info->channels); 
    if(rc<0) {
        perror("\nsnd_pcm_hw_params_set_channels:");
        exit(1);
    }

    //设置频率
    unsigned int val = info->frequency;
    rc=snd_pcm_hw_params_set_rate_near(info->handle, info->params, &val, &dir); 
    if(rc<0) {
        perror("\nsnd_pcm_hw_params_set_rate_near:");
        exit(1);
    }

    rc = snd_pcm_hw_params(info->handle, info->params);
    if(rc<0) {
        perror("\nsnd_pcm_hw_params: ");
        exit(1);
    }

    info->frames = 32;
    snd_pcm_hw_params_set_period_size_near(info->handle, info->params, &(info->frames), &dir);
    rc = snd_pcm_hw_params(info->handle, info->params);

    //获取周期长度
    rc=snd_pcm_hw_params_get_period_size(info->params, &(info->frames), &dir);
    if(rc<0) {
        perror("\nsnd_pcm_hw_params_get_period_size:");
        exit(1);
    }

    info->bufferSize = info->frames * 4;
    //test
    info->bufferSize = 4096;

    info->buffer = (char*)malloc(info->bufferSize);

    return info;
}

void sound_info_record(struct SoundInfo *info) {
    int rc;
    rc = snd_pcm_readi(info->handle, info->buffer, info->frames);
    if(rc == -EPIPE){
        fprintf(stderr, "overrun occurred\n");
        printf("%s, rc = %d \n", "overrun occurred", rc);
        snd_pcm_prepare(info->handle);
    }
    else if(rc<0) {
        fprintf(stderr, "error from read; %s\n", snd_strerror(rc));
        printf("%s, rc = %d \n", "error read", rc);
    }
    else if(rc != (int)info->frames){
        fprintf(stderr, "short read, read %d frames\n", rc);
        printf("%s, rc = %d \n", "short read", rc);
    } else {
        printf("%s, rc = %d \n, buffer[100] = %c ", "run normal", rc, info->buffer[100]);
    }

    //test
/*
    info->buffer[0] = 'a';
    info->buffer[1] = 'b';
    info->buffer[2] = 'c';
    info->buffer[4095] = 'k';
*/
    printf("-hi-----%d-------------------- %c ----\n", rc, info->buffer[0]);
    info->bufferSize = rc;
}

void sound_info_play(struct SoundInfo *info) {
   
    int rc; 
    while((rc = snd_pcm_writei(info->handle, info->buffer, info->frames)) < 0) {
        usleep(2000);
        if (rc == -EPIPE) {
            fprintf(stderr, "underrun occurred\n");
            //完成硬件参数设置，使设备准备好
            snd_pcm_prepare(info->handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei: %s\n",
            snd_strerror(rc));
        }
   }
}

void sound_info_free(struct SoundInfo *info) {
    
    snd_pcm_drain(info->handle);
    snd_pcm_close(info->handle);
    free(info->buffer);
    free(info);
}

