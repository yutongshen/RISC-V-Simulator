#ifndef __CPU__
#define __CPU__

#include "bus/bus.h"
#include "cpu/csr.h"
#include "cpu/mmu.h"
#include "cpu/trap.h"
#include "dev/device.h"

typedef MMU *pMMU;
typedef Bus *pBus;
typedef CSR *pCSR;

class CPU : public Device
{
    static const char regs_name[32][5];
    static const char fregs_name[32][5];
    static const char *fence_flag(const uint8_t &arg);
    bool low_power;
    uint64_t pc;
    uint64_t regs[32];
    uint64_t fregs[32];
    pCSR csr;
    pMMU mmu;
    virtual void _init();
    void trap_handling(const Trap &t, uint64_t epc);
    void take_interrupt(uint64_t interrupts);

public:
    CPU(uint64_t pc = 0UL);
    ~CPU();
    virtual void run();
    void bus_connect(pBus bus);
    bool support_extension(char ext);
    inline uint64_t *get_mip_ptr() { return &csr->mip; }
    inline uint64_t get_satp() { return csr->satp; }
};

#endif
