#include <stdio.h>
// #include "util.h"
#define TYPE_FIXED 0x0
#define TYPE_INCR  0x1
#define TYPE_SPI   0x2
#define TYPE_CONST 0x3
#define SIZE_BYTE  0x0
#define SIZE_HWORD 0x1
#define SIZE_WORD  0x2
#define SIZE_DWORD 0x3

int main(int argc, char **argv)
{
    float pi = 3.1415926;
    printf("hello world!!\npi = %f\n", pi);
    printf("MTIP TEST PASS\n", pi);
    return 0;
}

void dma_test(void)
{
    char buf1[512], buf2[512];
    int i, src_size, dest_size, src_offset, dest_offset;

    /* TM_PRINT="DMA test begin" */
    for (i = 0; i < 512; ++i) {
        buf1[i] = i;
        // /* TM_PRINT="buf1[%02x] = %02x", i, buf1[i] */
    }

    for (src_size = 0; src_size <= SIZE_WORD; ++src_size) {
        for (dest_size = 0; dest_size <= SIZE_WORD; ++dest_size) {
            for (src_offset = 0; src_offset < 4; ++src_offset) {
                for (dest_offset = 0; dest_offset < 4; ++dest_offset) {
                    /* TM_PRINT="SRC_SIZE: %d, DEST_SIZE: %d, SRC_OFFSET: %d, DEST_OFFSET: %d,", src_size, dest_size, src_offset, dest_offset */
                    *SPI_RG_DMA_SRC_32P  = src_offset + (unsigned int) buf1;
                    *SPI_RG_DMA_DEST_32P = dest_offset + (unsigned int) buf2;
                    *SPI_RG_DMA_LEN_32P  = 256;
                    *SPI_RG_DMA_CON_32P  = dest_size << 10 | // dest size
                                           src_size  << 8  | // src size
                                           TYPE_INCR << 6  | // dest type
                                           TYPE_INCR << 4  | // src type
                                           1; // enable

                    while (*SPI_RG_DMA_CON_32P >> 31);
                    for (i = 0; i < 256; ++i) {
                        if (buf2[i+dest_offset] != ((i + src_offset) & 0xff)) {
                            /* TM_PRINT="[ERROR] buf2[%02x] = %02x, expect = %02x", i+dest_offset, buf2[i+dest_offset], ((i + src_offset) & 0xff) */
                            return;
                        }
                        // /* TM_PRINT="buf2[%02x] = %02x", i+dest_offset, buf2[i+dest_offset] */
                    }
                }
            }
        }
    }

}
