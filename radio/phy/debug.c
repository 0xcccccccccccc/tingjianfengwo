//
// Created by ma on 2020/11/10.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "debug.h"
#include "radio_tx.h"
#include "../common/wav_helper/wav_helper.h"
#include "../common/config.h"

void debug_signal_save_pcm_wav(char* path,SIGNAL signal){
    FILE *f=fopen(path,"wb");
    write_wav_header_to_file(f,1,16,FS,2*(signal->length));
    SIGNAL normalized_signal=signal_normalize(signal);
    signal_broadcast_add_to(-0.5,normalized_signal);
    signal_broadcast_multiply_to(2.0,normalized_signal);
    signal_broadcast_multiply_to(32767.0,normalized_signal);

    for (int i = 0; i < signal->length; i++)
    {
        int16_t d=(int16_t)(round(normalized_signal->data[i]));
        fputc(d&0xff,f);
        fputc(d>>8,f);
    }
    fclose(f);
    signal_del(normalized_signal);
}
void debug_signal_print(SIGNAL signal){
    for(int32_t i=0;i<signal->length;i++){
        printf("INDEX%d:\t%f\n",i,signal_get(signal,i));
    }
}
void debug_bit_stream_print(BIT_STREAM bit_stream){
    for(int32_t i=0;i<bit_stream->length;i++){
        printf("INDEX%d:\t%d\n",i,bit_stream_get(bit_stream,i));
    }
}
#define RADIO_USING_LIBSNDFILE
#ifdef RADIO_USING_LIBSNDFILE
#include <sndfile.h>
SIGNAL debug_signal_load_pcm_wav(char* path){
struct SF_INFO sfInfo={0};
SNDFILE *sf=sf_open(path,SFM_READ,&sfInfo);
SIGNAL signal=signal_new(sfInfo.frames,0.0,SIGNAL_BOUNDARY_TYPE_FIXED);
sf_read_float(sf,signal->data,signal->length);
sf_close(sf);
return signal;

}
#endif