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

#define INSN_LEN(insn) ((insn & 0x3) == 0x3 ? 4 : 2)
#define OPCODE(insn) (((struct r_type_insn *) &insn)->opcode)
#define OPCODE16(insn) (((struct c_addi4spn_insn *) &insn)->opcode)
#define RD(insn) (((struct r_type_insn *) &insn)->rd)
#define RS1(insn) (((struct r_type_insn *) &insn)->rs1)
#define RS2(insn) (((struct r_type_insn *) &insn)->rs2)
#define FUNC3(insn) (((struct r_type_insn *) &insn)->func3)
#define FUNC7(insn) (((struct r_type_insn *) &insn)->func7)
#define FUNC5(insn) (((struct amo_insn *) &insn)->func5)
#define FUNC12(insn) (((struct i_type_insn *) &insn)->imm_11_0)
#define CSR_ADDR(insn) (((struct i_type_insn *) &insn)->imm_11_0)
#define AQ(insn) (((struct amo_insn *) &insn)->aq)
#define RL(insn) (((struct amo_insn *) &insn)->rl)
#define PRED(insn) (((struct fence_insn *) &insn)->pred)
#define SUCC(insn) (((struct fence_insn *) &insn)->succ)
#define SHAMT(insn) (((struct r_type_insn *) &insn)->rs2)
#define SHAMT64(insn) (((struct slli_insn *) &insn)->shamt)
#define IMM_I(insn) (sext(((struct i_type_insn *) &insn)->imm_11_0, 12))
#define IMM_S(insn)                                           \
    (sext(((struct s_type_insn *) &insn)->imm_11_5, 7) << 5 | \
     ((struct s_type_insn *) &insn)->imm_4_0)
#define IMM_B(insn)                                             \
    (sext(((struct b_type_insn *) &insn)->imm_12_12, 1) << 12 | \
     ((struct b_type_insn *) &insn)->imm_11_11 << 11 |          \
     ((struct b_type_insn *) &insn)->imm_10_5 << 5 |            \
     ((struct b_type_insn *) &insn)->imm_4_1 << 1)
#define IMM_U(insn) (sext(((struct u_type_insn *) &insn)->imm_31_12, 20) << 12)
#define IMM_J(insn)                                             \
    (sext(((struct j_type_insn *) &insn)->imm_20_20, 1) << 20 | \
     ((struct j_type_insn *) &insn)->imm_19_12 << 12 |          \
     ((struct j_type_insn *) &insn)->imm_11_11 << 11 |          \
     ((struct j_type_insn *) &insn)->imm_10_1 << 1)
#define IMM_C_ADDI4SPN(insn)                            \
    (((struct c_addi4spn_insn *) &insn)->imm_9_6 << 6 | \
     ((struct c_addi4spn_insn *) &insn)->imm_5_4 << 4 | \
     ((struct c_addi4spn_insn *) &insn)->imm_3_3 << 3 | \
     ((struct c_addi4spn_insn *) &insn)->imm_2_2 << 2)
#define IMM_C_SLD(insn)                            \
    (((struct c_sld_insn *) &insn)->imm_7_6 << 6 | \
     ((struct c_sld_insn *) &insn)->imm_5_3 << 3)
#define IMM_C_SLW(insn)                            \
    (((struct c_slw_insn *) &insn)->imm_6_6 << 6 | \
     ((struct c_slw_insn *) &insn)->imm_5_3 << 3 | \
     ((struct c_slw_insn *) &insn)->imm_2_2 << 2)
#define IMM_C_ADDI(insn)                                     \
    (sext(((struct c_addi_insn *) &insn)->imm_5_5, 1) << 5 | \
     ((struct c_addi_insn *) &insn)->imm_4_0)
#define IMM_C_J(insn)                                        \
    (sext(((struct c_j_insn *) &insn)->imm_11_11, 1) << 11 | \
     ((struct c_j_insn *) &insn)->imm_10_10 << 10 |          \
     ((struct c_j_insn *) &insn)->imm_9_8 << 8 |             \
     ((struct c_j_insn *) &insn)->imm_7_7 << 7 |             \
     ((struct c_j_insn *) &insn)->imm_6_6 << 6 |             \
     ((struct c_j_insn *) &insn)->imm_5_5 << 5 |             \
     ((struct c_j_insn *) &insn)->imm_4_4 << 4 |             \
     ((struct c_j_insn *) &insn)->imm_3_1 << 1)
