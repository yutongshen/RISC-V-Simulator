#include "finisher.h"
#include "mmap/finisher_reg.h"
#include "util/util.h"

void Finisher::_init() {}

Finisher::Finisher() : exit_code(0), Slave(0x1000) {}

Finisher::~Finisher() {}

bool Finisher::write(const Addr &addr,
                     const DataType &data_type,
                     const uint64_t &wdata)
{
    if (addr == RG_FINISH) {
        exit_code = (uint32_t) wdata;
    }
    return 1;
}

bool Finisher::read(const Addr &addr,
                    const DataType &data_type,
                    uint64_t &rdata)
{
    rdata = 0;
    if (addr == RG_FINISH) {
        rdata = exit_code;
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

uint32_t Finisher::get_exit_code()
{
    return exit_code;
}
