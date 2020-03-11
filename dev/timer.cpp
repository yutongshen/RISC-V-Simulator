#include "timer.h"
#include <iostream>
#include "cpu/csr_config.h"
#include "util/util.h"

void Timer::_init() {}

Timer::Timer() : is_mip_set(0), time(0), timecmp(0), Device(), Slave(0x1000) {}

Timer::~Timer() {}

void Timer::run()
{
    ++time;
    if (!is_mip_set)
        abort();
    if (time >= timecmp)
        *ip |= MIP_MTIP;
    else
        *ip &= ~MIP_MTIP;
}

bool Timer::write(const Addr &addr,
                  const DataType &data_type,
                  const uint64_t &wdata)
{
    uint8_t len;
    uint64_t mask, _wdata;
    switch (data_type) {
    case DATA_TYPE_DWORD:
        len = 8;
        mask = -1UL;
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        len = 4;
        mask = -1U;
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        len = 2;
        mask = 0xffff;
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        len = 1;
        mask = 0xff;
        break;
    default:
        abort();
    }

    _wdata = wdata & mask;

    // Illegal space
    if (addr < MIP_BASE)
        return 0;
    if (addr + len > MIP_BASE + sizeof(*ip) && addr < TIMECMP_BASE)
        return 0;
    if (addr + len > TIMECMP_BASE + sizeof(timecmp) && addr < TIME_BASE)
        return 0;
    if (addr + len > TIME_BASE + sizeof(time))
        return 0;

    if (addr >= MIP_BASE && addr + len <= MIP_BASE + sizeof(*ip)) {
        if (!is_mip_set)
            abort();
        if (_wdata << (addr - MIP_BASE << 3))
            *ip |= MIP_MTIP;
        else
            *ip &= ~MIP_MTIP;
    }
    if (addr >= TIMECMP_BASE && addr + len <= TIMECMP_BASE + sizeof(timecmp)) {
        timecmp &= ~(mask << (addr - TIMECMP_BASE << 3));
        timecmp |= _wdata << (addr - TIMECMP_BASE << 3);
    }
    if (addr >= TIME_BASE && addr + len <= TIME_BASE + sizeof(time)) {
        time &= ~(mask << (addr - TIME_BASE << 3));
        time |= _wdata << (addr - TIME_BASE << 3);
    }
    return 1;
}

bool Timer::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    uint8_t len;
    uint64_t mask;
    bool _is_signed(0);

    switch (data_type) {
    case DATA_TYPE_DWORD:
        len = 8;
        mask = -1UL;
        break;
    case DATA_TYPE_WORD:
        _is_signed = 1;
    case DATA_TYPE_WORD_UNSIGNED:
        len = 4;
        mask = -1U;
        break;
    case DATA_TYPE_HWORD:
        _is_signed = 1;
    case DATA_TYPE_HWORD_UNSIGNED:
        len = 2;
        mask = 0xffff;
        break;
    case DATA_TYPE_BYTE:
        _is_signed = 1;
    case DATA_TYPE_BYTE_UNSIGNED:
        mask = 0xff;
        len = 1;
        break;
    default:
        abort();
    }

    rdata = 0;

    // Illegal space
    if (addr < MIP_BASE)
        return 0;
    if (addr + len > MIP_BASE + sizeof(*ip) && addr < TIMECMP_BASE)
        return 0;
    if (addr + len > TIMECMP_BASE + sizeof(timecmp) && addr < TIME_BASE)
        return 0;
    if (addr + len > TIME_BASE + sizeof(time))
        return 0;

    if (addr >= MIP_BASE && addr + len <= MIP_BASE + sizeof(*ip)) {
        if (*ip & MIP_MTIP)
            rdata |= 1 >> (addr - MIP_BASE >> 3);
        else
            rdata |= 0;
    }
    if (addr >= TIMECMP_BASE && addr + len <= TIMECMP_BASE + sizeof(timecmp)) {
        rdata |= timecmp >> (addr - TIMECMP_BASE >> 3);
    }
    if (addr >= TIME_BASE && addr + len <= TIME_BASE + sizeof(time)) {
        rdata |= time >> (addr - TIME_BASE >> 3);
    }

    rdata = _is_signed ? sext(rdata, len << 3) : zext(rdata, len << 3);
    return 1;
}

void Timer::set_ip(uint64_t *ip)
{
    this->ip = ip;
    is_mip_set = 1;
}
