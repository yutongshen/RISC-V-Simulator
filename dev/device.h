#ifndef __DEVICE__
#define __DEVICE__

#include <thread>
#include "bus/bus.h"

class Device
{
    static uint8_t num;
    static void _cnt_num();

protected:
    uint8_t d_id;
    uint8_t wfi;
    uint8_t enable;
    std::thread dthread;
    virtual void _init() = 0;
    virtual void do_loop();

public:
    Device();
    ~Device();
    virtual void single_step() = 0;
    virtual void run();
    virtual void stop();
};

#endif
