#include "cpu/trap.h"
#include "cpu/csr_config.h"
#include <stdio.h>

Trap::Trap(uint64_t cause) : cause(cause) {}

Trap::~Trap() {}

bool Trap::has_tval() const { return 0; }

uint64_t Trap::get_tval() const { return 0; }

const char *Trap::get_name() {
  if ((int64_t)cause < 0)
    sprintf(_name, "Interrupt #%lu", cause & ((1UL << 31) - 1UL));
  else
    sprintf(_name, "Exception #%lu", cause);
  return _name;
}

uint64_t Trap::get_cause() const { return cause; }

MEMTrap::MEMTrap(uint64_t cause, uint64_t tval) : Trap(cause), tval(tval) {}

MEMTrap::~MEMTrap() {}

bool MEMTrap::has_tval() const { return 1; }

uint64_t MEMTrap::get_tval() const { return tval; }
