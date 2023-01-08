#include "cpu/mmu.h"
#include <iostream>
#include "bus/bus.h"
#include "cpu/csr_config.h"
#include "cpu/trap.h"
#include "util//util.h"
using namespace std;

void MMU::_init() {}

MMU::MMU(CSR *csr) : csr(csr), load_reservation_addr(-1UL) {}

MMU::~MMU() {}

bool MMU::write(const Addr &addr,
                const DataType &data_type,
                const uint64_t &wdata)
{
    if (connected) {
        return p_bus->write(addr, data_type, wdata);
    }
    return 0;
}
bool MMU::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    if (connected) {
        return p_bus->read(addr, data_type, rdata);
    }
    return 0;
}

Addr MMU::translate(const Addr &addr, const uint64_t &len, uint8_t type)
{
    Addr paddr;
    uint8_t prv(csr->prv);
    if (type != ACCESS_TYPE_FETCH && (csr->mstatus & MSTATUS_MPRV))
        prv = get_field(csr->mstatus, MSTATUS_MPP);

    paddr = trace_pt(addr, len, type, prv);
    if (!pmp_ok(paddr, len, type, csr->prv))
        throw_access_exception(addr, type);

    return paddr;
}

Addr MMU::trace_pt(const Addr &addr,
                   const uint64_t &len,
                   uint8_t type,
                   uint8_t prv)
{
    if (prv == PRV_M)
        return addr;

    // Define virtual address type
    uint8_t idx_len;
    uint8_t pt_levels;
    uint8_t pte_size;
    uint64_t pte_base;
    switch (get_field(csr->satp, SATP_MODE)) {
    case SATP_MODE_NONE:
        pt_levels = 0;
        break;
    case SATP_MODE_SV39:
        idx_len = 9;
        pt_levels = 3;
        pte_size = 8;
        pte_base = (csr->satp & SATP_PPN) << PAGE_SHIFT;
        break;
    case SATP_MODE_SV48:
        idx_len = 9;
        pt_levels = 4;
        pte_size = 8;
        pte_base = (csr->satp & SATP_PPN) << PAGE_SHIFT;
        break;
    case SATP_MODE_SV57:
        idx_len = 9;
        pt_levels = 5;
        pte_size = 8;
        pte_base = (csr->satp & SATP_PPN) << PAGE_SHIFT;
        break;
    case SATP_MODE_SV64:
        idx_len = 9;
        pt_levels = 6;
        pte_size = 8;
        pte_base = (csr->satp & SATP_PPN) << PAGE_SHIFT;
        break;
    default:
        abort();
    }

    // SATP_MODE_NONE mean virtual address == physical address
    if (!pt_levels)
        return addr;

    // Check virtual address is legal
    uint8_t va_len(idx_len * pt_levels + PAGE_SHIFT);
    // Virtual address must be less than or equal to 64
    if (va_len >= 64)
        va_len = 64;
    uint64_t mask((1UL << (64 - (va_len - 1))) - 1UL);
    uint64_t va_masked((addr >> (va_len - 1)) & mask);
    if (va_masked && va_masked != mask)
        pt_levels = 0;

    uint8_t i(pt_levels);
    uint64_t pn_shift(idx_len * pt_levels);
    uint64_t base(pte_base);
    while (i--) {
        // Calculate PTE's physical address
        pn_shift -= idx_len;
        uint64_t vpn_i((addr >> pn_shift >> PAGE_SHIFT) &
                       ((1UL << idx_len) - 1UL));
        uint64_t pte_paddr(base + vpn_i * pte_size);

        // Load PTE
        uint64_t pte;
        if (!read(pte_paddr, DATA_TYPE_DWORD, pte))
            throw_access_exception(addr, type);
        if (!pmp_ok(pte_paddr, 8, ACCESS_TYPE_LOAD, PRV_S))
            throw_access_exception(addr, type);
        uint64_t ppn(pte >> PTE_PPN_SHIFT);

        base = ppn << PAGE_SHIFT;
        // Internal node
        if ((pte & PTE_V) && !(pte & (PTE_R | PTE_W | PTE_X)))
            continue;

        // Check page fault
        bool fault(0);

        // Check user page access
        fault |= (pte & PTE_U) ? prv == PRV_S && (type == ACCESS_TYPE_FETCH ||
                                                  !(csr->mstatus & MSTATUS_SUM))
                               : prv == PRV_U;

        // Check PTE's valid
        fault |= !(pte & PTE_V);

        // Check W == 1 => R == 1
        fault |= (pte & PTE_W) && !(pte & PTE_R);

        // Check permission
        switch (type) {
        case ACCESS_TYPE_FETCH:
            fault |= !(pte & PTE_X);
            break;
        case ACCESS_TYPE_LOAD:
            fault |= !(pte & PTE_R) &&
                     !(get_field(csr->mstatus, MSTATUS_MXR) && (pte & PTE_X));
            break;
        case ACCESS_TYPE_STORE:
            fault |= !(pte & PTE_W);
            break;
        default:
            abort();
        }

        // Check ppn[i-1:0]
        fault |= ppn & ((1UL << pn_shift) - 1UL);

        if (fault)
            break;

        // Set dirty by hardware
        uint64_t ad(PTE_A);
        if (type == ACCESS_TYPE_STORE)
            ad |= PTE_D;
        if ((pte & ad) != ad) {
            pte |= ad;
            if (!pmp_ok(pte_paddr, 8, ACCESS_TYPE_STORE, PRV_S))
                throw_access_exception(addr, type);
            if (!write(pte_paddr, DATA_TYPE_DWORD, pte))
                throw_access_exception(addr, type);
        }

        // Return physical address
        uint64_t vpn_pffset(addr & ((1UL << pn_shift << PAGE_SHIFT) - 1UL));
        return base | vpn_pffset;
    }

    switch (type) {
    case ACCESS_TYPE_FETCH:
        throw TrapInstructionPageFault(addr);
    case ACCESS_TYPE_LOAD:
        throw TrapLoadPageFault(addr);
    case ACCESS_TYPE_STORE:
        throw TrapStorePageFault(addr);
    default:
        abort();
    }
}

