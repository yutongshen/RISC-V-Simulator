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
class HTIF : public Slave
{
    Bus *sysbus;
    FrontEndDevice *dev_list[256];
    uint64_t n_dev;
    DefaultFEDevice default_device;
    uint64_t fromhost;
    uint64_t exitcode;

public:
    HTIF();
    ~HTIF();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    void register_device(FrontEndDevice *dev);
    uint64_t get_exit_code();
    void set_exit_code(uint64_t code);
    bool exit();
    void bus_connect(Bus *bus);
    bool sysbus_read(uint64_t addr, DataType type, uint64_t &rdata);
};

#endif
