//
// Created by ma on 2020/11/10.
//

#include "radio_rx.h"
#include "../common/signal_t/signal_t.h"
#include "radio_tx.h"
#include "../common/config.h"

BIT_STREAM radio_rx_binarize(SIGNAL signal,RADIO_SIGNAL_SCALAR_TYPE threshold){
    BIT_STREAM bit_stream=bit_stream_new(signal->length,BIT_STREAM_BOUNDARY_FIXED);
    for(int32_t i=0;i<signal->length;i++){
        bit_stream_set(bit_stream,i,(uint8_t)(signal_get(signal,i)>threshold));
    }
    return bit_stream;
}

int32_t radio_rx_search_sync_header(BIT_STREAM bit_stream,BIT_STREAM header,uint8_t slice){


    BIT_STREAM bit_stream_slice=bit_stream_new(bit_stream->length*(slice?RADIO_RX_SEARCH_SYNC_HEADER_SCALE:1),BIT_STREAM_BOUNDARY_ZERO_PADDING);
    for(int i=0;i<bit_stream_slice->length;i++){
        bit_stream_set(bit_stream_slice,i,bit_stream_get(bit_stream,i));
    }
    SIGNAL signal=bit_stream_correlation(bit_stream_slice,header,RADIO_RX_SEARCH_SYNC_HEADER_CORRELATION_STEP);

    int32_t pos=signal_argmax(signal)+7*TONE_SPACING_LENGTH+7*TONE_LENGTH;

    signal_del(signal);
    //bit_stream_del(header);
    bit_stream_del(bit_stream_slice);

    return pos;
}

