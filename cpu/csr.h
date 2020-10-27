#ifndef __CSR__
#define __CSR__

#include <stdint.h>

class CSR
{
    uint64_t prv;
    uint64_t max_isa;
    uint64_t *pc_ptr;
    uint8_t max_xlen;
#define CSR_DECLARE
#include "cpu/csr_config.h"
#undef CSR_DECLARE
public:
    CSR(uint64_t cpuid, uint64_t *pc_ptr);
    ~CSR();
    static const char *csr_name(const uint32_t &addr);
    void set_csr(const uint32_t &addr, uint64_t value);
    uint64_t get_csr(const uint32_t &addr);
    bool support_extension(char ext);
    friend class CPU;
    friend class MMU;
};

#endif
