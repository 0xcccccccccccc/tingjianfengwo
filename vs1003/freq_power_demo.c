#include "vs1003.h"
#include "adpcm-xq/adpcm-lib.h"
#include "c-goertzel/goertzel-dtmf.h"
#include "c-goertzel/goertzel.h"
#include "../WaveChannel/kiss_fft/kiss_fft.h"
#include <math.h>
#include "port.h"

struct goertzel_dtmf_state dtmf;

uint16_t convert(int16_t in) {
    return (uint16_t) 32768 + (uint16_t) in;
}


static const uint16_t freq_ndot[4]={230,307,384,461};
static const uint16_t ndots=10;

FILE *f=NULL;
void adpcm_decode(uint8_t *buffer,uint32_t buffersize){
    
    int samples_n=1017;
    int16_t *pcm_buffer=(int16_t *)rt_malloc(samples_n*sizeof(int16_t));
    
    memset(pcm_buffer,0,samples_n*sizeof(int16_t));
    adpcm_decode_block(pcm_buffer,buffer,512,1);
    // float sum=0.0;
    // for(int i=0;i<buffersize*2;i++){
    //         sum+=fabsf(pcm_buffer[i]);
        
    // }
    // rt_kprintf("%f\n",sum/1024.0/32767.0);
    printf("buffer_size:%d",buffersize);
    for(int i=0;i<samples_n;i++){
        //radio_rx_decode_realtime(((float)(pcm_buffer[i])/32767.0));
         fprintf(f,"%d\n",pcm_buffer[i]);
    }
    rt_free(pcm_buffer);
}

void adpcm_goertzel_decode(uint8_t *buffer,uint32_t buffersize)
{
    struct goertzel_state s1,s2,s3,s4;
    
    
    int16_t *pcm_buffer=(int16_t *)rt_malloc(buffersize*4);
    double samples_n=buffersize*2;
    adpcm_decode_block(pcm_buffer,buffer,buffersize,1);
    goertzel_init(&s1,8000,600,1024);
    goertzel_init(&s2,8000,1200,1024);
    goertzel_init(&s3,8000,1800,1024);
    goertzel_init(&s4,8000,2400,1024);

    for (size_t s = 0; s < samples_n; s++)
    {
        goertzel_process_sample_pcm16(&s1,convert(pcm_buffer[s]));
        goertzel_process_sample_pcm16(&s2,convert(pcm_buffer[s]));
        goertzel_process_sample_pcm16(&s3,convert(pcm_buffer[s]));
        goertzel_process_sample_pcm16(&s4,convert(pcm_buffer[s]));
    }
    rt_kprintf("s1:%ld\t",(int)log10(1+goertzel_get_squared_magnitude(&s1)));
    rt_kprintf("s2:%ld\t",(int)log10(1+goertzel_get_squared_magnitude(&s2)));
    rt_kprintf("s3:%ld\t",(int)log10(1+goertzel_get_squared_magnitude(&s3)));
    rt_kprintf("s4:%ld\n\n",(int)log10(1+goertzel_get_squared_magnitude(&s4)));

    goertzel_reset(&s1);
    goertzel_reset(&s2);
    goertzel_reset(&s3);
    goertzel_reset(&s4);
    

}
void adpcm_dtmf_decode(uint8_t *buffer,uint32_t buffersize)
{
    int16_t *pcm_buffer=(int16_t *)rt_malloc(buffersize*4);
    double samples_n=buffersize*2;
    adpcm_decode_block(pcm_buffer,buffer,buffersize,1);
    goertzel_dtmf_init(&dtmf, 8000, samples_n);
    float psum=0;
    for (size_t s = 0; s < samples_n; s++)
    {
        goertzel_dtmf_add_sample_pcm16(&dtmf, pcm_buffer[s]);
        psum+=(float)(pcm_buffer[s]+32768)/(float)(65535);
    }

    //rt_kprintf("p:%d\n",(long)psum);
    //if(psum>590.0)
    rt_kprintf("code:%c\n",goertzel_dtmf_detect(&dtmf,3000000000, 3000000000));

    rt_free(pcm_buffer);
}


int freq_power_demo(int argc,char *argv[])
{
    f=fopen("out.txt","wb");
    rt_thread_t thread=vs1003_start_record(adpcm_decode);
    rt_thread_mdelay(10000);
    rt_thread_delete(thread);
    fclose(f);

}
MSH_CMD_EXPORT(freq_power_demo,freqdemo)
#include "../radio/common/signal_t/signal_t.h"
#include "../radio/common/package_t/package_t.h"
#include "../radio/common/config.h"
#include "../radio/common/wav_helper/wav_helper.h"
#include "../radio/port_interfaces/ls2k/port_interfaces.h"
#define PACKAGE_TX_LENGTH (TONE_LENGTH+TONE_SPACING_LENGTH)*8*4
int radio_tx_package(int argc,char *argv[]){
    if(argc==4){
        interface_station_init();
        uint16_t *buffer=rt_malloc(PACKAGE_TX_LENGTH * sizeof(int16_t) + 22);
        int channel=strtoul(argv[1], RT_NULL, 0);
        uint8_t d1=strtoul(argv[2], RT_NULL, 0);
        uint8_t d2=strtoul(argv[3], RT_NULL, 0);
        SIGNAL signal=package_to_signal(channel,package_new(d1,d2));
        signal_broadcast_multiply_to(32767.0,signal);
        signal_write_pcm16_to_buffer(buffer,signal,22);
        write_wav_header_to_buffer(buffer, 1, 16, 8000, PACKAGE_TX_LENGTH);
        interface_play_pcm_sync(buffer, PACKAGE_TX_LENGTH * sizeof(int16_t));
        signal_del(signal);
        rt_free(buffer);
        
    }

}
MSH_CMD_EXPORT(radio_tx_package,radio_tx_package)