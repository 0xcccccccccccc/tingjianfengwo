//
// Created by ma on 2020/11/12.
//

#include <stdio.h>
#include <stdlib.h>
#include "package_t.h"
#include "../../phy/radio_tx.h"
#include "../config.h"
#include "../../common/hamming/hamming.h"
#include "../byte_t/byte_t.h"

const static double channel_freqs[4]={CH1_FREQ,CH2_FREQ,CH3_FREQ,CH4_FREQ};
SIGNAL package_to_signal(int channel,PACKAGE package){
    uint8_t data[4];
    data[0]=SYNC_HEADER;
    data[1]=package.data[0];
    data[2]=package.data[1];
    data[3]=package.data[2];
    return radio_tx_generate_signal_from_bytes((double)channel_freqs[channel],&data,4);
}
PACKAGE package_new(uint8_t d1,uint8_t d2){
    PACKAGE package;
    package.data[0]=d1;
    package.data[1]=d2;
    package.data[2]=hamming_calculate_parity_2416(d1,d2);
    return package;
}

PACKAGE* package_new_alloc(uint8_t d1,uint8_t d2){
    PACKAGE *package=malloc(3);
    package->data[0]=d1;
    package->data[1]=d2;
    package->data[2]=hamming_calculate_parity_2416(d1,d2);
    return package;
}
// decode from a BIT_STREAM and correct automatically.

PACKAGE package_decode_from_bit_stream(BIT_STREAM bit_stream,int32_t base){
    PACKAGE package;


    package.data[0]=bit_stream_get_byte(bit_stream,base,0);
    package.data[1]=bit_stream_get_byte(bit_stream,base,1);
    package.data[2]=bit_stream_get_byte(bit_stream,base,2);

    int corrections=hamming_correct_2416(&package.data[0],&package.data[1],package.data[2]);
    if(corrections>1){
        printf("Warning: %d errors corrected.\n",corrections);
        return NULL_PACKAGE;
    }

    return package;
}

PACKAGE* package_alloc_decode_from_bit_stream(BIT_STREAM bit_stream,int32_t base){
    PACKAGE* package=malloc(3);


    package->data[0]=bit_stream_get_byte(bit_stream,base,0);
    package->data[1]=bit_stream_get_byte(bit_stream,base,1);
    package->data[2]=bit_stream_get_byte(bit_stream,base,2);

    int corrections=hamming_correct_2416(&(package->data[0]),&(package->data[1]),package->data[2]);
    if(corrections>1){
        printf("Warning: %d errors corrected.\n",corrections);
        return NULL;
    }

    return package;
}

void package_del(PACKAGE* package){
    free(package);
}

uint8_t package_is_null(PACKAGE p){
    return p.data[0]||p.data[1]||p.data[2];
}
uint8_t package_alloc_is_null(PACKAGE *p){
    return p->data[0]||p->data[1]||p->data[2];
}
uint8_t package_network_get_src(PACKAGE p){
    return bit_get(&p.data[0],6)+bit_get(&p.data[0],7)<<1;
}
uint8_t package_network_get_dst(PACKAGE p){
    return bit_get(&p.data[0],4)+bit_get(&p.data[0],5)<<1;
}