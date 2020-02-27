#include "bus/bus.h"
#include "util/util.h"
#include <assert.h>
#include <iostream>
using namespace std;

Bus::Bus() : slave_table{0}, slave_cnt(1), slaves(1, NULL) {}

Bus::~Bus() {}

bool Bus::_find_slave(const Addr &addr, Addr &offset, uint8_t &n_slave) {
  uint8_t test(leading_zero(addr));
  uint8_t base(63 - test);
  n_slave = slave_table[63 - base];
  // cout << hex << endl << addr << " " << (int)base << " " << (int)test <<
  // endl;

  if (n_slave != 1)
    offset = addr & ~(-1U << base);
  else
    offset = addr;

  return n_slave;
}

void Bus::m_connect(pMaster master) { masters.push_back(master); }

void Bus::s_connect(pSlave slave) {
  uint8_t base_inv(leading_zero(slave->get_size() - 1));
  if (slave_cnt != 1) {
    base_inv = (pre_base_inv < base_inv ? pre_base_inv : base_inv) - 1;
    slave_table[base_inv] = slave_cnt;
  } else {
    for (int i = base_inv; i < 64; ++i)
      slave_table[i] = slave_cnt;
  }
  pre_base_inv = base_inv;
  slaves.push_back(slave);
  slave->connect(this);
  ++slave_cnt;
  // for (uint8_t i = 0; i < 64; ++i)
  //   cout << (int)i << ": " << (int)slave_table[i] << endl;
}

void Bus::s_connect(const Addr &addr, pSlave slave) {
  uint8_t base_inv(leading_zero(addr) + 1);
  if (slave_cnt != 1) {
    base_inv = (pre_base_inv < base_inv ? pre_base_inv : base_inv) - 1;
    slave_table[base_inv] = slave_cnt;
    pre_base_inv = base_inv;
  } else {
    pre_base_inv = base_inv - leading_zero(slave->get_size() - 1);
    for (int i = pre_base_inv; i < base_inv; ++i)
      slave_table[i] = slave_cnt;
  }
  slaves.push_back(slave);
  slave->connect(this);
  ++slave_cnt;
  // for (uint8_t i = 0; i < 64; ++i)
  //   cout << (int)i << ": " << (int)slave_table[i] << endl;
}

bool Bus::write(const Addr &addr, const DataType &data_type,
                const uint64_t &wdata) {
  Addr offset;
  uint8_t n_slave;
  if (!_find_slave(addr, offset, n_slave))
    return 0;
  // if (addr == 0x10410) {
  //     cout << hex << endl << addr << " " << value << endl;
  //     throw "";
  // }
  return slaves[n_slave]->write(offset, data_type, wdata);
}

bool Bus::read(const Addr &addr, const DataType &data_type, uint64_t &rdata) {
  Addr offset;
  uint8_t n_slave;
  // cout << hex << endl << addr << endl;
  if (!_find_slave(addr, offset, n_slave))
    return 0;

  return slaves[n_slave]->read(offset, data_type, rdata);
}
