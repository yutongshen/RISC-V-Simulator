#ifndef __BUS__
#define __BUS__

#include "bus/addr.h"
#include "bus/datatype.h"
#include "bus/master.h"
#include "bus/slave.h"
#include <vector>

typedef Master *pMaster;
typedef Slave *pSlave;

class Bus {
  uint8_t slave_table[64];
  uint8_t slave_cnt;
  uint8_t pre_base_inv;
  std::vector<pMaster> masters;
  std::vector<pSlave> slaves;
  bool _find_slave(const Addr &addr, Addr &offset, uint8_t &n_slave);

public:
  Bus();
  ~Bus();
  void m_connect(pMaster master);
  void s_connect(pSlave slave);
  void s_connect(const Addr &addr, pSlave slave);
  bool write(const Addr &addr, const DataType &data_type,
             const uint64_t &wdata);
  bool read(const Addr &addr, const DataType &data_type, uint64_t &rdata);
};

#endif