void MMU::throw_access_exception(const Addr &addr, uint8_t type)
{
    switch (type) {
    case ACCESS_TYPE_FETCH:
        throw TrapInstructionAccessFault(addr);
    case ACCESS_TYPE_LOAD:
        throw TrapLoadAccessFault(addr);
    case ACCESS_TYPE_STORE:
        throw TrapStoreAccessFault(addr);
    default:
        abort();
    }
}

uint64_t MMU::fetch(const Addr &pc, const uint64_t &alignment_mask)
{
    if (pc & ~alignment_mask)
        throw TrapInstructionAddressMisaligned(pc);
    if (pc & 3UL) {
        uint64_t insn[2];
        for (uint8_t i = 0; i < 2; ++i) {
            Addr paddr(translate(pc + (i << 1), 2, ACCESS_TYPE_FETCH));
            if (!read(paddr, DATA_TYPE_HWORD_UNSIGNED, insn[i]))
                throw TrapInstructionAccessFault(paddr);
        }
        return insn[1] << 16 | insn[0];
    } else {
        uint64_t insn;
        Addr paddr(translate(pc, 4, ACCESS_TYPE_FETCH));
        if (!read(paddr, DATA_TYPE_WORD_UNSIGNED, insn))
            throw TrapInstructionAccessFault(paddr);
        return insn;
    }
}

uint64_t MMU::load(const Addr &addr, const DataType &data_type)
{
    uint64_t rdata(0);
    Addr paddr;
    switch (data_type) {
    case DATA_TYPE_DWORD:
        if (addr & 7UL) {
            rdata |= load(addr, DATA_TYPE_WORD_UNSIGNED);
            rdata |= load(addr + 4, DATA_TYPE_WORD_UNSIGNED) << 32;
            return rdata;
        }
        paddr = translate(addr, 8, ACCESS_TYPE_LOAD);
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        if (addr & 3UL) {
            rdata |= load(addr, DATA_TYPE_HWORD_UNSIGNED);
            rdata |= load(addr + 2,
                          HALF_DATA(DATA_TYPE_WORD_UNSIGNED, GET_SIGNED(data_type))) << 16;
            return rdata;
        }
        paddr = translate(addr, 4, ACCESS_TYPE_LOAD);
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        if (addr & 1UL) {
            rdata |= load(addr, DATA_TYPE_BYTE_UNSIGNED);
            rdata |= load(addr + 1,
                          HALF_DATA(DATA_TYPE_WORD_UNSIGNED, GET_SIGNED(data_type))) << 8;
            return rdata;
        }
        paddr = translate(addr, 2, ACCESS_TYPE_LOAD);
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        paddr = translate(addr, 1, ACCESS_TYPE_LOAD);
        break;
    default:
        abort();
    }
    if (!read(paddr, data_type, rdata))
        throw TrapLoadAccessFault(paddr);
    return rdata;
}

