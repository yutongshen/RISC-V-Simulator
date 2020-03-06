#ifndef __MMU__
#define __MMU__

#include "bus/master.h"
#include "cpu/csr.h"

class MMU : public Master {
  uint64_t load_reservation_addr;
  CSR *csr;
  virtual void _init();
  Addr translate(const Addr &addr, const uint64_t &len, uint8_t type);
  Addr trace_pt(const Addr &addr, const uint64_t &len, uint8_t type,
                uint8_t prv);
  void throw_access_exception(const Addr &addr, uint8_t type);

public:
  MMU(CSR *csr);
  ~MMU();
  virtual bool write(const Addr &addr, const DataType &data_type,
                     const uint64_t &wdata);
  virtual bool read(const Addr &addr, const DataType &data_type,
                    uint64_t &rdata);
  uint64_t fetch(const Addr &pc, const uint64_t &alignment_mask);
  uint64_t load(const Addr &addr, const DataType &data_type);
  void store(const Addr &addr, const DataType &data_type,
             const uint64_t &wdata);
  uint64_t
  amo_operate(const Addr &addr, const DataType &type, const uint64_t &src,
              uint64_t (*func)(const uint64_t &rdata, const uint64_t &src));
  bool pmp_ok(const Addr &addr, const uint64_t &len, uint8_t type, uint8_t prv);
  void acquire_load_reservation(const Addr &addr);
  bool check_load_reservation(const Addr &addr);
  inline void release_load_reservation() {
    load_reservation_addr = -1UL;
  }
};

#endif
