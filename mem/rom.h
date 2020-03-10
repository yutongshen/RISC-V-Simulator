#ifndef __ROM__
#define __ROM__

#include "bus/slave.h"

typedef uint64_t *pUint64;

class ROM : public Slave
{
    pUint64 data;
    void _init_data(const char *init_file);

public:
    ROM(const char *init_file, uint64_t size = 0x4000UL);
    ROM(const char *init_file, const char *str_size);
    ~ROM();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
};

#endif
