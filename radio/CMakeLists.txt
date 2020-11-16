cmake_minimum_required(VERSION 3.17)
project(radio C)

set(CMAKE_C_STANDARD 99)
aux_source_directory(common/queue QUEUE)

link_libraries(m)
link_libraries(sndfile)
link_libraries(portaudio)
set(CMAKE_C_FLAGS "$ENV{CFLAGS} -O3 -w")

add_executable(radio  phy/radio_tx.c phy/radio_tx.h phy/radio_rx.c phy/radio_rx.h phy/filters/fdacoefs_FIR_1500.h phy/filters/fdacoefs_FIR_2000.h phy/filters/fdacoefs_FIR_2500.h  ${QUEUE} common/signal_t/signal_t.c common/signal_t/signal_t.h phy/debug.c phy/debug.h common/config.h common/bit_stream_t/bit_stream_t.c common/bit_stream_t/bit_stream_t.h phy/filters/tmwtypes.h common/config.h common/hamming/hamming.c common/hamming/hamming.h common/byte_t/byte_t.c common/byte_t/byte_t.h common/package_t/package_t.c common/package_t/package_t.h link/station/station_entry.c link/user/user_entry.c common/wav_helper/wav_helper.c common/wav_helper/wav_helper.h port_interfaces/x86_64_linux/port_interfaces.c port_interfaces/x86_64_linux/port_interfaces.h port_interfaces/x86_64_linux/port_linux_main.c link/station/station_entry.h link/user/user_entry.h tools/gen_signal.c)