void MMU::store(const Addr &addr,
                const DataType &data_type,
                const uint64_t &wdata)
{
    Addr paddr;
    // cout << hex << "STORE(" << addr << ", " << (int)data_type << ", " <<
    // wdata
    //     << ")" << endl;
    switch (data_type) {
    case DATA_TYPE_DWORD:
        if (addr & 7UL) {
            store(addr, DATA_TYPE_WORD_UNSIGNED, wdata);
            store(addr + 4, DATA_TYPE_WORD_UNSIGNED, wdata >> 32);
            return;
        }
        paddr = translate(addr, 8, ACCESS_TYPE_STORE);
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        if (addr & 3UL) {
            store(addr, DATA_TYPE_HWORD_UNSIGNED, wdata);
            store(addr + 2, DATA_TYPE_HWORD_UNSIGNED, wdata >> 16);
            return;
        }
        paddr = translate(addr, 4, ACCESS_TYPE_STORE);
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        if (addr & 1UL) {
            store(addr, DATA_TYPE_BYTE_UNSIGNED, wdata);
            store(addr + 1, DATA_TYPE_BYTE_UNSIGNED, wdata >> 8);
            return;
        }
        paddr = translate(addr, 2, ACCESS_TYPE_STORE);
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        paddr = translate(addr, 1, ACCESS_TYPE_STORE);
        break;
    default:
        abort();
    }
    if (!write(paddr, data_type, wdata))
        throw TrapStoreAccessFault(paddr);
}

uint64_t MMU::amo_operate(const Addr &addr,
                          const DataType &type,
                          const uint64_t &src,
                          uint64_t (*func)(const uint64_t &rdata,
                                           const uint64_t &src))
{
    if (addr & GET_SIZE(type) - 1)
        throw TrapStoreAddressMisaligned(addr);
    try {
        uint64_t x(load(addr, type));
        store(addr, type, func(x, src));
        return x;
    } catch (TrapLoadPageFault &t) {
        throw TrapStorePageFault(t.get_tval());
    } catch (TrapLoadAccessFault &t) {
        throw TrapStoreAccessFault(t.get_tval());
    } catch (TrapStorePageFault &t) {
        throw TrapStorePageFault(t.get_tval());
    } catch (TrapStoreAccessFault &t) {
        throw TrapStoreAccessFault(t.get_tval());
    } catch (Trap &t) {
        throw t;
    }
}

bool MMU::pmp_ok(const Addr &addr,
                 const uint64_t &len,
                 uint8_t type,
                 uint8_t prv)
{
    uint64_t base(0);
    uint8_t a_field;

    for (uint8_t i = 0; i < 16; ++i) {
        uint64_t tor(csr->pmpaddr[i] << 2);
        uint8_t cfg(csr->pmpcfg[i]);
        if (a_field = get_field(cfg, PMP_A)) {
            bool is_tor(a_field == PMP_TOR);
            bool is_na4(a_field == PMP_NA4);
            uint64_t mask((csr->pmpaddr[i] << 1) | (!is_na4));
            mask = ~(mask & ~(mask + 1)) << 2;

            bool any_match(0);
            bool all_match(1);
            for (uint64_t offset = 0; offset < len; offset += 4) {
                uint64_t _addr = addr + offset;
                bool napot_match(!((_addr ^ tor) & mask));
                bool tor_match(base <= _addr && _addr < tor);
                bool match(is_tor ? tor_match : napot_match);
                any_match |= match;
                all_match &= match;
            }

            if (any_match) {
                if (!all_match)
                    return 0;
                return (prv == PRV_M && !(cfg && PMP_L)) ||
                       (type == ACCESS_TYPE_FETCH && (cfg && PMP_X)) ||
                       (type == ACCESS_TYPE_LOAD && (cfg && PMP_R)) ||
                       (type == ACCESS_TYPE_STORE && (cfg && PMP_W));
            }
        }
        base = tor;
    }

    return prv == PRV_M;
}

void MMU::acquire_load_reservation(const Addr &addr)
{
    uint64_t rdata;
    Addr paddr(translate(addr, 1, ACCESS_TYPE_LOAD));
    if (read(paddr, DATA_TYPE_BYTE, rdata))
        load_reservation_addr = paddr;
    else
        throw TrapLoadAccessFault(addr);
}

bool MMU::check_load_reservation(const Addr &addr)
{
    uint64_t rdata;
    Addr paddr(translate(addr, 1, ACCESS_TYPE_STORE));
    if (read(paddr, DATA_TYPE_BYTE, rdata))
        return load_reservation_addr == paddr;
    else
        throw TrapStoreAccessFault(addr);
}
