//
// Created by ma on 2020/11/12.
//

#ifndef RADIO_WAV_HELPER_H
#define RADIO_WAV_HELPER_H
#include <stdio.h>
#include <stdint.h>
typedef struct
{
    char riffType[4];               //4byte,资源交换文件标志:RIFF
    unsigned int riffSize;          //4byte,从下个地址到文件结尾的总字节数
    char wavType[4];                //4byte,wav文件标志:WAVE
    char formatType[4];             //4byte,波形文件标志:FMT(最后一位空格符)
    unsigned int formatSize;        //4byte,音频属性(compressionCode,numChannels,sampleRate,bytesPerSecond,blockAlign,bitsPerSample)所占字节数
    unsigned short compressionCode; //2byte,格式种类(1-线性pcm-WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM)
    unsigned short numChannels;     //2byte,通道数
    unsigned int sampleRate;        //4byte,采样率
    unsigned int bytesPerSecond;    //4byte,传输速率
    unsigned short blockAlign;      //2byte,数据块的对齐，即DATA数据块长度
    unsigned short bitsPerSample;   //2byte,采样精度-PCM位宽
    char dataType[4];               //4byte,数据标志:data
    unsigned int dataSize;          //4byte,从下个地址到文件结尾的总字节数，即除了wav header以外的pcm data length
} head_data_t;
int write_wav_header_to_file(FILE* f, int channels, int bits, int sample_rate, int fsize);
int write_wav_header_to_buffer(uint8_t *p, int channels, int bits, int sample_rate, int fsize);
#endif //RADIO_WAV_HELPER_H
