#include <rtthread.h>
#include <stdio.h>
#include <math.h>
#include "vs1003.h"
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

static int pcmAddWavHeader(uint8_t *p, int channels, int bits, int sample_rate, int fsize)
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

#define DOTS 4
#define PI 3.14159265358979323825
const static uint16_t freq_dot[DOTS] = {1800, 2400,3000,3600};
void generate_pcm_wav(uint8_t sig[DOTS])
{

    int fs = 8000;
    uint8_t *buffer = rt_malloc(2 * 8192 + 44);
    pcmAddWavHeader(buffer, 1, 16, 8000, 8192);
    buffer = buffer + 44;

    int ctr = 0;
    for (int i = 0; i < DOTS; i++)
        if (sig[i])
            ctr++;
    if (ctr == 0)
    {
        rt_thread_mdelay(1000.0 * 8192.0 / (float)fs);
        return;
    }
    int16_t val =0;
    for (int n = 0; n < 8192; n++)
    {
        for (int i = 0; i < DOTS; i++)
        {
            if (sig[i])
            {
                val += (float)0x7fff*(sin(2.0 * (float)PI * (float)n * (float)freq_dot[i] / (float)fs))/ (double)ctr;
            }

        }
        *(buffer + 2 * n) = val & 0xff;
        *(buffer + 2 * n + 1) = val >> 8;
        val=0;
    }
    vs1003_play_mode_init();
    vs1003_play_chunk(buffer-44,2 * 8192 + 44);
    rt_free(buffer-44);

}

int synth_play(int argc,char* argv[])
{
    uint8_t sig[4];
    if(argc==5){
        for(int i=0;i<4;i++){
            sig[i]=(*argv[1+i]=='1');
        }
        generate_pcm_wav(sig);
    }
}
MSH_CMD_EXPORT(synth_play,synth);