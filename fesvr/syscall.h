#ifndef __SYSCALL__
#define __SYSCALL__

#include <stdint.h>
#include "fesvr/fedev.h"

#define TABLE_SIZE 2048

class HTIF;
class SysCall;
typedef uint64_t (SysCall::*SyscallFunc)(uint64_t args);

class SysCall : public FrontEndDevice
{
    SyscallFunc table[TABLE_SIZE];

public:
    SysCall(HTIF *htif);
    ~SysCall();
    uint64_t func(uint64_t args);
    uint64_t SysWrite(uint64_t args);
};

#endif
