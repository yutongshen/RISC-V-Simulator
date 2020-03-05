#ifndef __TRAP__
#define __TRAP__

#include "csr_config.h"
#include <stdint.h>

class WaitForInterrupt {};

class Trap {
  char _name[32];
  uint64_t cause;

public:
  Trap(uint64_t cause);
  ~Trap();
  virtual bool has_tval() const;
  virtual uint64_t get_tval() const;
  virtual const char *get_name();
  uint64_t get_cause() const;
};

class MEMTrap : public Trap {
  uint64_t tval;

public:
  MEMTrap(uint64_t cause, uint64_t tval);
  ~MEMTrap();
  virtual bool has_tval() const;
  virtual uint64_t get_tval() const;
};

#define TRAP_DECLARE(cause, type)                                              \
  class Trap##type : public Trap {                                             \
  public:                                                                      \
    Trap##type() : Trap(cause) {}                                              \
    ~Trap##type() {}                                                           \
    const char *get_name() { return "Trap" #type; }                            \
  };

TRAP_DECLARE(CAUSE_USER_ECALL, UserEcall)
TRAP_DECLARE(CAUSE_SUPERVISOR_ECALL, SupervisorEcall)
TRAP_DECLARE(CAUSE_HYPERVISOR_ECALL, HypervisorEcall)
TRAP_DECLARE(CAUSE_MACHINE_ECALL, MachineEcall)

#define MEM_TRAP_DECLARE(cause, type)                                          \
  class Trap##type : public MEMTrap {                                          \
  public:                                                                      \
    Trap##type(uint64_t tval) : MEMTrap(cause, tval) {}                        \
    ~Trap##type() {}                                                           \
    const char *get_name() { return "Trap" #type; }                            \
  };

MEM_TRAP_DECLARE(CAUSE_MISALIGNED_FETCH, InstructionAddressMisaligned)
MEM_TRAP_DECLARE(CAUSE_INSTRUCTION_ACCESS, InstructionAccessFault)
MEM_TRAP_DECLARE(CAUSE_ILLEGAL_INSTRUCTION, IllegalInstruction)
MEM_TRAP_DECLARE(CAUSE_BREAKPOINT, Breakpoint)
MEM_TRAP_DECLARE(CAUSE_MISALIGNED_LOAD, LoadAddressMisaligned)
MEM_TRAP_DECLARE(CAUSE_LOAD_ACCESS, LoadAccessFault)
MEM_TRAP_DECLARE(CAUSE_MISALIGNED_STORE, StoreAddressMisaligned)
MEM_TRAP_DECLARE(CAUSE_STORE_ACCESS, StoreAccessFault)

MEM_TRAP_DECLARE(CAUSE_INSTRUCTION_PAGE_FAULT, InstructionPageFault)
MEM_TRAP_DECLARE(CAUSE_LOAD_PAGE_FAULT, LoadPageFault)
MEM_TRAP_DECLARE(CAUSE_STORE_PAGE_FAULT, StorePageFault)

#endif
