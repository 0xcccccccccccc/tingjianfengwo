//
// Created by ma on 2020/11/10.
//

#include "../common/signal_t/signal_t.h"

#ifndef RADIO_DEBUG_H
#define RADIO_DEBUG_H
void debug_signal_save_pcm_wav(char* path,SIGNAL signal);
SIGNAL debug_signal_load_pcm_wav(char* path);
void debug_signal_print(SIGNAL signal);
#endif //RADIO_DEBUG_H
