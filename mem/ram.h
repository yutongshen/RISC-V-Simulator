#ifndef __RAM__
#define __RAM__

#include "bus/slave.h"
typedef uint64_t *pUint64;

class RAM : public Slave {
  pUint64 data;

public:
  RAM(uint64_t size = 0x4000UL);
  RAM(const char *str_size);
  ~RAM();
  virtual bool write(const Addr &addr, const DataType &data_type,
                     const uint64_t &wdata);
  virtual bool read(const Addr &addr, const DataType &data_type,
                    uint64_t &rdata);
};

#endif
