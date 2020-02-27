#include "bus/master.h"
#include "bus/bus.h"
#include <assert.h>
#include <iostream>

uint8_t Master::num(0);

void Master::_cnt_num() { ++Master::num; }

Master::Master() : m_id(num), p_bus(NULL), connected(0) { Master::_cnt_num(); }

Master::~Master() {}

void Master::connect(pBus bus) {
  assert(!connected);
  this->p_bus = bus;
  bus->m_connect(this);
  connected = 1;
}

bool Master::write(const Addr &addr, const DataType &data_type,
                   const uint64_t &wdata) {
  if (connected)
    return p_bus->write(addr, data_type, wdata);
  return 0;
}
bool Master::read(const Addr &addr, const DataType &data_type,
                  uint64_t &rdata) {
  if (connected)
    return p_bus->read(addr, data_type, rdata);
  return 0;
}
