#ifndef __IRQSRC__
#define __IRQSRC__

#include "bus/bus.h"
#include "dev/plic.h"

class IRQSource
{
protected:
    int32_t irq_id;
    PLIC *plic;

public:
    IRQSource(int32_t irq_id, PLIC *plic);
    ~IRQSource();
};

#endif
