#include "plic.h"
#include "plic_reg.h"
#include <iostream>
#include "cpu/csr_config.h"
#include "util/util.h"

void PLIC::_init() {}

PLIC::PLIC() : csr_connect{0}, prior{0}, pending{0}, enable{0}, int_id{0}, threshold{0}, Device(), Slave(0x40000000) {}

PLIC::~PLIC() {}

void PLIC::run()
{
    uint32_t max_id, int_valid, id;

    for (int i = 0; i < TARGET_NUM; ++i)
    {
        if (!csr_connect[i]) continue;
        #ifndef IRQ_PREEMPTION
        if (*(irqdst[i]) & MIP_MEIP) continue;
        #endif

        max_id = 0;
        for (int j = 0; j < INT_REG_NUM; ++j)
        {
            id = j << 5;
            int_valid = enable[i * INT_REG_NUM + j] & pending[j] & ~dispatch[j];
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
        if (max_id && prior[max_id] > threshold[i])
        {
            *(irqdst[i]) |= MIP_MEIP;
            int_id[i] = max_id;
            dispatch[max_id >> 5] |= 1U << (max_id & 0x1f);
        }
    }
    // std::cout << "PENDING     : " << pending[0] << std::endl;
    // std::cout << "ENABLE      : " << enable[0] << std::endl;
    // std::cout << "DISPATCH    : " << dispatch[0] << std::endl;
    // std::cout << "INT_ID      : " << int_id[0] << std::endl;
    // std::cout << "PRIORITY[0] : " << prior[0] << std::endl;
    // std::cout << "PRIORITY[1] : " << prior[1] << std::endl;
    // std::cout << "PRIORITY[2] : " << prior[2] << std::endl;
    // std::cout << "PRIORITY[3] : " << prior[3] << std::endl;
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
            int_n = addr & 0x7f;
            if (int_n < INT_REG_NUM)
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
                *(irqdst[tar_n]) &= ~MIP_MEIP; // Clear target
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
        if (int_n < INT_REG_NUM)
        {
            rdata = pending[int_n];
        }

    }
    else if (addr < 0x200000) // Enable
    {
        tar_n = (addr - 0x2000) >> 7;
        if (tar_n < TARGET_NUM)
        {
            int_n = (addr & 0x7f);
            if (int_n < INT_REG_NUM)
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
                pending [(rdata >> 5) % INT_REG_NUM] &= ~(1U << (rdata & 0x1f)); // Clear pending
                dispatch[(rdata >> 5) % INT_REG_NUM] &= ~(1U << (rdata & 0x1f)); // Clear dispatch
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

void PLIC::bind_irqdst(uint64_t *dst, uint8_t target)
{
    if (csr_connect[target]) abort();
    csr_connect[target] = 1;
    this->irqdst[target] = dst;
}

uint32_t *PLIC::get_pending()
{
    return pending;
}
