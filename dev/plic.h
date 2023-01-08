#ifndef __PLIC__
#define __PLIC__
#include <pthread.h>
#include "bus/slave.h"
#include "dev/device.h"

#define TARGET_NUM 8 * 2
#define INT_NUM 32
#define INT_REG_NUM ((INT_NUM >> 5) + !!(INT_NUM & 0x1f))

#define DEV_RISING_IRQ()                    \
    do {                                    \
        pthread_mutex_lock(&plic->mutex);   \
        plic->set_pending(irq_id, 1);       \
        pthread_mutex_unlock(&plic->mutex); \
    } while (0)

#define DEV_FALLING_IRQ()                   \
    do {                                    \
        pthread_mutex_lock(&plic->mutex);   \
        plic->set_pending(irq_id, 0);       \
        pthread_mutex_unlock(&plic->mutex); \
    } while (0)

class PLIC : public Device, public Slave
{
    virtual void _init();
    void _update();
    bool csr_connect[TARGET_NUM];
    uint64_t *irqdst[TARGET_NUM];
    uint8_t irqoffset[TARGET_NUM];
    uint32_t prior[INT_NUM];
    uint32_t pending[INT_REG_NUM];
    uint32_t dispatch[INT_NUM];
    uint32_t enable[TARGET_NUM * INT_REG_NUM];
    uint32_t int_id[TARGET_NUM];
    uint32_t threshold[TARGET_NUM];

public:
    PLIC();
    ~PLIC();
    virtual void run();
    virtual void stop();
    virtual void single_step();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    void bind_irqdst(uint64_t *dst, uint8_t offset, uint8_t target);
    uint32_t *get_pending();
    void set_pending(int32_t irq_id, uint8_t value);
    pthread_mutex_t mutex;
};


#endif