#define IMM_C_ADDI16SP(insn)                                     \
    (sext(((struct c_addi16sp_insn *) &insn)->imm_9_9, 1) << 9 | \
     ((struct c_addi16sp_insn *) &insn)->imm_8_7 << 7 |          \
     ((struct c_addi16sp_insn *) &insn)->imm_6_6 << 6 |          \
     ((struct c_addi16sp_insn *) &insn)->imm_5_5 << 5 |          \
     ((struct c_addi16sp_insn *) &insn)->imm_4_4 << 4)
#define IMM_C_LUI(insn)                                        \
    (sext(((struct c_lui_insn *) &insn)->imm_17_17, 1) << 17 | \
     ((struct c_lui_insn *) &insn)->imm_16_12 << 12)
#define IMM_C_B(insn)                                     \
    (sext(((struct c_b_insn *) &insn)->imm_8_8, 1) << 8 | \
     ((struct c_b_insn *) &insn)->imm_7_6 << 6 |          \
     ((struct c_b_insn *) &insn)->imm_5_5 << 5 |          \
     ((struct c_b_insn *) &insn)->imm_4_3 << 3 |          \
     ((struct c_b_insn *) &insn)->imm_2_1 << 1)
#define IMM_C_LDSP(insn)                            \
    (((struct c_ldsp_insn *) &insn)->imm_8_6 << 6 | \
     ((struct c_ldsp_insn *) &insn)->imm_5_5 << 5 | \
     ((struct c_ldsp_insn *) &insn)->imm_4_3 << 3)
#define IMM_C_LWSP(insn)                            \
    (((struct c_lwsp_insn *) &insn)->imm_7_6 << 6 | \
     ((struct c_lwsp_insn *) &insn)->imm_5_5 << 5 | \
     ((struct c_lwsp_insn *) &insn)->imm_4_2 << 2)
#define IMM_C_SDSP(insn)                            \
    (((struct c_sdsp_insn *) &insn)->imm_8_6 << 6 | \
     ((struct c_sdsp_insn *) &insn)->imm_5_3 << 3)
#define IMM_C_SWSP(insn)                            \
    (((struct c_swsp_insn *) &insn)->imm_7_6 << 6 | \
     ((struct c_swsp_insn *) &insn)->imm_5_2 << 2)

struct r_type_insn {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t func3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t func7 : 7;
};

struct i_type_insn {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t func3 : 3;
    uint32_t rs1 : 5;
    uint32_t imm_11_0 : 12;
};

struct s_type_insn {
    uint32_t opcode : 7;
    uint32_t imm_4_0 : 5;
    uint32_t func3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm_11_5 : 7;
};

struct b_type_insn {
    uint32_t opcode : 7;
    uint32_t imm_11_11 : 1;
    uint32_t imm_4_1 : 4;
    uint32_t func3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm_10_5 : 6;
    uint32_t imm_12_12 : 1;
};

struct u_type_insn {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm_31_12 : 20;
};

struct j_type_insn {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm_19_12 : 8;
    uint32_t imm_11_11 : 1;
    uint32_t imm_10_1 : 10;
    uint32_t imm_20_20 : 1;
};

struct slli_insn {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t func3 : 3;
    uint32_t rs1 : 5;
    uint32_t shamt : 6;
    uint32_t func6 : 6;
};

struct amo_insn {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t func3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t rl : 1;
    uint32_t aq : 1;
    uint32_t func5 : 5;
};

struct fence_insn {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t func3 : 3;
    uint32_t rs1 : 5;
    uint32_t pred : 4;
    uint32_t succ : 4;
    uint32_t fm : 4;
};

struct c_addi4spn_insn {
    uint32_t opcode : 2;
    uint32_t rd : 3;
    uint32_t imm_3_3 : 1;
    uint32_t imm_2_2 : 1;
    uint32_t imm_9_6 : 4;
    uint32_t imm_5_4 : 2;
    uint32_t func3 : 3;
};

