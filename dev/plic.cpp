#include "plic.h"
#include "cpu/csr_config.h"
#include "mmap/plic_reg.h"
#include "util/util.h"

void PLIC::_init() {}

PLIC::PLIC()
    : csr_connect{0},
      prior{0},
      pending{0},
      enable{0},
      int_id{0},
      threshold{0},
      Device(),
      Slave(0x4000000)
{
}

PLIC::~PLIC() {}

void PLIC::run()
{
    uint32_t max_id, int_valid, id;

    for (int i = 0; i < TARGET_NUM; ++i) {
        if (!csr_connect[i])
            continue;
#ifndef IRQ_PREEMPTION
        if (*(irqdst[i]) & (1U << irqoffset[i]))
            continue;
#endif

        max_id = 0;
        for (int j = 0; j < INT_REG_NUM; ++j) {
            id = j << 5;
            int_valid = enable[i * INT_REG_NUM + j] & pending[j] & ~dispatch[j];
            while (int_valid) {
                if (int_valid & 0x1) {
                    max_id = prior[max_id] < prior[id] ? id : max_id;
                }
                int_valid >>= 1;
                ++id;
            }
        }
        if (max_id && prior[max_id] > threshold[i]) {
            *(irqdst[i]) |= 1UL << irqoffset[i];
            int_id[i] = max_id;
            dispatch[max_id >> 5] |= 1U << (max_id & 0x1f);
        }
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
    // printf("[%lx] = %lx\n", addr, _wdata);

    if (addr >= RG_PRIOR_TH)  // Priorty threshole, Claim/Complete
    {
        tar_n = (addr - RG_PRIOR_TH) >> 12;
        if (tar_n < TARGET_NUM) {
            if (addr & 0x4) {
                int_id[tar_n] = 0;
                *(irqdst[tar_n]) &= ~(1U << irqoffset[tar_n]);  // Clear target
            } else {
                threshold[tar_n] = _wdata;
            }
        }
    } else if (addr >= RG_ENABLE)  // Enable
    {
        tar_n = (addr - RG_ENABLE) >> 7;
        if (tar_n < TARGET_NUM) {
            int_n = addr & 0x7f;
            if (int_n < INT_REG_NUM) {
                enable[tar_n * INT_REG_NUM + int_n] = _wdata;
            }
        }
    } else if (addr >= RG_PEND)  // Pending
    {
        /* Write ignore */
    } else if (addr >= RG_PRIOR)  // Priorty
    {
        int_n = (addr - RG_PRIOR) >> 2;
        if (int_n < INT_NUM) {
            prior[int_n] = _wdata;
        }
    }
    prior[0] = 0;
    return 1;
}

bool PLIC::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    uint32_t int_n, tar_n;

    rdata = 0L;

    if (addr >= RG_PRIOR_TH)  // Priorty threshole, Claim/Complete
    {
        tar_n = (addr - RG_PRIOR_TH) >> 12;
        if (tar_n < TARGET_NUM) {
            if (addr & 0x4) {
                rdata = int_id[tar_n];
                pending[(rdata >> 5) % INT_REG_NUM] &=
                    ~(1U << (rdata & 0x1f));  // Clear pending
                dispatch[(rdata >> 5) % INT_REG_NUM] &=
                    ~(1U << (rdata & 0x1f));  // Clear dispatch
            } else {
                rdata = threshold[tar_n];
            }
        }
    } else if (addr >= RG_ENABLE)  // Enable
    {
        tar_n = (addr - RG_ENABLE) >> 7;
        if (tar_n < TARGET_NUM) {
            int_n = (addr & 0x7f);
            if (int_n < INT_REG_NUM) {
                rdata = enable[tar_n * INT_REG_NUM + int_n];
            }
        }
    } else if (addr >= RG_PEND)  // Pending
    {
        int_n = (addr - RG_PEND) >> 2;
        if (int_n < INT_REG_NUM) {
            rdata = pending[int_n];
        }
    } else if (addr >= RG_PRIOR)  // Priorty
    {
        int_n = (addr - RG_PRIOR) >> 2;
        if (int_n < INT_NUM) {
            rdata = prior[int_n];
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

void PLIC::bind_irqdst(uint64_t *dst, uint8_t offset, uint8_t target)
{
    if (csr_connect[target])
        abort();
    csr_connect[target] = 1;
    this->irqdst[target] = dst;
    this->irqoffset[target] = offset;
}

uint32_t *PLIC::get_pending()
{
    return pending;
}

void PLIC::set_pending(int32_t irq_id, uint8_t value)
{
    if (irq_id < 0)
        abort();

    if (value)
        pending[irq_id >> 5] |= (1 << (irq_id & 0x1f));
    else
        pending[irq_id >> 5] &= ~(1 << (irq_id & 0x1f));
}
