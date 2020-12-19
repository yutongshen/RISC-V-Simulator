#ifndef __HART__
#define __HART__
#include <stdint.h>

#define MSIP_BIT 3
#define MTIP_BIT 7
#define SEIP_BIT 9
#define MEIP_BIT 11

#define RG_CLINT_MSIP 0x0000
#define RG_CLINT_TIMECMP 0x1000
#define RG_CLINT_TIME 0x2ffe

#define RG_PLIC_PRIOR 0x0
#define RG_PLIC_PEND 0x400
#define RG_PLIC_ENABLE 0x800
#define RG_PLIC_PRIOR_TH 0x80000
#define RG_PLIC_INTID 0x80001

#endif
