#ifndef __BRIDGE__
#define __BRIDGE__
#include "bus/bus.h"
#include "bus/slave.h"

class Bridge : public Slave
{
    pBus bus;

public:
    Bridge(uint32_t size);
    ~Bridge();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    void bus_connect(pBus bus);
};

#endif
