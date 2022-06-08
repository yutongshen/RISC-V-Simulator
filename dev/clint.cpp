#include <unistd.h>
#include "clint.h"
#include "cpu/csr_config.h"
#include "mmap/clint_reg.h"
#include "util/util.h"

void CLINT::_init() {}

CLINT::CLINT()
    : ip{0}, time(0), cnt(0), timecmp{(uint64_t) -1}, Device(), Slave(0x10000)
{
}

CLINT::~CLINT() {}

void CLINT::single_step()
{
    uint8_t i;
    if (!((cnt + 1) % 4)) {
        time += 3;
        for (i = 0; i < CORE_NUM; ++i) {
            if (ip[i]) {
                if (time >= timecmp[i])
                    *(ip[i]) |= MIP_MTIP;
                else
                    *(ip[i]) &= ~MIP_MTIP;
            } else
                abort();
        }
        cnt = 0;
    } else
        ++cnt;
}

bool CLINT::write(const Addr &addr,
                  const DataType &data_type,
                  const uint64_t &wdata)
{
    uint64_t mask, _wdata, _addr;
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

    _wdata = (wdata & mask) << ((addr & 0x7) << 3);
    _addr = addr & ~0x7;

    if (addr >= RG_MSIP && addr < RG_MSIP + 4 * CORE_NUM) {
        uint8_t id((addr - RG_MSIP) >> 2);
        if (ip[id]) {
            if (wdata & mask) {
                *(ip[id]) |= MIP_MSIP;
            } else {
                *(ip[id]) &= ~MIP_MSIP;
            }
        }
    } else if (_addr >= RG_TIMECMP && _addr < RG_TIMECMP + 8 * CORE_NUM) {
        uint8_t id((_addr - RG_TIMECMP) >> 3);
        timecmp[id] = _wdata;
    } else if (_addr == RG_TIME) {
        time = _wdata;
    }
    return 1;
}

bool CLINT::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    uint64_t _addr;
    _addr = addr & ~0x7;

    rdata = 0;
    if (addr >= RG_MSIP && addr < RG_MSIP + 4 * CORE_NUM) {
        uint8_t id((addr - RG_MSIP) >> 2);
        if (ip[id]) {
            rdata = !!(*(ip[id]) & MIP_MSIP);
        }
    } else if (_addr >= RG_TIMECMP && _addr < RG_TIMECMP + 8 * CORE_NUM) {
        uint8_t id((_addr - RG_TIMECMP) >> 3);
        rdata = timecmp[id] >> ((addr & 0x7) << 3);
    } else if (_addr == RG_TIME) {
        rdata = time >> ((addr & 0x7) << 3);
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

void CLINT::set_ip(const uint8_t &id, uint64_t *ip)
{
    if (id < CORE_NUM) {
        this->ip[id] = ip;
    } else {
        abort();
    }
}

const uint64_t *CLINT::get_time()
{
    return (const uint64_t *) &time;
}
