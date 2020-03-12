#include "fesvr/htif.h"
#include <iostream>

#define PAYLOAD(x) ((x) << 16 >> 16)

HTIF::HTIF()
    : exitcode(0), sysbus(0), tohost_addr(0), fromhost_addr(0), syscall(this)
{
}

HTIF::~HTIF() {}

void HTIF::run()
{
    uint64_t tohost(0);
    sysbus->read(tohost_addr, DATA_TYPE_DWORD, tohost);

    if (tohost & 1) {  // pass or fail
        exitcode = PAYLOAD(tohost);
        if (exit_code()) {  // meaning fail
            std::cerr << std::hex << "There is error (tohost: 0x" << exit_code()
                      << ")" << std::endl;
        }
    } else if (tohost) {
        uint64_t sys_id, ret;
        sysbus->read(tohost, DATA_TYPE_DWORD, sys_id);
        // std::cout << std::hex << "TOHOST: " << tohost << std::endl;
        ret = syscall.func(sys_id, tohost);

        sysbus->write(tohost_addr, DATA_TYPE_DWORD, 0);
        sysbus->write(fromhost_addr, DATA_TYPE_DWORD, ret);
    }
}

void HTIF::bus_connect(Bus *bus)
{
    sysbus = bus;
    syscall.bus_connect(bus);
}

void HTIF::set_host(uint64_t tohost_addr, uint64_t fromhost_addr)
{
    this->tohost_addr = tohost_addr;
    this->fromhost_addr = fromhost_addr;
}

uint64_t HTIF::exit_code()
{
    return exitcode >> 1;
}

bool HTIF::exit()
{
    return exitcode;
}
