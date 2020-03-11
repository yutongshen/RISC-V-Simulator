#include "fesvr/syscall.h"
#include <iostream>
#include "bus/datatype.h"
#include "util/util.h"

SysCall::SysCall() : exit_code(0), sysbus(0), table{0}
{
    table[64] = &SysCall::SysWrite;
}

SysCall::~SysCall() {}

uint64_t SysCall::func(uint64_t sys_id, uint64_t args)
{
    if (sys_id >= TABLE_SIZE || !table[sys_id]) {
        printf("Unknown Syscall ID : 0x%lx\n", sys_id);
        exit_code = 1;
        return 0;
    }
    return (this->*(table[sys_id]))(args);
}

void SysCall::bus_connect(Bus *bus)
{
    sysbus = bus;
}

uint64_t SysCall::SysWrite(uint64_t args)
{
    uint64_t fd;
    uint64_t pbuf;
    uint64_t len;

    sysbus->read(args + 1 * 8, DATA_TYPE_DWORD, fd);
    sysbus->read(args + 2 * 8, DATA_TYPE_DWORD, pbuf);
    sysbus->read(args + 3 * 8, DATA_TYPE_DWORD, len);

    char *buff(new char[len]);
    uint64_t tmp;

    for (int i = 0; i < len; ++i) {
        sysbus->read(pbuf + i, DATA_TYPE_BYTE, tmp);
        buff[i] = tmp;
    }

    printf("%s", buff);

    return 1;
}
