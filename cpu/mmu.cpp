#include "cpu/mmu.h"
#include "bus/bus.h"
#include "cpu/csr_config.h"
#include "cpu/trap.h"
#include "util//util.h"
#include <iostream>
using namespace std;

void MMU::_init() {}

MMU::MMU(CSR *csr) : csr(csr) {}

MMU::~MMU() {}

bool MMU::write(const Addr &addr, const DataType &data_type,
                const uint64_t &wdata) {
  if (connected) {
    return p_bus->write(addr, data_type, wdata);
  }
  return 0;
}
bool MMU::read(const Addr &addr, const DataType &data_type, uint64_t &rdata) {
  if (connected) {
    return p_bus->read(addr, data_type, rdata);
  }
  return 0;
}

Addr MMU::trace_pt(const Addr &addr, const uint64_t &len, uint8_t type, uint8_t prv) {
  if (prv == PRV_M) return addr;

  uint8_t idx_len;
  uint8_t pt_levels;
  uint8_t pte_size;
  uint64_t pte_base;
  
  switch (get_field(csr->satp, SATP_MODE)) {
    case SATP_MODE_NONE:
      idx_len = 0;
      pt_levels = 0;
      pte_size = 0;
      pte_base = 0;
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
    default: abort();
  }

  if (!pt_levels) return addr;

  // check virtual address
  uint8_t va_len(idx_len * pt_levels + PAGE_SHIFT);
  uint64_t mask((1UL << 64 - (va_len - 1)) - 1UL);
  uint64_t va_remain((addr >> (va_len - 1)) & mask);
  if (va_remain && va_remain && va_remain ^ mask) // overflow
    pt_levels = 0; // goto exception


  uint64_t base(pte_base);
  uint8_t i(pt_levels);
  uint8_t pn_shift(pt_levels * idx_len);
  while (i--) {
    // Calculate PTE address
    pn_shift -= idx_len;
    uint64_t idx_mask(((1UL << idx_len) - 1UL) << (pn_shift + PAGE_SHIFT));
    uint64_t idx(get_field(addr, idx_mask));
    uint64_t pte_paddr(base + pte_size * idx);

    // Load PTE
    uint64_t pte;
    if (!read(pte_paddr, DATA_TYPE_DWORD, pte))
      throw_access_exception(addr, type);
    if (!pmp_ok(pte_paddr, pte_size, ACCESS_TYPE_LOAD, PRV_S))
      throw_access_exception(addr, type);

    // cout << "PTE_BASE : " << pte_base << endl;
    // cout << "PTE : " << pte << endl;


    uint64_t ppn(pte >> PTE_PPN_SHIFT);
    base = ppn << PAGE_SHIFT;

    // Internal node
    if ((pte & (PTE_V | PTE_R | PTE_W | PTE_X)) == PTE_V) {
      continue;
    }
    
    // Check page fault
    bool fault(0);

    // Invalid PTE
    fault |= !(pte & PTE_V);
    
    // cout << "Fault : " << fault << endl;

    // If W = 1, R = 1 
    fault |= (pte & PTE_W) && !(pte & PTE_R);

    // User page access
    fault |= (pte & PTE_U) ? prv == PRV_S && !(csr->mstatus & MSTATUS_SUM):
                             prv == PRV_U;

    // Check permission
    switch (type) {
      case ACCESS_TYPE_FETCH:
        fault |= !(pte & PTE_X);
        break;
      case ACCESS_TYPE_LOAD:
        fault |= !((pte & PTE_R) || ((csr->mstatus & MSTATUS_MXR) && (pte & PTE_R)));
        break;
      case ACCESS_TYPE_STORE:
        fault |= !(pte & PTE_W);
        break;
      default: abort();
    }
              
    // If leaf node, PPN[i-1] ~ PPN[0] must be zeres
    fault |= ppn & ((1UL << pn_shift) - 1UL);

    if (fault) break;
    
    uint64_t ad(PTE_A);
    if (type == ACCESS_TYPE_STORE) ad |= PTE_D;
    if ((pte & ad) != ad) {
      pte |= ad;
      if (!pmp_ok(pte_paddr, pte_size, ACCESS_TYPE_STORE, PRV_S))
        throw_access_exception(addr, type);
      if (!write(pte_paddr, DATA_TYPE_DWORD, pte))
        throw_access_exception(addr, type);
    }
    
    // Physical address is combined with ppn[n] ~ ppn[i], vpn[i-1] ~ vpn[0] and page-offset
    uint64_t vpn_poffset(addr & ((1UL << (pn_shift + PAGE_SHIFT)) - 1UL));
    return base | vpn_poffset;
  }

  // Page fault
  switch (type) {
    case ACCESS_TYPE_FETCH: throw TrapInstructionPageFault(addr);
    case ACCESS_TYPE_LOAD: throw TrapLoadPageFault(addr);
    case ACCESS_TYPE_STORE: throw TrapStorePageFault(addr);
    default: abort();
  }
}

