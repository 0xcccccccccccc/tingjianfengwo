//
// Created by ma on 2020/11/13.
//

#include "port_interfaces.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "portaudio.h"

#define FRAMES_PER_BUFFER   (512)
#include "../../phy/debug.h"
#include "../../common/config.h"
//typedef struct
//{
//    int16_t   *data;
//    uint32_t length;
//    uint32_t ptr;
//} callback_data_s;
void interface_delay_ms(float ms){
    //printf("delay:%f ms\n",ms);
    usleep((long)ms*1000.0);

}

//static
//int
//callback
//        (const void                     *input
//                ,void                           *output
//                ,unsigned long                   frameCount
//                ,const PaStreamCallbackTimeInfo *timeInfo
//                ,PaStreamCallbackFlags           statusFlags
//                ,void                           *userData
//        )
//{
//    int16_t *out=output;
//    callback_data_s *p_data = (callback_data_s*)userData;
//
//    p_data = (callback_data_s*)userData;
//    /* clear output buffer */
//    if(p_data->ptr+frameCount<p_data->length){
//        memset(out, 0, sizeof(int16_t) * frameCount);
//        p_data->ptr+=frameCount;
//    }
//    else{
//        memset(out, 0, sizeof(int16_t) * (p_data->length-p_data->ptr));
//        printf("Complete\n");
//        return paComplete;
//    }
//
//    return paContinue;
//}

void interface_play_pcm_sync(int16_t *data, int32_t length){
    SIGNAL signal=signal_new(length,0.0,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int i=0;i<length;i++){
        signal->data[i]=data[i];
    }

    debug_signal_save_pcm_wav("play.wav",signal);
    system("aplay -D default play.wav");
    signal_del(signal);
//    double ratio=44100.0/FS;
//    int32_t new_length=length*ratio;
//    int16_t *new_data=malloc(sizeof(int16_t)*new_length);
//    for(int32_t i=0;i<new_length;i++){
//        new_data[i]=data[(int32_t)(i/ratio)];
//    }
//
//    PaError error;
//    PaStream *stream;
//    callback_data_s callback_data;
//    callback_data.data=new_data;
//    callback_data.length=new_length;
//    callback_data.ptr=0;
//
//
//
//    error = Pa_OpenDefaultStream(&stream
//            ,0                     /* no input */
//            ,1         /* stereo out */
//            ,paInt16             /* floating point */
//            ,44100
//            ,FRAMES_PER_BUFFER
//            ,callback
//            ,&callback_data);        /* our sndfile data struct */
//    if(error != paNoError)
//    {
//        fprintf(stderr, "Problem opening Default Stream\n");
//        free(new_data);
//        return ;
//    }
//
//    /* Start the stream */
//    error = Pa_StartStream(stream);
//    if(error != paNoError)
//    {
//        fprintf(stderr, "Problem opening starting Stream\n");
//        free(new_data);
//        return ;
//    }
//
//    /* Run until EOF is reached */
//    while(Pa_IsStreamActive(stream))
//    {
//        Pa_Sleep(100);
//    }
//    error = Pa_CloseStream(stream);
//    if(error != paNoError)
//    {
//        fprintf(stderr, "Problem closing stream\n");
//        free(new_data);
//        return ;
//    }
//
//    error = Pa_Terminate();
//    if(error != paNoError)
//    {
//        fprintf(stderr, "Problem terminating\n");
//        free(new_data);
//        return ;
//    }
//    free(new_data);


}
SIGNAL interface_get_pcm_sync(double ms){
    char cmd[100];
    sprintf(cmd,"arecord -D default -r 8000 -t wav -f s16_le -s %d rec.wav",(int)((double)FS*ms/1000.0));
    system(cmd);
    return debug_signal_load_pcm_wav("rec.wav");
}
double interface_get_current_ms(){
    //return 1000.0*(double)clock()/(double)CLOCKS_PER_SEC;
    struct timespec time_start={0, 0};
    clock_gettime(CLOCK_REALTIME, &time_start);
    return (double )time_start.tv_sec*1000.0+time_start.tv_nsec/1000000.0;
    //printf("start time %llus,%llu ns\n", time_start.tv_sec, time_start.tv_nsec);
}

void interface_user_init(){

}
void interface_station_init(){
//Pa_Initialize();
}
PACKAGE interface_user_get_package(){
    return package_new(0xb0,'i');
}