#ifndef __FINISHER__
#define __FINISHER__

#include <stdint.h>

#define FINISHER_PASS 0x5555
#define FINISHER_FAIL 0x3333

#define RG_FINISHER_TXFIFO 0x00
#define RG_FINISHER_RXFIFO 0x01
#define RG_FINISHER_TXCTRL 0x02
#define RG_FINISHER_RXCTRL 0x03
#define RG_FINISHER_IE 0x04
#define RG_FINISHER_IP 0x05
#define RG_FINISHER_DIV 0x06

typedef struct {
    uint32_t find;
    uint32_t *addr;
} finisher_scan;

#endif
