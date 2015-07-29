#ifndef __SOUND_H__
#define __SOUND_H__
#include <alsa/asoundlib.h>

struct SoundInfo { 
    snd_pcm_t* handle; //PCI设备句柄
    snd_pcm_hw_params_t* params;//硬件信息和PCM流配置
    snd_pcm_uframes_t frames; 
    int channels;
    int frequency;
    int bit;
    char *buffer;
    int bufferSize;
};

struct SoundInfo* sound_info_new();

void sound_info_record(struct SoundInfo *info);

void sound_info_play(struct SoundInfo *info);

void sound_info_free(struct SoundInfo *info);

#endif

