//
// Created by ma on 2020/11/10.
//

#ifndef RADIO_RADIO_RX_H
#define RADIO_RADIO_RX_H

#include "../common/bit_stream_t/bit_stream_t.h"


#define RADIO_RX_BINARIZE_THRESHOLD 0.65
//#define RADIO_RX_SEARCH_SYNC_HEADER_CORRELATION_THRESHOLD 0.65
#define RADIO_RX_SEARCH_SYNC_HEADER_SCALE 0.40
#define RADIO_RX_SEARCH_SYNC_HEADER_CORRELATION_STEP 150
BIT_STREAM radio_rx_binarize(SIGNAL signal,RADIO_SIGNAL_SCALAR_TYPE threshold);
int32_t radio_rx_search_sync_header(BIT_STREAM bit_stream,BIT_STREAM header,uint8_t slice);

#endif //RADIO_RADIO_RX_H
