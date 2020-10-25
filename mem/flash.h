#ifndef __FLASH__
#define __FLASH__

#include <fstream>
#include <iostream>
#include "bus/slave.h"

class Flash : public Slave
{
    std::fstream data;
    void copyfile(const char *name, const char *cp_name);

public:
    Flash(const char *file_name, uint64_t size = 0x4000UL);
    Flash(const char *file_name, const char *str_size);
    ~Flash();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
};

#endif