Addr MMU::translate(const Addr &addr, const uint64_t &len, uint8_t type) {
  Addr paddr;
  
  uint8_t prv(csr->prv);
  if (type != ACCESS_TYPE_FETCH && (csr->mstatus & MSTATUS_MPRV))
    prv = get_field(csr->mstatus, MSTATUS_MPP);
  
  // cout << "ADDR : " << addr << endl;
  // cout << "_PRV : " << (int) prv << endl;
  
  paddr = trace_pt(addr, len, type, prv);
  // cout << "PADDR : " << paddr << endl;
  
  if (!pmp_ok(paddr, len, type, csr->prv))
    throw_access_exception(addr, type);

  return paddr;
}

void MMU::throw_access_exception(const Addr &addr, uint8_t type) {
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

uint64_t MMU::fetch(const Addr &pc) {
  uint64_t insn;
  Addr paddr(translate(pc, 4, ACCESS_TYPE_FETCH));
  if (!read(paddr, DATA_TYPE_WORD_UNSIGNED, insn))
    throw TrapInstructionAccessFault(pc);
  return insn;
}

uint64_t MMU::load(const Addr &addr, const DataType &data_type) {
  uint64_t rdata;
  uint64_t paddr;
  switch (data_type) {
  case DATA_TYPE_DWORD:
    paddr = translate(addr, 8, ACCESS_TYPE_LOAD);
    break;
  case DATA_TYPE_WORD:
  case DATA_TYPE_WORD_UNSIGNED:
    paddr = translate(addr, 4, ACCESS_TYPE_LOAD);
    break;
  case DATA_TYPE_HWORD:
  case DATA_TYPE_HWORD_UNSIGNED:
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
    throw TrapLoadAccessFault(addr);
  return rdata;
}

void MMU::store(const Addr &addr, const DataType &data_type,
                const uint64_t &wdata) {
  uint64_t paddr;
  switch (data_type) {
  case DATA_TYPE_DWORD:
    paddr = translate(addr, 8, ACCESS_TYPE_STORE);
    break;
  case DATA_TYPE_WORD:
  case DATA_TYPE_WORD_UNSIGNED:
    paddr = translate(addr, 4, ACCESS_TYPE_STORE);
    break;
  case DATA_TYPE_HWORD:
  case DATA_TYPE_HWORD_UNSIGNED:
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
    throw TrapStoreAccessFault(addr);
}

bool MMU::pmp_ok(const Addr &addr, const uint64_t &len, uint8_t type, uint8_t prv) {
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
        return (prv == PRV_M && !(cfg && PMP_R)) ||
               (type == ACCESS_TYPE_FETCH && (cfg && PMP_X)) ||
               (type == ACCESS_TYPE_LOAD && (cfg && PMP_R)) ||
               (type == ACCESS_TYPE_STORE && (cfg && PMP_W));
      }
    }
    base = tor;
  }

  return prv == PRV_M;
}
