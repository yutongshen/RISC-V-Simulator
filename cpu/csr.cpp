#include "cpu/csr.h"
#include "cpu/rule_define.h"
#include "util/util.h"

CSR::CSR(uint64_t cpuid, uint64_t *pc_ptr) : mhartid(cpuid), prv(PRV_M), pc_ptr(pc_ptr)
{
#define CSR_READ_DECLARE(addr, csr) csr = 0;
#include "cpu/csr_config.h"
#undef CSR_READ_DECLARE
    max_isa = 2UL << 62;  // XLEN = 64
    max_isa |= 1UL << ('I' - 'A');
    max_isa |= 1UL << ('M' - 'A');
    max_isa |= 1UL << ('A' - 'A');
    max_isa |= 1UL << ('F' - 'A');
    max_isa |= 1UL << ('D' - 'A');
    max_isa |= 1UL << ('C' - 'A');
    max_isa |= 1UL << ('S' - 'A');
    misa = max_isa;

    mstatus = set_field(mstatus, MSTATUS_UXL, 2);
    mstatus = set_field(mstatus, MSTATUS_SXL, 2);

    // pmpaddr[0] = -1UL;
    // pmpcfg[0] = PMP_R | PMP_W | PMP_X | set_field(0, PMP_A, PMP_NAPOT);
}

CSR::~CSR() {}

const char *CSR::csr_name(const uint32_t &addr)
{
    switch (addr) {
#define CSR_NAME_DECLARE(addr, name) \
    case addr:                       \
        return #name;
#include "cpu/csr_config.h"
#undef CSR_NAME_DECLARE
    }
}

