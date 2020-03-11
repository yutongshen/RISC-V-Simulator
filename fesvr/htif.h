#ifndef __HTIF__
#define __HTIF__

#include "fesvr/syscall.h"

// host target interface
class HTIF {
    bus *sysbus;
    uint64_t tohost_addr;
    uint64_t fromhost_addr;
    SysCall syscall;
public:
    HTIF();
    ~HTIF();
    void run();
};

#endif
