#ifndef __SLAVE__
#define __SLAVE__

#include <stdint.h>
#include "bus/addr.h"
#include "bus/datatype.h"

class Bus;

typedef Bus *pBus;

class Slave
{
    static uint8_t num;
    static void _cnt_num();

protected:
    uint64_t size;
    uint8_t s_id;
    pBus p_bus;
    bool connected;
    bool _check_bound(const Addr &addr, const uint8_t len);

public:
    Slave(uint64_t size = 0x40000L);
    Slave(const char *str_size);
    ~Slave();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata) = 0;
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata) = 0;
    void sb_connect(pBus bus);
    uint64_t get_size();
};

#endif
