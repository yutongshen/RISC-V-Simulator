#include "bus/bus.h"
#include "util/util.h"

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
    : slave_table{0}, slave_cnt(0), mmap(new memmap_entry_t[64]), slaves()
{
}

Bus::~Bus() {}

bool Bus::find_slave(const Addr &addr, Addr &offset, uint8_t &n_slave)
{
    uint8_t idx(clz(addr));
    memmap_entry_t *mmap_ptr(mmap);
    offset = addr;

    do {
        idx = idx >= 64 ? 63 : idx;
        if (!mmap_ptr[idx].valid)
            return 0;
        if (mmap_ptr[idx].is_ptr) {
            mmap_ptr = (memmap_entry_t *) mmap_ptr[idx].val.ptr;
            offset &= ~(1 << (64 - (idx + 1)));
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
    slave->connect(this);

    uint8_t idx(clz(addr));
    uint64_t offset(addr);
    memmap_entry_t *mmap_ptr;
    if (idx == 64)
        return fill_mmap_entry(mmap, slave_cnt++, clz(slave->get_size() - 1));

    mmap_ptr = mmap;
    do {
        if (idx >= clz(slave->get_size() - 1))
            abort();
        if (!mmap_ptr[idx].valid) {
            mmap_ptr[idx].valid = 1;
            mmap_ptr[idx].is_ptr = 1;
            mmap_ptr[idx].val.ptr = (memmap_entry_t *) (new memmap_entry_t[64]);
            return fill_mmap_entry((memmap_entry_t *) mmap_ptr[idx].val.ptr,
                                   slave_cnt++, clz(slave->get_size() - 1));
        } else if (mmap_ptr[idx].is_ptr) {
            mmap_ptr = (memmap_entry_t *) mmap_ptr[idx].val.ptr;
            offset &= ~(1 << (64 - (idx + 1)));
            idx = clz(offset);
        } else
            abort();
    } while (1);
}

bool Bus::write(const Addr &addr,
                const DataType &data_type,
                const uint64_t &wdata)
{
    Addr offset;
    uint8_t n_slave;
    if (!find_slave(addr, offset, n_slave))
        return 0;
    return slaves[n_slave]->write(offset, data_type, wdata);
}

bool Bus::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    Addr offset;
    uint8_t n_slave;
    if (!find_slave(addr, offset, n_slave))
        return 0;
    return slaves[n_slave]->read(offset, data_type, rdata);
}
