#ifndef __DISK__
#define __DISK__

#include "bus/slave.h"
#include <fstream>
#include <iostream>

class Disk : public Slave {
  std::fstream data;
  void copyfile(const char *name, const char *cp_name);

public:
  Disk(const char *file_name, uint64_t size = 0x4000UL);
  Disk(const char *file_name, const char *str_size);
  ~Disk();
  virtual bool write(const Addr &addr, const DataType &data_type,
                     const uint64_t &wdata);
  virtual bool read(const Addr &addr, const DataType &data_type,
                    uint64_t &rdata);
};

#endif
