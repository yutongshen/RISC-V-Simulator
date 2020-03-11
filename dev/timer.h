#ifndef __TIMER__
#define __TIMER__
#include "bus/slave.h"
#include "dev/device.h"


#define MIP_BASE 0x0000
#define TIMECMP_BASE 0x0800
#define TIME_BASE 0x0ff8


class Timer : public Device, public Slave
{
    virtual void _init();
    bool is_mip_set;
    uint64_t *ip;
    uint64_t timecmp;
    uint64_t time;

public:
    Timer();
    ~Timer();
    virtual void run();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    void set_ip(uint64_t *ip);
};


#endif
