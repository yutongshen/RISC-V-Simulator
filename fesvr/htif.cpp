#include "fesvr/htif.h"
#include <iostream>
#include "fesvr/bcd.h"
#include "fesvr/syscall.h"

HTIF::HTIF()
    : exitcode(0),
      sysbus(0),
      tohost_addr(0),
      fromhost_addr(0),
      dev_list{0},
      n_dev(0),
      default_device()
{
    register_device(new SysCall(this));
    register_device(new BCD(this));
    for (uint16_t i = n_dev; i < 256; ++i) {
        dev_list[i] = &default_device;
    }
}

HTIF::~HTIF()
{
    for (uint16_t i = 0; i < n_dev; ++i) {
        delete dev_list[i];
    }
}

void HTIF::run()
{
    uint64_t tohost(0);
    sysbus->read(tohost_addr, DATA_TYPE_DWORD, tohost);

    if (tohost) {
        printf("TOHOST: 0x%x\n", tohost);
        uint64_t ret;
        ret = dev_list[get_dev_id(tohost)]->handle_command(tohost);

        sysbus->write(tohost_addr, DATA_TYPE_DWORD, 0);
        sysbus->write(fromhost_addr, DATA_TYPE_DWORD, ret);
    }
}

void HTIF::bus_connect(Bus *bus)
{
    sysbus = bus;
}

void HTIF::register_device(FrontEndDevice *dev)
{
    dev_list[n_dev++] = dev;
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

void HTIF::write_exitcode(uint64_t code)
{
    exitcode = code;
}

bool HTIF::sysbus_read(uint64_t addr, DataType type, uint64_t &rdata)
{
    return sysbus->read(addr, type, rdata);
}
