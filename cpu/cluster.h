#ifndef __CLUSTER__
#define __CLUSTER__
#include "bus/slave.h"
#include "cpu/cpu.h"
#include "dev/device.h"

class Cluster : public Device, public Slave
{
    CPU *cores[8];
    pBus bus;
    virtual void _init();

public:
    Cluster();
    ~Cluster();
    virtual void run();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    void add(const uint8_t &num, CPU *core);
    void bus_connect(pBus bus);
};

#endif
