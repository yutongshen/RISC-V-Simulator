#include "fesvr/htif.h"

HTIF::HTIF() : sysbus(0), tohost_addr(0), fromhost_addr(0), syscall() {}

HTIF::~HTIF() {}

void HTIF::run()
{
    uint64_t tohost(0);
    sysbus->read(tohost_addr, DATA_TYPE_DWORD, tohost);
    if (tohost) {
        uint64_t sys_id, ret;
        sysbus->read(tohost, DATA_TYPE_DWORD, sys_id);
        ret = syscall.func(sys_id, tohost);

        if (get_exit_code())
            return;

        sysbus->write(tohost_addr, DATA_TYPE_DWORD, 0);
        sysbus->write(fromhost_addr, DATA_TYPE_DWORD, ret);
    } else
        sysbus->write(fromhost_addr, DATA_TYPE_DWORD, 0);
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