void CSR::set_csr(const uint32_t &addr, uint64_t value)
{
    uint64_t _mask(0);
    uint64_t *csr_ptr(0);

    if (addr >= CSR_PMPADDR0_ADDR && addr < CSR_PMPADDR0_ADDR + 16) {
        uint8_t i(addr - CSR_PMPADDR0_ADDR);
        bool is_locked(pmpcfg[i] & PMP_L);
        bool nxt_locked(i + 1 < 16 && pmpcfg[i + 1] & PMP_L);
        bool nxt_tor(i + 1 < 16 && get_field(pmpcfg[i + 1], PMP_A) == PMP_TOR);
        if (!is_locked && !(nxt_locked && nxt_tor)) {
            pmpaddr[i] = value & PMPADDR_MASK;
        }
        return;
    }

    if (addr >= CSR_PMPCFG0_ADDR && addr < CSR_PMPCFG0_ADDR + 4) {
        uint8_t *_val_ptr((uint8_t *) &value);
        uint8_t i0((addr - CSR_PMPCFG0_ADDR) << 2);
        for (uint8_t i = 0; i < 8; ++i) {
            if (!(pmpcfg[i0 + i] & PMP_L)) {
                uint8_t cfg(_val_ptr[i] &
                            (PMP_R | PMP_W | PMP_X | PMP_A | PMP_L));
                cfg &= ~((cfg & PMP_R) ? 0 : PMP_W);  // if R=0 => W=0
                pmpcfg[i0 + i] = cfg;
            }
        }
        return;
    }

    uint64_t s_ints, all_ints;
    s_ints = MIP_SSIP | MIP_STIP | MIP_SEIP;             // Supervise mode
    all_ints = s_ints | MIP_MSIP | MIP_MTIP | MIP_MEIP;  // Add machine mode

    switch (addr) {
#define CSR_WRITE_DECLARE(addr, csr, mask) \
    case addr:                             \
        csr_ptr = &csr;                    \
        _mask = mask;                      \
        break;
#include "cpu/csr_config.h"
#undef CSR_WRITE_DECLARE
    case CSR_FCSR_ADDR:
        set_csr(CSR_FFLAGS_ADDR,
                (fflags & ~FFLAGS_MASK) |
                    ((value >> FCSR_FFLAGS_SHIFT) & FFLAGS_MASK));
        set_csr(CSR_FRM_ADDR,
                (frm & ~FRM_MASK) | ((value >> FCSR_FRM_SHIFT) & FRM_MASK));
        return;
    // Supervispr
    case CSR_SSTATUS_ADDR:
        _mask = SSTATUS_MASK & ~MSTATUS_UXL;
        return set_csr(CSR_MSTATUS_ADDR, (mstatus & ~_mask) | (value & _mask));

    case CSR_SIE_ADDR:
        return set_csr(CSR_MIE_ADDR, (mie & ~mideleg) | (value & mideleg));

    case CSR_SEPC_ADDR:
        sepc = value & ~1UL;
        return;

    case CSR_SIP_ADDR:
        _mask = MIP_SSIP & mideleg;
        return set_csr(CSR_MIP_ADDR, (mie & ~_mask) | (value & _mask));

    // Machine
    case CSR_MSTATUS_ADDR:
        _mask = MSTATUS_SIE | MSTATUS_SPIE | MSTATUS_MIE | MSTATUS_MPIE |
                MSTATUS_MPRV | MSTATUS_SUM | MSTATUS_MXR | MSTATUS_TW |
                MSTATUS_TVM | MSTATUS_TSR | MSTATUS_UXL | MSTATUS_SXL |
                MSTATUS_MPP | MSTATUS_SPP | MSTATUS_FS | MSTATUS_XS;
        mstatus = (mstatus & ~_mask) | (value & _mask);
        mstatus = set_field(mstatus, MSTATUS_UXL, 2);
        mstatus = set_field(mstatus, MSTATUS_SXL, 2);
        return;
    case CSR_MISA_ADDR:
        // Avoid unalign PC
        if (*pc_ptr & 2UL)
            value |= 1UL << ('C' - 'A');
        if (!(value & (1UL << ('F' - 'A'))))
            value &= ~(1UL << ('D' - 'A'));
        _mask = 0;
        _mask |= 1UL << ('M' - 'A');
        _mask |= 1UL << ('A' - 'A');
        _mask |= 1UL << ('F' - 'A');
        _mask |= 1UL << ('D' - 'A');
        _mask |= 1UL << ('C' - 'A');
        _mask &= max_isa;
        misa = (misa & ~_mask) | (value & _mask);
        return;
    case CSR_MEDELEG_ADDR:
        _mask = (1 << CAUSE_MISALIGNED_FETCH) | (1 << CAUSE_BREAKPOINT) |
                (1 << CAUSE_USER_ECALL) | (1 << CAUSE_INSTRUCTION_PAGE_FAULT) |
                (1 << CAUSE_LOAD_PAGE_FAULT) | (1 << CAUSE_STORE_PAGE_FAULT);
        medeleg = (medeleg & ~_mask) | (value & _mask);
        return;
    case CSR_MIDELEG_ADDR:
        _mask = s_ints;
        mideleg = (mideleg & ~_mask) | (value & _mask);
        return;
    case CSR_MIE_ADDR:
        _mask = all_ints;
        mie = (mie & ~_mask) | (value & _mask);
        return;
    case CSR_MTVEC_ADDR:
        _mask = ~0x2UL;
        mtvec = (mtvec & ~_mask) | (value & _mask);
        return;
    case CSR_MIP_ADDR:
        _mask = all_ints & (MIP_SSIP | MIP_STIP | MIP_SEIP | MIP_MSIP);
        mip = (mip & ~_mask) | (value & _mask);
        return;
    case CSR_MEPC_ADDR:
        mepc = value & ~1UL;
        return;
    }
    if (csr_ptr)
        *csr_ptr = (*csr_ptr & ~_mask) | (value & _mask);
}
uint64_t CSR::get_csr(const uint32_t &addr)
{
    switch (addr) {
#define CSR_READ_DECLARE(addr, csr) \
    case addr:                      \
        return csr;
#include "cpu/csr_config.h"
#undef CSR_READ_DECLARE
    case CSR_FCSR_ADDR:
        return (frm & FRM_MASK) << FCSR_FRM_SHIFT | (fflags & FFLAGS_MASK)
                                                        << FCSR_FFLAGS_SHIFT;
    case CSR_SSTATUS_ADDR:
        return mstatus & SSTATUS_MASK;
    case CSR_SIE_ADDR:
        return mie & mideleg;
    case CSR_SEPC_ADDR:
        return sepc & pc_alignment_mask;
    case CSR_SIP_ADDR:
        return mip & mideleg;
    case CSR_MEPC_ADDR:
        return mepc & pc_alignment_mask;
    }

    return 0;
}

bool CSR::support_extension(char ext)
{
    return misa & (1UL << (ext - 'A'));
}
