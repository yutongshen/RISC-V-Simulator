#include "cpu/mmu.h"
#include "bus/bus.h"
#include "cpu/csr_config.h"
#include "cpu/trap.h"
#include "util//util.h"
#include <iostream>

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

Addr MMU::translate(const Addr &addr, const uint64_t &len, uint8_t type) {
  Addr paddr;

  paddr = addr;
  if (!pmp_ok(paddr, len, type)) {
    switch (type) {
    case ACCESS_TYPE_FETCH:
      throw TrapFetchAccessFault(paddr);
    case ACCESS_TYPE_LOAD:
      throw TrapLoadAccessFault(paddr);
    case ACCESS_TYPE_STORE:
      throw TrapStoreAccessFault(paddr);
    default:
      abort();
    }
  }
  return paddr;
}

uint64_t MMU::fetch(const Addr &pc) {
  uint64_t insn;
  Addr paddr(translate(pc, 4, ACCESS_TYPE_FETCH));
  if (!read(paddr, DATA_TYPE_WORD_UNSIGNED, insn))
    throw TrapFetchAccessFault(paddr);
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
    throw TrapLoadAccessFault(paddr);
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
    throw TrapStoreAccessFault(paddr);
}

bool MMU::pmp_ok(const Addr &addr, const uint64_t &len, uint8_t type) {
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
        return (csr->prv == PRV_M && !(cfg && PMP_R)) ||
               (type == ACCESS_TYPE_FETCH && (cfg && PMP_X)) ||
               (type == ACCESS_TYPE_LOAD && (cfg && PMP_R)) ||
               (type == ACCESS_TYPE_STORE && (cfg && PMP_W));
      }
    }
    base = tor;
  }

  return csr->prv == PRV_M;
}
