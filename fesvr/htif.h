#ifndef __HTIF__
#define __HTIF__

#include "bus/bus.h"
#include "fesvr/syscall.h"

// host target interface
class HTIF
{
    Bus *sysbus;
    uint64_t tohost_addr;
    uint64_t fromhost_addr;
    SysCall syscall;
    uint64_t exitcode;

public:
    HTIF();
    ~HTIF();
    void run();
    void bus_connect(Bus *bus);
    void set_host(uint64_t tohost_addr, uint64_t fromhost_addr);
    uint64_t exit_code();
    bool exit();

    friend class SysCall;
};

#endif
