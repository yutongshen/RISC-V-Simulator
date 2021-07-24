#include "fesvr/syscall.h"
#include <iostream>
#include "bus/datatype.h"
#include "fesvr/htif.h"
#include "util/util.h"

SysCall::SysCall(HTIF *htif) : table{0}, FrontEndDevice(htif)
{
    table[64] = &SysCall::SysWrite;
    register_command(0, std::bind(&SysCall::func, this, std::placeholders::_1));
}

SysCall::~SysCall() {}

uint64_t SysCall::func(uint64_t args)
{
    if (args & 1) {  // pass or fail
        write_exitcode(get_cmd_value(args));
        if (exit_code()) {  // meaning fail
            std::cerr << std::hex << "There is error (tohost: 0x" << exit_code()
                      << ")" << std::endl;
        }
        return args;
    } else {
        uint64_t sys_id;
        sysbus_read(args, DATA_TYPE_DWORD, sys_id);

        if (sys_id >= TABLE_SIZE || !table[sys_id]) {
            printf("Unknown Syscall ID : 0x%lx\r\n", sys_id);
            abort();
        }
        // std::cout << std::hex << "SYS_ID: " << sys_id << std::endl;
        return (this->*(table[sys_id]))(args);
    }
}

uint64_t SysCall::SysWrite(uint64_t args)
{
    uint64_t fd;
    uint64_t pbuf;
    uint64_t len;

    sysbus_read(args + 1 * 8, DATA_TYPE_DWORD, fd);
    sysbus_read(args + 2 * 8, DATA_TYPE_DWORD, pbuf);
    sysbus_read(args + 3 * 8, DATA_TYPE_DWORD, len);

    char *buff(new char[len]);
    uint64_t tmp;

    for (int i = 0; i < len; ++i) {
        sysbus_read(pbuf + i, DATA_TYPE_BYTE, tmp);
        if ((char) tmp == '\n');
            putchar('\r');
        putchar((char) tmp);
        // buff[i] = tmp;
    }

    // printf("%s", buff);

    return 1;
}
