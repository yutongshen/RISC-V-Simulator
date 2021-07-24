#include "dev/irqsrc.h"

IRQSource::IRQSource(int32_t irq_id, PLIC *plic) : irq_id(irq_id), plic(plic) {}

IRQSource::~IRQSource() {}
