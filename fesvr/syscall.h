#ifndef __SYSCALL__
#define __SYSCALL__

#include <stdint.h>

#define TABLE_SIZE 2048

class SysCall;
typedef uint64_t (SysCall::*SyscallFunc) (uint64_t sys_id, ...);

class SysCall {
    SyscallFunc table[TABLE_SIZE];

public:
    SysCall();
    ~SysCall();
    SyscallFunc get_func(uint64_t sys_id);
};

#endif
