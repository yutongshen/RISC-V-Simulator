#include "fesvr/syscall.h"
#include <iostream>

SysCall::SysCall() : table{0} {
}

SysCall::~SysCall() {}

SyscallFunc SysCall::get_func(uint64_t sys_id) {
    if (sys_id >= TABLE_SIZE || !table[sys_id]) abort();
    return table[sys_id];
}
