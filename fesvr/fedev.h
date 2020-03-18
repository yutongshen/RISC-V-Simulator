#ifndef __FEDEV__
#define __FEDEV__

#include <functional>
#include "bus/datatype.h"
#include "util/util.h"

class HTIF;

typedef std::function<uint64_t(uint64_t)> command_func_t;

class FrontEndDevice
{
    command_func_t cmd_list[256];
    uint16_t n_cmd;
    HTIF *htif;

public:
    FrontEndDevice(HTIF *htif);
    ~FrontEndDevice();
    void register_command(uint16_t cmd_id, command_func_t func);
    uint64_t handle_command(uint64_t cmd);
    static uint64_t default_command(uint64_t cmd);
    bool sysbus_read(uint64_t addr, DataType type, uint64_t &rdata);
    void write_exitcode(uint64_t code);
    uint64_t exit_code();
};

class DefaultFEDevice : public FrontEndDevice
{
public:
    DefaultFEDevice() : FrontEndDevice(0) {}
    ~DefaultFEDevice() {}
};

#endif
