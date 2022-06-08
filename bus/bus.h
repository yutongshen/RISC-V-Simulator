#ifndef __BUS__
#define __BUS__

#include <vector>
#include <pthread.h>
#include "bus/addr.h"
#include "bus/datatype.h"
#include "bus/master.h"
#include "bus/slave.h"

typedef Master *pMaster;
typedef Slave *pSlave;

union uint64_ptr_t {
    uint64_t ui;
    void *ptr;
};

typedef struct {
    bool valid;
    bool is_ptr;
    union uint64_ptr_t val;
} memmap_entry_t;


class Bus
{
    memmap_entry_t *mmap;
    uint8_t slave_cnt;
    std::vector<pMaster> masters;
    std::vector<pSlave> slaves;
    pthread_mutex_t bus_mutex;
    bool find_slave(const Addr &addr, Addr &offset, uint8_t &n_slave);
    void fill_mmap_entry(memmap_entry_t *ptr, uint32_t num, uint8_t bound);

public:
    Bus();
    ~Bus();
    void m_connect(pMaster master);
    void s_connect(const Addr &addr, pSlave slave);
    bool write(const Addr &addr,
               const DataType &data_type,
               const uint64_t &wdata);
    bool read(const Addr &addr, const DataType &data_type, uint64_t &rdata);
};

#endif
