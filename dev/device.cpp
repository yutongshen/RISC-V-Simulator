#include "dev/device.h"
#include <unistd.h>

uint8_t Device::num(0);

void Device::_cnt_num()
{
    ++Device::num;
}

Device::Device() : d_id(num), wfi(0), enable(0) {}

Device::~Device() {}

void Device::do_loop()
{
    while (enable) {
        if (wfi)
            usleep(1000);
        else
            single_step();
    }
}

void Device::run()
{
    if (!enable) {
        enable = 1;
        dthread = std::thread(&Device::do_loop, this);
    }
}

void Device::stop()
{
    if (enable) {
        enable = 0;
        dthread.join();
    }
}
