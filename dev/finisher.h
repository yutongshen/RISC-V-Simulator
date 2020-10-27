#ifndef __FINISHER__
#define __FINISHER__
#include "bus/slave.h"
#include "dev/device.h"
#include "main/riscv_soc_def.h"

#define FINISHER_PASS 0x5555
#define FINISHER_FAIL 0x3333

class Finisher : public Slave
{
    virtual void _init();
    uint32_t exit_code;

public:
    Finisher();
    ~Finisher();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    uint32_t get_exit_code();
};


#endif
