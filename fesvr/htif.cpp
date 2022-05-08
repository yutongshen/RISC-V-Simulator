#include "fesvr/htif.h"
#include <iostream>
#include "fesvr/bcd.h"
#include "fesvr/syscall.h"
#include "mmap/htif_reg.h"

extern bool __exit;

HTIF::HTIF()
    : exitcode(0),
      sysbus(0),
      fromhost(0),
      dev_list{0},
      n_dev(0),
      default_device(),
      Slave(0x1000)
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

bool HTIF::write(const Addr &addr,
                 const DataType &data_type,
                 const uint64_t &wdata)
{
    uint64_t mask, _wdata;
    switch (data_type) {
    case DATA_TYPE_DWORD:
        mask = -1UL;
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        mask = -1U;
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        mask = 0xffff;
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        mask = 0xff;
        break;
    default:
        abort();
    }

    _wdata = wdata & mask;

    switch (addr) {
    case RG_TOHOST:
        if (_wdata) {
            uint64_t ret;
            fromhost = dev_list[get_dev_id(_wdata)]->handle_command(_wdata);
        }
        break;
    case RG_FROMHOST:
        fromhost = _wdata;
        break;
    }

    return 1;
}

bool HTIF::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = 0;
    switch (addr) {
    case RG_TOHOST:
        rdata = 0;
        break;
    case RG_FROMHOST:
        rdata = fromhost;
        break;
    }

    switch (data_type) {
    case DATA_TYPE_DWORD:
        break;
    case DATA_TYPE_WORD:
        rdata = sext(rdata, 32);
        break;
    case DATA_TYPE_WORD_UNSIGNED:
        rdata = zext(rdata, 32);
        break;
    case DATA_TYPE_HWORD:
        rdata = sext(rdata, 16);
        break;
    case DATA_TYPE_HWORD_UNSIGNED:
        rdata = zext(rdata, 16);
        break;
    case DATA_TYPE_BYTE:
        rdata = sext(rdata, 8);
        break;
    case DATA_TYPE_BYTE_UNSIGNED:
        rdata = zext(rdata, 8);
        break;
    default:
        abort();
    }

    return 1;
}

void HTIF::bus_connect(Bus *bus)
{
    sysbus = bus;
}

void HTIF::register_device(FrontEndDevice *dev)
{
    dev_list[n_dev++] = dev;
}

uint64_t HTIF::get_exit_code()
{
    return exitcode >> 1;
}

void HTIF::set_exit_code(uint64_t code)
{
    __exit = !!code;
    exitcode = code;
}

bool HTIF::exit()
{
    return exitcode;
}

bool HTIF::sysbus_read(uint64_t addr, DataType type, uint64_t &rdata)
{
    return sysbus->read(addr, type, rdata);
}