struct c_sld_insn {
    uint32_t opcode : 2;
    uint32_t rd : 3;
    uint32_t imm_7_6 : 2;
    uint32_t rs1 : 3;
    uint32_t imm_5_3 : 3;
    uint32_t func3 : 3;
};

struct c_slw_insn {
    uint32_t opcode : 2;
    uint32_t rd : 3;
    uint32_t imm_6_6 : 1;
    uint32_t imm_2_2 : 1;
    uint32_t rs1 : 3;
    uint32_t imm_5_3 : 3;
    uint32_t func3 : 3;
};

struct c_addi_insn {
    uint32_t opcode : 2;
    uint32_t imm_4_0 : 5;
    uint32_t rd : 5;
    uint32_t imm_5_5 : 1;
    uint32_t func3 : 3;
};

struct c_j_insn {
    uint32_t opcode : 2;
    uint32_t imm_5_5 : 1;
    uint32_t imm_3_1 : 3;
    uint32_t imm_7_7 : 1;
    uint32_t imm_6_6 : 1;
    uint32_t imm_10_10 : 1;
    uint32_t imm_9_8 : 2;
    uint32_t imm_4_4 : 1;
    uint32_t imm_11_11 : 1;
    uint32_t func3 : 3;
};

struct c_addi16sp_insn {
    uint32_t opcode : 2;
    uint32_t imm_5_5 : 1;
    uint32_t imm_8_7 : 2;
    uint32_t imm_6_6 : 1;
    uint32_t imm_4_4 : 1;
    uint32_t rd : 5;
    uint32_t imm_9_9 : 1;
    uint32_t func3 : 3;
};

struct c_lui_insn {
    uint32_t opcode : 2;
    uint32_t imm_16_12 : 5;
    uint32_t rd : 5;
    uint32_t imm_17_17 : 1;
    uint32_t func3 : 3;
};

struct c_andi_insn {
    uint32_t opcode : 2;
    uint32_t imm_4_0 : 5;
    uint32_t rd : 3;
    uint32_t func2 : 2;
    uint32_t imm_5_5 : 1;
    uint32_t func3 : 3;
};

struct c_sub_insn {
    uint32_t opcode : 2;
    uint32_t rs2 : 3;
    uint32_t func2_0 : 2;
    uint32_t rd : 3;
    uint32_t func2_1 : 2;
    uint32_t func1 : 1;
    uint32_t func3 : 3;
};

struct c_b_insn {
    uint32_t opcode : 2;
    uint32_t imm_5_5 : 1;
    uint32_t imm_2_1 : 2;
    uint32_t imm_7_6 : 2;
    uint32_t rd : 3;
    uint32_t imm_4_3 : 2;
    uint32_t imm_8_8 : 1;
    uint32_t func3 : 3;
};

struct c_ldsp_insn {
    uint32_t opcode : 2;
    uint32_t imm_8_6 : 3;
    uint32_t imm_4_3 : 2;
    uint32_t rd : 5;
    uint32_t imm_5_5 : 1;
    uint32_t func3 : 3;
};

struct c_lwsp_insn {
    uint32_t opcode : 2;
    uint32_t imm_7_6 : 2;
    uint32_t imm_4_2 : 3;
    uint32_t rd : 5;
    uint32_t imm_5_5 : 1;
    uint32_t func3 : 3;
};

struct c_add_insn {
    uint32_t opcode : 2;
    uint32_t rs2 : 5;
    uint32_t rd : 5;
    uint32_t func1 : 1;
    uint32_t func3 : 3;
};

struct c_sdsp_insn {
    uint32_t opcode : 2;
    uint32_t rs2 : 5;
    uint32_t imm_8_6 : 3;
    uint32_t imm_5_3 : 3;
    uint32_t func3 : 3;
};

struct c_swsp_insn {
    uint32_t opcode : 2;
    uint32_t rs2 : 5;
    uint32_t imm_7_6 : 2;
    uint32_t imm_5_2 : 4;
    uint32_t func3 : 3;
};

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
    virtual void single_step();
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
