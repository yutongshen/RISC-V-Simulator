#include "cluster.h"
#include <iostream>
#include "cpu/csr_config.h"
#include "mmap/cluster_reg.h"
#include "util/util.h"

void Cluster::_init() {}

Cluster::Cluster() : bus(0), cores{0}, Device(), Slave(0x40000000) {}

Cluster::~Cluster() {}

void Cluster::run()
{
    for (int i = 0; i < 8; ++i) {
        if (cores[i])
            cores[i]->run();
    }
}

bool Cluster::write(const Addr &addr,
                    const DataType &data_type,
                    const uint64_t &wdata)
{
    uint64_t mask, _wdata;
    uint32_t i;

    switch (data_type) {
    case DATA_TYPE_DWORD:
        mask = -1UL;
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        mask = -1U;
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        mask = 0xffff;
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        mask = 0xff;
        break;
    default:
        abort();
    }

    _wdata = wdata & mask;

    i = 0x0;
    switch (addr) {
    case RG_PWR_REQ:
        _wdata &= 0xff;
        for (i = 0; _wdata; ++i, _wdata >>= 1) {
            if (cores[i] && (_wdata & 0x1))
                cores[i]->set_power_on(true);
        }
        break;
    case RG_CPU7_PC:
        ++i;
    case RG_CPU6_PC:
        ++i;
    case RG_CPU5_PC:
        ++i;
    case RG_CPU4_PC:
        ++i;
    case RG_CPU3_PC:
        ++i;
    case RG_CPU2_PC:
        ++i;
    case RG_CPU1_PC:
        ++i;
    case RG_CPU0_PC:
        if (cores[i])
            *(cores[i]->get_pc()) = _wdata;
        break;
    }

    return 1;
}

bool Cluster::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    int i;
    rdata = 0L;
    switch (addr) {
    case RG_PWR_REQ:
        /* Read all zero */
        break;
    case RG_CPU7_PC:
        ++i;
    case RG_CPU6_PC:
        ++i;
    case RG_CPU5_PC:
        ++i;
    case RG_CPU4_PC:
        ++i;
    case RG_CPU3_PC:
        ++i;
    case RG_CPU2_PC:
        ++i;
    case RG_CPU1_PC:
        ++i;
    case RG_CPU0_PC:
        if (cores[i])
            rdata = *(cores[i]->get_pc());
        break;
    }

    switch (data_type) {
    case DATA_TYPE_DWORD:
        break;
    case DATA_TYPE_WORD:
        rdata = sext(rdata, 32);
        break;
    case DATA_TYPE_WORD_UNSIGNED:
        rdata = zext(rdata, 32);
        break;
    case DATA_TYPE_HWORD:
        rdata = sext(rdata, 16);
        break;
    case DATA_TYPE_HWORD_UNSIGNED:
        rdata = zext(rdata, 16);
        break;
    case DATA_TYPE_BYTE:
        rdata = sext(rdata, 8);
        break;
    case DATA_TYPE_BYTE_UNSIGNED:
        rdata = zext(rdata, 8);
        break;
    default:
        abort();
    }


    return 1;
}

void Cluster::add(const uint8_t &num, CPU *core)
{
    cores[num] = core;
    core->bus_connect(bus);
}

void Cluster::bus_connect(pBus bus)
{
    this->bus = bus;
    for (int i = 0; i < 8; ++i) {
        if (cores[i])
            cores[i]->bus_connect(bus);
    }
}
