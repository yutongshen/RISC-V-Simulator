#ifndef __SYSCALL__
#define __SYSCALL__

#include <stdint.h>
#include "bus/bus.h"

#define TABLE_SIZE 2048

class HTIF;
class SysCall;
typedef uint64_t (SysCall::*SyscallFunc)(uint64_t args);

class SysCall
{
    SyscallFunc table[TABLE_SIZE];
    Bus *sysbus;
    HTIF *htif;

public:
    SysCall(HTIF *htif);
    ~SysCall();
    uint64_t func(uint64_t sys_id, uint64_t args);
    void bus_connect(Bus *bus);

    uint64_t SysWrite(uint64_t args);
};

#endif
