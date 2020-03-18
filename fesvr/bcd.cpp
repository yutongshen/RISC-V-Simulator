#include "fesvr/bcd.h"

BCD::BCD(HTIF *htif) : FrontEndDevice(htif)
{
    register_command(0,
                     std::bind(&BCD::handle_read, this, std::placeholders::_1));
    register_command(
        1, std::bind(&BCD::handle_write, this, std::placeholders::_1));
}

BCD::~BCD() {}

uint64_t BCD::handle_read(uint64_t args)
{
    return 1;
}

uint64_t BCD::handle_write(uint64_t args)
{
    putchar((int) args);
    return 1;
}
