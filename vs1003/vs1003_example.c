#include <stdio.h>
#include <drv_spi.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/pin.h>
#include "vs1003.h"

#include <drivers/pin.h>
static int amplifer_enable()
{
  rt_pin_mode(21,0);
  rt_pin_write(21,0);

}
static int amplifer_disable()
{
   rt_pin_mode(21,0);
   rt_pin_write(21,1);
}

static unsigned char header[] = {
    0x52, 0x49, 0x46, 0x46, 0x1c, 0x10, 0x00, 0x00,
    0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,
    0x14, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00,
    0x40, 0x1f, 0x00, 0x00, 0x75, 0x12, 0x00, 0x00,
    0x00, 0x01, 0x04, 0x00, 0x02, 0x00, 0xf9, 0x01,
    0x66, 0x61, 0x63, 0x74, 0x04, 0x00, 0x00, 0x00,
    0x5c, 0x1f, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61,
    0xe8, 0x0f, 0x00, 0x00};
union bit_converter_t
{
    unsigned char b[4];
    uint32_t d;
} bit_converter;

static void wrtie_header(FILE *f, uint32_t n, uint32_t fs)
{
    int F = n * VS1003_ADPCM_BATCH_SIZE + 52;
    int R = fs;
    int B = fs * VS1003_ADPCM_BATCH_SIZE / 505;
    int S = n * 505;
    int D = n * VS1003_ADPCM_BATCH_SIZE;

    bit_converter.d = F;
    for (int i = 0; i < 4; i++)
        header[4 + i] = bit_converter.b[i];

    bit_converter.d = R;
    for (int i = 0; i < 4; i++)
        header[24 + i] = bit_converter.b[i];

    bit_converter.d = B;
    for (int i = 0; i < 4; i++)
        header[28 + i] = bit_converter.b[i];

    bit_converter.d = S;
    for (int i = 0; i < 4; i++)
        header[48 + i] = bit_converter.b[i];

    bit_converter.d = D;
    for (int i = 0; i < 4; i++)
        header[56 + i] = bit_converter.b[i];

    for (int i = 0; i < sizeof(header); i++)
    {
        fputc(header[i], f);
    }
}

unsigned char db[VS1003_ADPCM_BATCH_SIZE * 2];
void vs1003_record(int argc, char *argv[]) // 0.128 s/CHUNK @ fs=8000
{
    if (argc == 3)
    {
        uint16_t t, w;
        amplifer_disable();
        vs1003_record_mode_init();
        rt_kprintf("fopen:%s\n", argv[1]);
        FILE *f = fopen(argv[1], "wb");
        int len = strtoul(argv[2], NULL, 0);
        rt_kprintf("len:%d\n", len);
        wrtie_header(f, 4*len, 8000);

        while (len)
        {
            //rt_kprintf("batch:%d\n", __i);
            t = read_register(SCI_HDAT1);
            while (t >= VS1003_ADPCM_BATCH_SIZE /*&& t < 896*/)
            {
                if (t >= 896)
                {
                    rt_kprintf("Buffer Overflow!\n", t);
                    break;
                }

                for (int i = 0; i < VS1003_ADPCM_BATCH_SIZE * 2; i = i + 2)
                {
                    w = read_register(SCI_HDAT0);
                    db[i] = w >> 8;
                    db[i + 1] = w & 0xFF;
                }

                for (int i = 0; i < VS1003_ADPCM_BATCH_SIZE * 2; i++)
                {
                    fputc(db[i], f);
                }

                len--;
                if (len % 10 == 0)
                    rt_kprintf("len: %d\n", len);
                t -= VS1003_ADPCM_BATCH_SIZE;
            }
        }
        fclose(f);
        vs1003_reset();
        
    }
    else
    {
        printf("vs1003_record filename chunk_length\n");
    }
}
MSH_CMD_EXPORT(vs1003_record, record)

#include "hellomp3.h"

int vs1003_play_mode_test(int argc, char *argv[])
{
    amplifer_enable();
    vs1003_play_mode_init();
    vs1003_set_volume(0x00);
    while (1)
    {
        vs1003_play_chunk(HelloMP3, sizeof(HelloMP3));
        rt_thread_mdelay(500);
        rt_kprintf("hello.mp3\n");
    };
    amplifer_disable();
}
MSH_CMD_EXPORT(vs1003_play_mode_test, play);

int vs1003_play_once(int argc, char *argv[])
{
    amplifer_enable();
    vs1003_play_mode_init();
    vs1003_set_volume(0x00);

    vs1003_play_chunk(HelloMP3, sizeof(HelloMP3));
    rt_thread_mdelay(500);
    rt_kprintf("hello.mp3\n");
    amplifer_disable();
}
MSH_CMD_EXPORT(vs1003_play_once, play);

int vs1003_play_wav(int argc, char *argv[])
{
    if (argc == 2)
    {
        amplifer_enable();
        FILE *f = fopen(argv[1], "rb");
        fseek(f, 0, SEEK_END);
        long flen = ftell(f);
        fseek(f, 0, SEEK_SET);
        rt_kprintf("flen:%d\n", flen);

        unsigned char *fbuffer = (unsigned char *)malloc(flen);
        fread(fbuffer, 1, flen, f);

        vs1003_play_mode_init();
        vs1003_set_volume(0x00);

        vs1003_play_chunk(fbuffer, flen);

        //rt_thread_mdelay(500);
        rt_kprintf(argv[1]);
        rt_kprintf("\n");
        free(fbuffer);
        fclose(f);
        amplifer_disable();
    }
    else
    {
        rt_kprintf("usage: vs1003_play_wav filename(mp3,wav,wma,...,etc.)");
    }
}
MSH_CMD_EXPORT(vs1003_play_wav, play);


