//
// Created by ma on 2020/11/12.
//

#include "wav_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int write_wav_header_to_buffer(uint8_t *p, int channels, int bits, int sample_rate, int fsize)
{
    head_data_t pcm2wavHEAD;
    memcpy(pcm2wavHEAD.riffType, "RIFF", strlen("RIFF"));
    memcpy(pcm2wavHEAD.wavType, "WAVE", strlen("WAVE"));
    pcm2wavHEAD.riffSize = 36 + fsize;
    pcm2wavHEAD.sampleRate = sample_rate;
    pcm2wavHEAD.bitsPerSample = bits;
    memcpy(pcm2wavHEAD.formatType, "fmt ", strlen("fmt "));
    pcm2wavHEAD.formatSize = 16;
    pcm2wavHEAD.numChannels = channels;
    pcm2wavHEAD.blockAlign = channels * bits / 8;
    pcm2wavHEAD.compressionCode = 1;
    pcm2wavHEAD.bytesPerSecond = pcm2wavHEAD.sampleRate * pcm2wavHEAD.blockAlign;
    memcpy(pcm2wavHEAD.dataType, "data", strlen("data"));
    pcm2wavHEAD.dataSize = fsize;
    for (int i = 0; i < 44; i++)
    {
        *(p + i) = *(((uint8_t *)(&pcm2wavHEAD)) + i);
    }
}

int write_wav_header_to_file(FILE* f, int channels, int bits, int sample_rate, int fsize)
{
    head_data_t pcm2wavHEAD;
    memcpy(pcm2wavHEAD.riffType, "RIFF", strlen("RIFF"));
    memcpy(pcm2wavHEAD.wavType, "WAVE", strlen("WAVE"));
    pcm2wavHEAD.riffSize = 36 + fsize;
    pcm2wavHEAD.sampleRate = sample_rate;
    pcm2wavHEAD.bitsPerSample = bits;
    memcpy(pcm2wavHEAD.formatType, "fmt ", strlen("fmt "));
    pcm2wavHEAD.formatSize = 16;
    pcm2wavHEAD.numChannels = channels;
    pcm2wavHEAD.blockAlign = channels * bits / 8;
    pcm2wavHEAD.compressionCode = 1;
    pcm2wavHEAD.bytesPerSecond = pcm2wavHEAD.sampleRate * pcm2wavHEAD.blockAlign;
    memcpy(pcm2wavHEAD.dataType, "data", strlen("data"));
    pcm2wavHEAD.dataSize = fsize;
    for (int i = 0; i < 44; i++)
    {
        fputc(*(((uint8_t *)(&pcm2wavHEAD)) + i),f);
    }
}

