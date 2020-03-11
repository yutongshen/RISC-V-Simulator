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

public:
    HTIF();
    ~HTIF();
    void run();
    void bus_connect(Bus *bus);
    void set_host(uint64_t tohost_addr, uint64_t fromhost_addr);
    inline uint64_t get_exit_code() { return syscall.get_exit_code(); }
};

#endif
