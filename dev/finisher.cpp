#include "finisher.h"
#include "mmap/finisher_reg.h"
#include "util/util.h"

extern bool __exit;

Finisher::Finisher() : exit_code(0), Slave(0x1000) {}

Finisher::~Finisher() {}

bool Finisher::write(const Addr &addr,
                     const DataType &data_type,
                     const uint64_t &wdata)
{
    if (addr == RG_FINISH) {
        exit_code = (uint32_t) wdata;
        __exit = !!wdata;
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

    return 1;
}

uint32_t Finisher::get_exit_code()
{
    return exit_code;
}
