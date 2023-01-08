#include "bus/bus.h"
#include <iostream>
#include "util/util.h"
using namespace std;

void del_mmap(memmap_entry_t *ptr)
{
    for (int i = 0; i < 64; ++i) {
        if (ptr[i].valid && ptr[i].is_ptr)
            del_mmap((memmap_entry_t *) ptr[i].val.ptr);
    }
    delete[] ptr;
}

void init_mmap(memmap_entry_t *ptr)
{
    for (int i = 0; i < 64; ++i) {
        ptr[i].valid = 0;
    }
}

void Bus::fill_mmap_entry(memmap_entry_t *ptr, uint32_t num, uint8_t bound)
{
    for (int i = bound; i < 64; ++i) {
        if (ptr[i].valid)
            abort();
        ptr[i].valid = 1;
        ptr[i].is_ptr = 0;
        ptr[i].val.ui = num;
    }
}

Bus::Bus()
    : slave_cnt(0),
      mmap(new memmap_entry_t[64]),
      bus_mutex(PTHREAD_MUTEX_INITIALIZER),
      slaves()
{
    init_mmap(mmap);
}

Bus::~Bus()
{
    del_mmap(mmap);
}

bool Bus::find_slave(const Addr &addr, Addr &offset, uint8_t &n_slave)
{
    uint8_t idx(clz(addr));
    memmap_entry_t *mmap_ptr(mmap);
    offset = addr;

    // uint64_t rdata;
    // slaves[5]->read(0, DATA_TYPE_DWORD, rdata);
    // std::cout << std::hex << "MEM[80000000]: " << rdata << std::endl;
    // std::cout << std::hex << "ADDR : " << addr << " SLAVE : " << (int)n_slave
    // << std::endl;

    do {
        idx = idx >= 64 ? 63 : idx;
        if (!mmap_ptr[idx].valid)
            return 0;
        if (mmap_ptr[idx].is_ptr) {
            mmap_ptr = (memmap_entry_t *) mmap_ptr[idx].val.ptr;
            offset &= ~(1L << (64 - (idx + 1)));
            idx = clz(offset);
        } else {
            break;
        }
    } while (1);

    n_slave = mmap_ptr[idx].val.ui;
    return 1;
}

void Bus::m_connect(pMaster master)
{
    masters.push_back(master);
}

void Bus::s_connect(const Addr &addr, pSlave slave)
{
    slaves.push_back(slave);
    // slave->sb_connect(this);

    uint8_t idx(clz(addr));
    uint64_t offset(addr);
    memmap_entry_t *mmap_ptr(mmap);

    do {
        if (idx == 64)
            return fill_mmap_entry(mmap_ptr, slave_cnt++,
                                   clz(slave->get_size() - 1));
        if (!mmap_ptr[idx].valid) {
            mmap_ptr[idx].valid = 1;
            mmap_ptr[idx].is_ptr = 1;
            mmap_ptr[idx].val.ptr = (memmap_entry_t *) (new memmap_entry_t[64]);
            init_mmap((memmap_entry_t *) mmap_ptr[idx].val.ptr);
            mmap_ptr = (memmap_entry_t *) mmap_ptr[idx].val.ptr;
        } else if (mmap_ptr[idx].is_ptr) {
            mmap_ptr = (memmap_entry_t *) mmap_ptr[idx].val.ptr;
        } else {
            abort();
        }
        offset &= ~(1L << (64 - (idx + 1)));
        idx = clz(offset);
    } while (1);
}

bool Bus::write(const Addr &addr,
                const DataType &data_type,
                const uint64_t &wdata)
{
    Addr offset;
    uint8_t n_slave;
    bool ret = 0;

    // pthread_mutex_lock(&bus_mutex);
    if (!find_slave(addr, offset, n_slave) || (addr % GET_SIZE(data_type)))
        goto done;
    ret = slaves[n_slave]->write(offset, data_type, wdata);

done:
    // pthread_mutex_unlock(&bus_mutex);
    return ret;
}

bool Bus::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    Addr offset;
    uint8_t n_slave;
    bool ret = 0;

    // pthread_mutex_lock(&bus_mutex);
    if (!find_slave(addr, offset, n_slave) || (addr % GET_SIZE(data_type)))
        goto done;
    ret = slaves[n_slave]->read(offset, data_type, rdata);
    if (GET_SIGNED(data_type))
        rdata = (int64_t) rdata << (64 - GET_SIZE(data_type) * 8) >>
                (64 - GET_SIZE(data_type) * 8);
    else
        rdata = (uint64_t) rdata << (64 - GET_SIZE(data_type) * 8) >>
                (64 - GET_SIZE(data_type) * 8);


done:
    // pthread_mutex_unlock(&bus_mutex);
    return ret;
}
