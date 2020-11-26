#ifndef __CLINT__
#define __CLINT__
#include "bus/slave.h"
#include "dev/device.h"
#include "main/riscv_soc_def.h"

// Core Local Interrupt Controller
class CLINT : public Device, public Slave
{
    virtual void _init();
    uint64_t *ip[CORE_NUM];
    uint64_t timecmp[CORE_NUM];
    uint64_t time;

public:
    CLINT();
    ~CLINT();
    virtual void run();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    void set_ip(const uint8_t &id, uint64_t *ip);
    const uint64_t *get_time();
};


#endif
