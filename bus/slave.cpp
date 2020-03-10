#include "bus/slave.h"
#include <assert.h>
#include "util/util.h"

uint8_t Slave::num(0);

void Slave::_cnt_num()
{
    ++Slave::num;
}

bool Slave::_check_bound(const Addr &addr, const uint8_t len)
{
    return addr + len <= size;
}

Slave::Slave(uint64_t size) : size(size), s_id(num), p_bus(NULL), connected(0)
{
    Slave::_cnt_num();
}

Slave::Slave(const char *str_size)
    : size(str2uint64_t(str_size)), s_id(num), p_bus(NULL), connected(0)
{
    Slave::_cnt_num();
}

Slave::~Slave() {}

void Slave::connect(pBus bus)
{
    assert(!connected);
    this->p_bus = bus;
    connected = 1;
}

uint64_t Slave::get_size()
{
    return size;
}
