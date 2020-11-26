#ifndef __TMDL__
#define __TMDL__
#include <string>
#include <vector>
#include "bus/slave.h"
#include "dev/device.h"
#include "main/riscv_soc_def.h"

#define FIFO_DEPTH 128

class TMDL : public Slave
{
    uint64_t arg_fifo[FIFO_DEPTH];
    uint32_t arg_head;
    uint32_t arg_tail;
    const uint64_t *time;
    std::vector<std::string> tmdl_log;

public:
    TMDL(const char *path);
    ~TMDL();
    virtual bool write(const Addr &addr,
                       const DataType &data_type,
                       const uint64_t &wdata);
    virtual bool read(const Addr &addr,
                      const DataType &data_type,
                      uint64_t &rdata);
    void tm_print(const char *fmt);
    void set_time(const uint64_t *time);
};


#endif
