#include "plic.h"
#include "plic_reg.h"
#include <iostream>
#include "cpu/csr_config.h"
#include "util/util.h"

void PLIC::_init() {}

PLIC::PLIC() : csr_connect({0}), prior({0}), pending({0}), enable({0}), int_id({0}), threshold({0}), Device(), Slave(0x40000000) {}

PLIC::~PLIC() {}

void PLIC::run()
{
    uint32_t max_id, int_valid, id;

    for (int i = 0; i < TARGET_NUM; ++i)
    {
        max_id = 1;
        for (int j =0; j < (INT_NUM >> 5) + !!(INT_NUM & 0x1f); ++j)
        {
            id = j << 5;
            int_valid = enable[i * INT_NUM + j] & pending[j];
            while (int_valid)
            {
                if (int_valid & 0x1)
                {
                    max_id = prior[max_id] < prior[id] ? id : max_id;
                }
                int_valid >>= 1;
                ++id;
            }
        }
        int_id[i] = max_id;
    }
}

bool PLIC::write(const Addr &addr,
                  const DataType &data_type,
                  const uint64_t &wdata)
{
    uint64_t mask, _wdata;
    uint32_t int_n, tar_n;

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

    if (addr < 0x1000) // Priorty
    {
        int_n = addr >> 2;
        if (int_n < INT_NUM)
        {
            prior[int_n] = _wdata;
        }
    }
    else if (addr < 0x2000) // Pending
    {
        /* Write ignore */
    }
    else if (addr < 0x200000) // Enable
    {
        tar_n = (addr - 0x2000) >> 7;
        if (tar_n < TARGET_NUM)
        {
            int_n = (addr % 0x7f);
            if (int_n < (INT_NUM >> 5) + !!(INT_NUM & 0x1f))
            {
                enable[tar_n * INT_NUM + int_n] = _wdata;
            }
        }
    }
    else if (addr < 0x40000000) // Priorty threshole, Claim/Complete
    {
        tar_n = (addr - 0x200000) >> 12;
        if (tar_n < TARGET_NUM)
        {
            if (addr & 0x4) 
            {
                int_id[tar_n] = _wdata;
            }
            else
            {
                threshold[tar_n] = _wdata;
            }
        }
    }
    prior[0] = 0;
    return 1;
}

bool PLIC::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    uint64_t mask;
    uint32_t int_n, tar_n;

    rdata = 0L;
    if (addr < 0x1000) // Priorty
    {
        int_n = addr >> 2;
        if (int_n < INT_NUM)
        {
            rdata = prior[int_n];
        }
    }
    else if (addr < 0x2000) // Pending
    {
        int_n = (addr - 0x1000) >> 2;
        if (int_n < (INT_NUM >> 5) + !!(INT_NUM & 0x1f))
        {
            rdata = pending[int_n];
        }

    }
    else if (addr < 0x200000) // Enable
    {
        tar_n = (addr - 0x2000) >> 7;
        if (tar_n < TARGET_NUM)
        {
            int_n = (addr % 0x7f);
            if (int_n < (INT_NUM >> 5) + !!(INT_NUM & 0x1f))
            {
                rdata = enable[tar_n * INT_NUM + int_n];
            }
        }
    }
    else if (addr < 0x40000000) // Priorty threshole, Claim/Complete
    {
        tar_n = (addr - 0x200000) >> 12;
        if (tar_n < TARGET_NUM)
        {
            if (addr & 0x4) 
            {
                rdata = int_id[tar_n];
            }
            else
            {
                rdata = threshold[tar_n];
            }
        }
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

void PLIC::set_ip(uint64_t *ip, uint8_t target)
{
    if (csr_connect[target]) abort();
    csr_connect[target] = 1;
    this->ip[target] = ip;
}

uint32_t *PLIC::get_pending()
{
    return pending;
}
