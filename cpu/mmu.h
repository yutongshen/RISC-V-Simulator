#ifndef __MMU__
#define __MMU__

#include "bus/master.h"
#include "cpu/csr.h"

class MMU : public Master {
  CSR *csr;
  virtual void _init();
  virtual bool write(const Addr &addr, const DataType &data_type,
                     const uint64_t &wdata);
  virtual bool read(const Addr &addr, const DataType &data_type,
                    uint64_t &rdata);
  Addr translate(const Addr &addr, const uint64_t &len, uint8_t type);

public:
  MMU(CSR *csr);
  ~MMU();
  uint64_t fetch(const Addr &pc);
  uint64_t load(const Addr &addr, const DataType &data_type);
  void store(const Addr &addr, const DataType &data_type,
             const uint64_t &wdata);
  bool pmp_ok(const Addr &addr, const uint64_t &len, uint8_t type);
};

#endif
