#include "fesvr/fedev.h"
#include <iostream>
#include "fesvr/htif.h"

bool FrontEndDevice::sysbus_read(uint64_t addr, DataType type, uint64_t &rdata)
{
    return htif->sysbus_read(addr, type, rdata);
}

void FrontEndDevice::write_exitcode(uint64_t code)
{
    htif->set_exit_code(code);
}

uint64_t FrontEndDevice::exit_code()
{
    return htif->get_exit_code();
}

FrontEndDevice::FrontEndDevice(HTIF *htif) : htif(htif), cmd_list(), n_cmd(0)
{
    for (uint16_t i = 0; i < 256; ++i) {
        cmd_list[i] =
            std::bind(&FrontEndDevice::default_command, std::placeholders::_1);
    }
}

FrontEndDevice::~FrontEndDevice() {}

void FrontEndDevice::register_command(uint16_t cmd_id, command_func_t func)
{
    cmd_list[cmd_id] = func;
}

uint64_t FrontEndDevice::handle_command(uint64_t cmd)
{
    return cmd_list[get_cmd_id(cmd)](get_cmd_value(cmd));
}

uint64_t FrontEndDevice::default_command(uint64_t cmd)
{
    std::cerr << "Unknown front-end server command" << std::endl;
    return 0;
}
