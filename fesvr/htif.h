#ifndef __HTIF__
#define __HTIF__

#include "bus/bus.h"
#include "fesvr/fedev.h"

#define get_dev_id(x) (((x) >> 56) & 0xff)
#define get_cmd_id(x) (((x) >> 48) & 0xff)
#define get_cmd_value(x) ((x) << 16 >> 16)

class FrontEndDevice;
class DefaultFEDevice;

// host target interface
class HTIF
{
    Bus *sysbus;
    FrontEndDevice *dev_list[256];
    uint64_t n_dev;
    DefaultFEDevice default_device;
    uint64_t tohost_addr;
    uint64_t fromhost_addr;
    uint64_t exitcode;

public:
    HTIF();
    ~HTIF();
    void run();
    void bus_connect(Bus *bus);
    void register_device(FrontEndDevice *dev);
    void set_host(uint64_t tohost_addr, uint64_t fromhost_addr);
    uint64_t exit_code();
    bool exit();
    void write_exitcode(uint64_t code);
    bool sysbus_read(uint64_t addr, DataType type, uint64_t &rdata);
};

#endif
