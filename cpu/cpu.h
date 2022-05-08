#ifndef __CPU__
#define __CPU__

#include <fstream>
#include <stack>
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
    static void get_funct_offset(char *str, const uint64_t &pc);
    std::ofstream cpu_trace;
    bool low_power;
    bool power_sta;
    uint64_t pc;
    uint64_t regs[32];
    uint64_t fregs[32];
    pCSR csr;
    pMMU mmu;
    std::stack<uint64_t> pcs;
    std::stack<uint64_t> ras;
    virtual void _init();
    void trap_handling(Trap &t, uint64_t epc);
    void take_interrupt(uint64_t interrupts);

public:
    CPU(uint64_t cpuid = 0, uint64_t pc = 0UL);
    ~CPU();
    virtual void run();
    void set_power_on(bool power_sta);
    bool get_power_sta();
    void bus_connect(pBus bus);
    bool support_extension(char ext);
    inline uint64_t get_cpuid() { return csr->mhartid; }
    inline uint64_t *get_mip_ptr() { return &csr->mip; }
    inline uint64_t get_satp() { return csr->satp; }
    inline uint64_t *get_pc() { return &pc; }
};

#endif
