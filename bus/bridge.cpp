#include "bridge.h"

Bridge::Bridge(uint32_t size) : bus(0), Slave(size) {}

Bridge::~Bridge() {}

bool Bridge::write(const Addr &addr,
                    const DataType &data_type,
                    const uint64_t &wdata)
{
    if (bus)
        return bus->write(addr, data_type, wdata);
    return 0;
}

bool Bridge::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    if (bus)
        return bus->read(addr, data_type, rdata);
    return 0;
}

void Bridge::bus_connect(pBus bus)
{
    this->bus = bus;
}
