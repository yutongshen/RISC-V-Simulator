#include "dev/device.h"

uint8_t Device::num(0);

void Device::_cnt_num()
{
    ++Device::num;
}

Device::Device() : d_id(num) {}

Device::~Device() {}
