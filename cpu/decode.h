#ifndef __DECODE__
#define __DECODE__

#include <assert.h>

// case format
#define DECLARECASE(LABEL, EXECUTE) \
    case LABEL:                     \
        EXECUTE
#define DECLAREDEFAULTCASE(EXECUTE) \
    default:                        \
        EXECUTE

// Register declare
#define REG_ZERO 0
#define REG_RA 1
#define REG_SP 2
#define REG_GP 3
#define REG_TP 4
#define REG_T0 5
#define REG_T1 6
#define REG_T2 7
#define REG_S0 8
#define REG_FP 8
#define REG_S1 9
#define REG_A0 10
#define REG_A1 11
#define REG_A2 12
#define REG_A3 13
#define REG_A4 14
#define REG_A5 15
#define REG_A6 16
#define REG_A7 17
#define REG_S2 18
#define REG_S3 19
#define REG_S4 20
#define REG_S5 21
#define REG_S6 22
#define REG_S7 23
#define REG_S8 24
#define REG_S9 25
#define REG_S10 26
#define REG_S11 27
#define REG_T3 28
#define REG_T4 29
#define REG_T5 30
#define REG_T6 31

#define REG_FT0 0
#define REG_FT1 1
#define REG_FT2 2
#define REG_FT3 3
#define REG_FT4 4
#define REG_FT5 5
#define REG_FT6 6
#define REG_FT7 7
#define REG_FS0 8
#define REG_FS1 9
#define REG_FA0 10
#define REG_FA1 11
#define REG_FA2 12
#define REG_FA3 13
#define REG_FA4 14
#define REG_FA5 15
#define REG_FA6 16
#define REG_FA7 17
#define REG_FS2 18
#define REG_FS3 19
#define REG_FS4 20
#define REG_FS5 21
#define REG_FS6 22
#define REG_FS7 23
#define REG_FS8 24
#define REG_FS9 25
#define REG_FS10 26
#define REG_FS11 27
#define REG_FT8 28
#define REG_FT9 29
#define REG_FT10 30
#define REG_FT11 31

#define FUNCT12_ECALL 0x000
#define FUNCT12_EBREAK 0x001
#define FUNCT12_WFI 0x105
#define FUNCT12_SRET 0x102
#define FUNCT12_MRET 0x302

#define FUNCT7_SRLI 0x00
#define FUNCT7_SRAI 0x20
#define FUNCT7_ADD 0x00
#define FUNCT7_SUB 0x20
#define FUNCT7_SLL 0x00
#define FUNCT7_SLT 0x00
#define FUNCT7_SLTU 0x00
#define FUNCT7_XOR 0x00
#define FUNCT7_SRL 0x00
#define FUNCT7_SRA 0x20
#define FUNCT7_OR 0x00
#define FUNCT7_AND 0x00
#define FUNCT7_MULDIV 0x01
#define FUNCT7_SFENCE_VMA 0x9

#define FUNCT3_BEQ 0x0
#define FUNCT3_BNE 0x1
#define FUNCT3_BLT 0x4
#define FUNCT3_BGE 0x5
#define FUNCT3_BLTU 0x6
#define FUNCT3_BGEU 0x7
#define FUNCT3_LB 0x0
#define FUNCT3_LH 0x1
#define FUNCT3_LW 0x2
#define FUNCT3_LWU 0x6
#define FUNCT3_LD 0x3
#define FUNCT3_LBU 0x4
#define FUNCT3_LHU 0x5
#define FUNCT3_SB 0x0
#define FUNCT3_SH 0x1
#define FUNCT3_SW 0x2
#define FUNCT3_SD 0x3
#define FUNCT3_ADDI 0x0
#define FUNCT3_SLTI 0x2
#define FUNCT3_SLTIU 0x3
#define FUNCT3_XORI 0x4
#define FUNCT3_ORI 0x6
#define FUNCT3_ANDI 0x7
#define FUNCT3_SLLI 0x1
#define FUNCT3_SRI 0x5
#define FUNCT3_ADD 0x0
#define FUNCT3_SLL 0x1
#define FUNCT3_SLT 0x2
#define FUNCT3_SLTU 0x3
#define FUNCT3_XOR 0x4
#define FUNCT3_SRR 0x5
#define FUNCT3_OR 0x6
#define FUNCT3_AND 0x7
#define FUNCT3_FENCE 0x0
#define FUNCT3_FENCE_I 0x1
#define FUNCT3_PRIV 0x0
#define FUNCT3_CSRRW 0x1
#define FUNCT3_CSRRS 0x2
#define FUNCT3_CSRRC 0x3
#define FUNCT3_CSRRWI 0x5
#define FUNCT3_CSRRSI 0x6
#define FUNCT3_CSRRCI 0x7

// M extension code
#define FUNCT3_MUL 0x0
#define FUNCT3_MULH 0x1
#define FUNCT3_MULHSU 0x2
#define FUNCT3_MULHU 0x3
#define FUNCT3_DIV 0x4
#define FUNCT3_DIVU 0x5
#define FUNCT3_REM 0x6
#define FUNCT3_REMU 0x7

// A extension code
#define FUNCT5_LR 0x2
#define FUNCT5_SC 0x3
#define FUNCT5_AMOSWAP 0x1
#define FUNCT5_AMOADD 0x0
#define FUNCT5_AMOXOR 0x4
#define FUNCT5_AMOAND 0xc
#define FUNCT5_AMOOR 0x8
#define FUNCT5_AMOMIN 0x10
#define FUNCT5_AMOMAX 0x14
#define FUNCT5_AMOMINU 0x18
#define FUNCT5_AMOMAXU 0x1c

#define FUNCT3_AMO_W 0b010
#define FUNCT3_AMO_D 0b011

// F extension code
#define FUNCT7_FMADD_S 0x0
#define FUNCT7_FMADD_D 0x1

#define FUNCT7_FADD_S 0x00
#define FUNCT7_FSUB_S 0x04
#define FUNCT7_FMUL_S 0x08
#define FUNCT7_FDIV_S 0x0c
#define FUNCT7_FSQRT_S 0x2c
#define FUNCT7_FSGNJ_S 0x10
#define FUNCT7_FSGNJN_S 0x10
#define FUNCT7_FSGNJX_S 0x10
#define FUNCT7_FMIN_S 0x14
#define FUNCT7_FMAX_S 0x14
#define FUNCT7_FCVT_W_S 0x60
#define FUNCT7_FCVT_WU_S 0x60
#define FUNCT7_FMV_X_W 0x70
#define FUNCT7_FCMP_S 0x50
#define FUNCT7_FCLASS_S 0x70
#define FUNCT7_FCVT_S_W 0x68
#define FUNCT7_FCVT_S_WU 0x68
#define FUNCT7_FMV_W_X 0x78
#define FUNCT7_FCVT_L_S 0x60
#define FUNCT7_FCVT_LU_S 0x60
#define FUNCT7_FCVT_S_L 0x68
#define FUNCT7_FCVT_S_LU 0x68

#define FUNCT3_FMV_X_W 0x0
#define FUNCT3_FCLASS_S 0x1
#define FUNCT3_FEQ_S 0x2
#define FUNCT3_FLT_S 0x1
#define FUNCT3_FLE_S 0x0
#define FUNCT3_FMIN 0x0
#define FUNCT3_FMAX 0x1
#define FUNCT3_FSGNJ 0x0
#define FUNCT3_FSGNJN 0x1
#define FUNCT3_FSGNJX 0x2

#define FUNCT5_FCVT_S_W 0x0
#define FUNCT5_FCVT_S_WU 0x1
#define FUNCT5_FCVT_S_L 0x2
#define FUNCT5_FCVT_S_LU 0x3

// D extension code
#define FUNCT7_FADD_D 0x01
#define FUNCT7_FSUB_D 0x05
#define FUNCT7_FMUL_D 0x09
#define FUNCT7_FDIV_D 0x0d
#define FUNCT7_FSQRT_D 0x2d
#define FUNCT7_FSGNJ_D 0x11
#define FUNCT7_FSGNJN_D 0x11
#define FUNCT7_FSGNJX_D 0x11
#define FUNCT7_FMIN_D 0x15
#define FUNCT7_FMAX_D 0x15
#define FUNCT7_FMAX_D 0x15
#define FUNCT7_FCVT_S_D 0x20
#define FUNCT7_FCVT_D_S 0x21
#define FUNCT7_FCMP_D 0x51
#define FUNCT7_FCLASS_D 0x71
#define FUNCT7_FCVT_W_D 0x61
#define FUNCT7_FCVT_WU_D 0x61
#define FUNCT7_FCVT_D_W 0x69
#define FUNCT7_FCVT_D_WU 0x69

#define FUNCT7_FCVT_L_D 0x61
#define FUNCT7_FCVT_LU_D 0x61
#define FUNCT7_FMV_X_D 0x71
#define FUNCT7_FCVT_D_L 0x69
#define FUNCT7_FCVT_D_LU 0x69
#define FUNCT7_FMV_D_X 0x79

#define FUNCT5_FCVT_S_D 0x1
#define FUNCT5_FCVT_D_S 0x0

// C extension code
#define FUNCT3_C_ADDI4SPN 0b000
#define FUNCT3_C_FLD 0b001
#define FUNCT3_C_LW 0b010
#define FUNCT3_C_LD 0b011
#define FUNCT3_C_FSD 0b101
#define FUNCT3_C_SW 0b110
#define FUNCT3_C_SD 0b111
#define FUNCT3_C_ADDI 0b000
#define FUNCT3_C_ADDIW 0b001
#define FUNCT3_C_LI 0b010
#define FUNCT3_C_LUI 0b011
#define FUNCT3_C_MISC_ALU 0b100
#define FUNCT3_C_J 0b101
#define FUNCT3_C_BEQZ 0b110
#define FUNCT3_C_BNEZ 0b111
#define FUNCT3_C_SLLI 0b000
#define FUNCT3_C_LWSP 0b010
#define FUNCT3_C_LDSP 0b011
#define FUNCT3_C_JR 0b100
#define FUNCT3_C_SWSP 0b110
#define FUNCT3_C_SDSP 0b111

#define FUNCT6_C_SUB 0x23
#define FUNCT6_C_SUBW 0x27

#define FUNCT2_C_SUB 0b00
#define FUNCT2_C_XOR 0b01
#define FUNCT2_C_OR 0b10
#define FUNCT2_C_AND 0b11
#define FUNCT2_C_SUBW 0b00
#define FUNCT2_C_ADDW 0b01

#define FUNCT2_C_SRLI 0b00
#define FUNCT2_C_SRAI 0b01
#define FUNCT2_C_ANDI 0b10

#define FUNCT4_C_JR 0x8
#define FUNCT4_C_JALR 0x9

// RV32I Base Instruction Set
#define OPCODE_LUI 0x37
#define OPCODE_AUIPC 0x17
#define OPCODE_JAL 0x6F
#define OPCODE_JALR 0x67
#define OPCODE_BRANCH 0x63
#define OPCODE_LOAD 0x03
#define OPCODE_STORE 0x23
#define OPCODE_OP_IMM 0x13
#define OPCODE_OP 0x33
#define OPCODE_MISC_MEM 0x0F
#define OPCODE_SYSTEM 0x73

// RV64I Base Instruction Set (in addition to RV32I)
#define OPCODE_OP_IMM_32 0x1B
#define OPCODE_OP_32 0x3B

// RV32A Standard Extension
#define OPCODE_AMO 0x2F

// RV32F Standard Extension
#define OPCODE_LOAD_FP 0x07
#define OPCODE_STORE_FP 0x27
#define OPCODE_FMADD 0x43
#define OPCODE_FMSUB 0x47
#define OPCODE_FNMSUB 0x4B
#define OPCODE_FNMADD 0x4F
#define OPCODE_OP_FP 0x53

// RVC Standard Extension
#define OPCODE_C0 0x0
#define OPCODE_C1 0x1
#define OPCODE_C2 0x2

#define FREG_FILL_MSB_WORD 0xffffffff00000000UL

#define confirm_insn_legal(x)                  \
    do {                                       \
        if (!(x)) {                            \
            throw TrapIllegalInstruction((x)); \
        }                                      \
    } while (0)

#define require_privilege(x) confirm_insn_legal(csr->prv >= (x))
#define require_extension(x) confirm_insn_legal(support_extension(x))
#define require_fp confirm_insn_legal(csr->mstatus &MSTATUS_FS)

#define confirm_csr_legal(x, write)                                 \
    do {                                                            \
        if ((csr->mstatus & MSTATUS_TVM) && x == CSR_SATP_ADDR)     \
            require_privilege(PRV_M);                               \
        else                                                        \
            require_privilege(get_field((x), 0x300));               \
        confirm_insn_legal(!(get_field((x), 0xC00) == 3 && write)); \
    } while (0)

#define INSTRUCT_UNKNOWN                    \
    sprintf(remark, "unknown instruction"); \
    throw TrapIllegalInstruction(insn);     \
    pc += 4UL;                              \
    break;

#define INSTRUCT_LUI                                                       \
    sprintf(remark, "lui %s,0x%lx", regs_name[rd], imm_u >> 12 & 0xfffff); \
    regs[rd] = imm_u;                                                      \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_AUIPC                                                       \
    sprintf(remark, "auipc %s,0x%lx", regs_name[rd], imm_u >> 12 & 0xfffff); \
    regs[rd] = imm_u + pc;                                                   \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_JAL                                           \
    if (!rd)                                                   \
        sprintf(remark, "j %08lx", pc + imm_j & 0xffffffff);   \
    else if (rd == REG_RA)                                     \
        sprintf(remark, "jal %08lx", pc + imm_j & 0xffffffff); \
    else                                                       \
        sprintf(remark, "jal %s,%08lx", regs_name[rd],         \
                pc + imm_j & 0xffffffff);                      \
    regs[rd] = pc + 4UL;                                       \
    pc += imm_j;                                               \
    break;

#define INSTRUCT_JALR                                                          \
    {                                                                          \
        if (imm_i || !(rd == 0 || rd == 1))                                    \
            sprintf(remark, "jalr %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                    regs_name[rs1]);                                           \
        else if (rd == 1)                                                      \
            sprintf(remark, "jalr %s", regs_name[rs1]);                        \
        else if (rs1 == 1)                                                     \
            sprintf(remark, "ret");                                            \
        else                                                                   \
            sprintf(remark, "jr %s", regs_name[rs1]);                          \
        uint64_t rs1_data(regs[rs1]);                                          \
        regs[rd] = pc + 4UL;                                                   \
        pc = (imm_i + rs1_data) & ~1UL;                                        \
    }                                                                          \
    break;

#define INSTRUCT_BEQ                                                       \
    if (rs2)                                                               \
        sprintf(remark, "beq %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                pc + imm_b & 0xffffffff);                                  \
    else                                                                   \
        sprintf(remark, "beqz %s,%08lx", regs_name[rs1],                   \
                pc + imm_b & 0xffffffff);                                  \
    pc += regs[rs1] == regs[rs2] ? imm_b : 4UL;                            \
    break;

#define INSTRUCT_BNE                                                       \
    if (rs2)                                                               \
        sprintf(remark, "bne %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                pc + imm_b & 0xffffffff);                                  \
    else                                                                   \
        sprintf(remark, "bnez %s,%08lx", regs_name[rs1],                   \
                pc + imm_b & 0xffffffff);                                  \
    pc += regs[rs1] != regs[rs2] ? imm_b : 4UL;                            \
    break;

#define INSTRUCT_BLT                                                       \
    if (!rs1)                                                              \
        sprintf(remark, "bgtz %s,%08lx", regs_name[rs2],                   \
                pc + imm_b & 0xffffffff);                                  \
    else if (!rs2)                                                         \
        sprintf(remark, "bltz %s,%08lx", regs_name[rs1],                   \
                pc + imm_b & 0xffffffff);                                  \
    else                                                                   \
        sprintf(remark, "blt %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                pc + imm_b & 0xffffffff);                                  \
    pc += (int64_t) regs[rs1] < (int64_t) regs[rs2] ? imm_b : 4UL;         \
    break;

#define INSTRUCT_BGE                                                       \
    if (!rs1)                                                              \
        sprintf(remark, "blez %s,%08lx", regs_name[rs2],                   \
                pc + imm_b & 0xffffffff);                                  \
    else if (!rs2)                                                         \
        sprintf(remark, "bgez %s,%08lx", regs_name[rs1],                   \
                pc + imm_b & 0xffffffff);                                  \
    else                                                                   \
        sprintf(remark, "bge %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                pc + imm_b & 0xffffffff);                                  \
    pc += (int64_t) regs[rs1] >= (int64_t) regs[rs2] ? imm_b : 4UL;        \
    break;

#define INSTRUCT_BLTU                                                   \
    sprintf(remark, "bltu %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
            pc + imm_b & 0xffffffff);                                   \
    pc += regs[rs1] < regs[rs2] ? imm_b : 4UL;                          \
    break;

#define INSTRUCT_BGEU                                                   \
    sprintf(remark, "bgeu %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
            pc + imm_b & 0xffffffff);                                   \
    pc += regs[rs1] >= regs[rs2] ? imm_b : 4UL;                         \
    break;

#define INSTRUCT_LB                                                  \
    sprintf(remark, "lb %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                         \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_BYTE);         \
    pc += 4UL;                                                       \
    break;

#define INSTRUCT_LH                                                  \
    sprintf(remark, "lh %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                         \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_HWORD);        \
    pc += 4UL;                                                       \
    break;

#define INSTRUCT_LW                                                  \
    sprintf(remark, "lw %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                         \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD);         \
    pc += 4UL;                                                       \
    break;

#define INSTRUCT_LBU                                                  \
    sprintf(remark, "lbu %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                          \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_BYTE_UNSIGNED); \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_LHU                                                   \
    sprintf(remark, "lhu %s,%ld(%s)", regs_name[rd], (int64_t) imm_i,  \
            regs_name[rs1]);                                           \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_HWORD_UNSIGNED); \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_LWU                                                  \
    sprintf(remark, "lwu %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                          \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD_UNSIGNED); \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_LD                                                  \
    sprintf(remark, "ld %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                         \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_DWORD);        \
    pc += 4UL;                                                       \
    break;

#define INSTRUCT_SB                                                   \
    sprintf(remark, "sb %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
            regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_BYTE, regs[rs2]);         \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_SH                                                   \
    sprintf(remark, "sh %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
            regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_HWORD, regs[rs2]);        \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_SW                                                   \
    sprintf(remark, "sw %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
            regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_WORD, regs[rs2]);         \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_SD                                                   \
    sprintf(remark, "sd %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
            regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_DWORD, regs[rs2]);        \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_ADDI                                                    \
    if (!rd && !rs1 && !imm_i)                                           \
        sprintf(remark, "nop");                                          \
    else if (!rs1)                                                       \
        sprintf(remark, "li %s,%ld", regs_name[rd], (int64_t) imm_i);    \
    else if (!imm_i)                                                     \
        sprintf(remark, "mv %s,%s", regs_name[rd], regs_name[rs1]);      \
    else                                                                 \
        sprintf(remark, "addi %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                (int64_t) imm_i);                                        \
    regs[rd] = regs[rs1] + imm_i;                                        \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_SLTI                                                \
    sprintf(remark, "slti %s,%s,%ld", regs_name[rd], regs_name[rs1], \
            (int64_t) imm_i);                                        \
    regs[rd] = (int64_t) regs[rs1] < (int64_t) imm_i;                \
    pc += 4UL;                                                       \
    break;

#define INSTRUCT_SLTIU                                                \
    sprintf(remark, "sltiu %s,%s,%ld", regs_name[rd], regs_name[rs1], \
            (int64_t) imm_i);                                         \
    regs[rd] = regs[rs1] < imm_i;                                     \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_XORI                                                    \
    if (imm_i == -1U)                                                    \
        sprintf(remark, "not %s,%s", regs_name[rd], regs_name[rs1]);     \
    else                                                                 \
        sprintf(remark, "xori %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                (int64_t) imm_i);                                        \
    regs[rd] = regs[rs1] ^ imm_i;                                        \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_ORI                                                \
    sprintf(remark, "ori %s,%s,%ld", regs_name[rd], regs_name[rs1], \
            (int64_t) imm_i);                                       \
    regs[rd] = regs[rs1] | imm_i;                                   \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_ANDI                                                \
    sprintf(remark, "andi %s,%s,%ld", regs_name[rd], regs_name[rs1], \
            (int64_t) imm_i);                                        \
    regs[rd] = regs[rs1] & imm_i;                                    \
    pc += 4UL;                                                       \
    break;

#define INSTRUCT_SLLI                                                 \
    sprintf(remark, "slli %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
            shamt64);                                                 \
    regs[rd] = regs[rs1] << shamt64;                                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_SRLI                                                 \
    sprintf(remark, "srli %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
            shamt64);                                                 \
    regs[rd] = regs[rs1] >> shamt64;                                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_SRAI                                                 \
    sprintf(remark, "srai %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
            shamt64);                                                 \
    regs[rd] = (int64_t) regs[rs1] >> shamt64;                        \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_ADD                                               \
    sprintf(remark, "add %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] + regs[rs2];                              \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_SLL                                               \
    sprintf(remark, "sll %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] << (regs[rs2] & 0x3F);                    \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_SLT                                               \
    sprintf(remark, "slt %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = (int64_t) regs[rs1] < (int64_t) regs[rs2];          \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_SLTU                                               \
    sprintf(remark, "sltu %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    regs[rd] = regs[rs1] < regs[rs2];                               \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_XOR                                               \
    sprintf(remark, "xor %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] ^ regs[rs2];                              \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_SRL                                               \
    sprintf(remark, "srl %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] >> (regs[rs2] & 0x3F);                    \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_OR                                               \
    sprintf(remark, "or %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                      \
    regs[rd] = regs[rs1] | regs[rs2];                             \
    pc += 4UL;                                                    \
    break;

#define INSTRUCT_AND                                               \
    sprintf(remark, "and %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] & regs[rs2];                              \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_SUB                                               \
    sprintf(remark, "sub %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] - regs[rs2];                              \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_SRA                                               \
    sprintf(remark, "sra %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    regs[rd] = (int64_t) regs[rs1] >> (regs[rs2] & 0x3F);          \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_MUL                                               \
    sprintf(remark, "mul %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    require_extension('M');                                        \
    regs[rd] = regs[rs1] * regs[rs2];                              \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_MULH                                               \
    sprintf(remark, "mulh %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    require_extension('M');                                         \
    regs[rd] = mulh(regs[rs1], regs[rs2]);                          \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_MULHSU                                               \
    sprintf(remark, "mulhsu %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                          \
    require_extension('M');                                           \
    regs[rd] = mulhsu(regs[rs1], regs[rs2]);                          \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_MULHU                                               \
    sprintf(remark, "mulhu %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                         \
    require_extension('M');                                          \
    regs[rd] = mulhu(regs[rs1], regs[rs2]);                          \
    pc += 4UL;                                                       \
    break;

#define INSTRUCT_DIV                                               \
    sprintf(remark, "div %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    require_extension('M');                                        \
    if (!regs[rs2])                                                \
        regs[rd] = -1UL;                                           \
    else if (regs[rs1] == 1UL << 63 && regs[rs2] == -1UL)          \
        regs[rd] = regs[rs1];                                      \
    else                                                           \
        regs[rd] = (int64_t) regs[rs1] / (int64_t) regs[rs2];      \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_DIVU                                               \
    sprintf(remark, "divu %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    require_extension('M');                                         \
    if (!regs[rs2])                                                 \
        regs[rd] = -1UL;                                            \
    else                                                            \
        regs[rd] = regs[rs1] / regs[rs2];                           \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_REM                                               \
    sprintf(remark, "rem %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                       \
    require_extension('M');                                        \
    if (!regs[rs2])                                                \
        regs[rd] = regs[rs1];                                      \
    else if (regs[rs1] == (1UL << 63) && regs[rs2] == -1UL)        \
        regs[rd] = 0;                                              \
    else                                                           \
        regs[rd] = (int64_t) regs[rs1] % (int64_t) regs[rs2];      \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_REMU                                               \
    sprintf(remark, "remu %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    require_extension('M');                                         \
    if (!regs[rs2])                                                 \
        regs[rd] = regs[rs1];                                       \
    else                                                            \
        regs[rd] = regs[rs1] % regs[rs2];                           \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_FENCE                                                      \
    if (pred != 0xf | succ != 0xf)                                          \
        sprintf(remark, "fence %s,%s", fence_flag(pred), fence_flag(succ)); \
    else                                                                    \
        sprintf(remark, "fence");                                           \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FENCE_I        \
    sprintf(remark, "fence.i"); \
    pc += 4UL;                  \
    break;

#define INSTRUCT_ECALL               \
    sprintf(remark, "ecall");        \
    switch (csr->prv) {              \
    case PRV_U:                      \
        throw TrapUserEcall();       \
        break;                       \
    case PRV_S:                      \
        throw TrapSupervisorEcall(); \
        break;                       \
    case PRV_H:                      \
        throw TrapHypervisorEcall(); \
        break;                       \
    case PRV_M:                      \
        throw TrapMachineEcall();    \
        break;                       \
    default:                         \
        abort();                     \
    }                                \
    break;

#define INSTRUCT_EBREAK        \
    sprintf(remark, "ebreak"); \
    throw TrapBreakpoint(pc);  \
    break;

#define INSTRUCT_WFI                                                \
    sprintf(remark, "wfi");                                         \
    require_privilege((csr->mstatus & MSTATUS_TW) ? PRV_M : PRV_S); \
    throw WaitForInterrupt();                                       \
    break;

#define INSTRUCT_SRET                                                        \
    {                                                                        \
        sprintf(remark, "sret");                                             \
        require_privilege((csr->mstatus & MSTATUS_TSR) ? PRV_M : PRV_S);     \
        pc = csr->sepc & pc_alignment_mask;                                  \
        uint64_t status(csr->mstatus);                                       \
        csr->prv = get_field(status, MSTATUS_SPP);                           \
        status =                                                             \
            set_field(status, MSTATUS_SIE, get_field(status, MSTATUS_SPIE)); \
        status = set_field(status, MSTATUS_SPIE, 1);                         \
        status = set_field(status, MSTATUS_SPP, PRV_U);                      \
        csr->mstatus = status;                                               \
    }                                                                        \
    break;

#define INSTRUCT_MRET                                                        \
    {                                                                        \
        sprintf(remark, "mret");                                             \
        require_privilege(PRV_M);                                            \
        pc = csr->mepc & pc_alignment_mask;                                  \
        uint64_t status(csr->mstatus);                                       \
        csr->prv = get_field(status, MSTATUS_MPP);                           \
        status =                                                             \
            set_field(status, MSTATUS_MIE, get_field(status, MSTATUS_MPIE)); \
        status = set_field(status, MSTATUS_MPIE, 1);                         \
        status = set_field(status, MSTATUS_MPP, PRV_U);                      \
        csr->mstatus = status;                                               \
    }                                                                        \
    break;

#define INSTRUCT_SFENCE_VMA                                                  \
    sprintf(remark, "sfence.vma %s,%s", regs_name[rs1], regs_name[rs2]);     \
    require_privilege((csr->mstatus & MSTATUS_TVM) ? PRV_M                   \
                                                   : PRV_S); /* TLB flush */ \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_CSRRW                                             \
    {                                                              \
        if (rd)                                                    \
            sprintf(remark, "csrrw %s,%s,%s", regs_name[rd],       \
                    csr->csr_name(csr_addr), regs_name[rs1]);      \
        else                                                       \
            sprintf(remark, "csrw %s,%s", csr->csr_name(csr_addr), \
                    regs_name[rs1]);                               \
        confirm_csr_legal(csr_addr, 1);                            \
        uint64_t tmp(csr->get_csr(csr_addr));                      \
        csr->set_csr(csr_addr, regs[rs1]);                         \
        regs[rd] = tmp;                                            \
        pc += 4UL;                                                 \
    }                                                              \
    break;

#define INSTRUCT_CSRRS                                             \
    {                                                              \
        if (!rs1)                                                  \
            sprintf(remark, "csrr %s,%s", regs_name[rd],           \
                    csr->csr_name(csr_addr));                      \
        else if (!rd)                                              \
            sprintf(remark, "csrs %s,%s", csr->csr_name(csr_addr), \
                    regs_name[rs1]);                               \
        else                                                       \
            sprintf(remark, "csrrs %s,%s,%s", regs_name[rd],       \
                    csr->csr_name(csr_addr), regs_name[rs1]);      \
        confirm_csr_legal(csr_addr, rs1);                          \
        uint64_t tmp(csr->get_csr(csr_addr));                      \
        csr->set_csr(csr_addr, tmp | regs[rs1]);                   \
        regs[rd] = tmp;                                            \
        pc += 4UL;                                                 \
    }                                                              \
    break;

#define INSTRUCT_CSRRC                                             \
    {                                                              \
        if (rd)                                                    \
            sprintf(remark, "csrrc %s,%s,%s", regs_name[rd],       \
                    csr->csr_name(csr_addr), regs_name[rs1]);      \
        else                                                       \
            sprintf(remark, "csrc %s,%s", csr->csr_name(csr_addr), \
                    regs_name[rs1]);                               \
        confirm_csr_legal(csr_addr, rs1);                          \
        uint64_t tmp(csr->get_csr(csr_addr));                      \
        csr->set_csr(csr_addr, tmp & ~regs[rs1]);                  \
        regs[rd] = tmp;                                            \
        pc += 4UL;                                                 \
    }                                                              \
    break;

#define INSTRUCT_CSRRWI                                               \
    if (rd)                                                           \
        sprintf(remark, "csrrwi %s,%s,%d", regs_name[rd],             \
                csr->csr_name(csr_addr), rs1);                        \
    else                                                              \
        sprintf(remark, "csrwi %s,%d", csr->csr_name(csr_addr), rs1); \
    confirm_csr_legal(csr_addr, 1);                                   \
    regs[rd] = csr->get_csr(csr_addr);                                \
    csr->set_csr(csr_addr, zext(rs1, 5));                             \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_CSRRSI                                                   \
    {                                                                     \
        if (rd)                                                           \
            sprintf(remark, "csrrsi %s,%s,%d", regs_name[rd],             \
                    csr->csr_name(csr_addr), rs1);                        \
        else                                                              \
            sprintf(remark, "csrsi %s,%d", csr->csr_name(csr_addr), rs1); \
        confirm_csr_legal(csr_addr, rs1);                                 \
        uint64_t tmp(csr->get_csr(csr_addr));                             \
        csr->set_csr(csr_addr, tmp | zext(rs1, 5));                       \
        regs[rd] = tmp;                                                   \
        pc += 4UL;                                                        \
    }                                                                     \
    break;

#define INSTRUCT_CSRRCI                                                   \
    {                                                                     \
        if (rd)                                                           \
            sprintf(remark, "csrrci %s,%s,%d", regs_name[rd],             \
                    csr->csr_name(csr_addr), rs1);                        \
        else                                                              \
            sprintf(remark, "csrci %s,%d", csr->csr_name(csr_addr), rs1); \
        confirm_csr_legal(csr_addr, rs1);                                 \
        uint64_t tmp(csr->get_csr(csr_addr));                             \
        csr->set_csr(csr_addr, tmp &(~zext(rs1, 5)));                     \
        regs[rd] = tmp;                                                   \
        pc += 4UL;                                                        \
    }                                                                     \
    break;

#define INSTRUCT_ADDIW                                                    \
    if (imm_i)                                                            \
        sprintf(remark, "addiw %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                (int64_t) imm_i);                                         \
    else                                                                  \
        sprintf(remark, "sext.w %s,%s", regs_name[rd], regs_name[rs1]);   \
    regs[rd] = (int32_t) regs[rs1] + (int32_t) imm_i;                     \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_SLLIW                                                         \
    sprintf(remark, "slliw %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt); \
    regs[rd] = (int32_t) regs[rs1] << shamt;                                   \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_SRLIW                                                         \
    sprintf(remark, "srliw %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt); \
    regs[rd] = (int32_t)((uint32_t) regs[rs1] >> shamt);                       \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_SRAIW                                                 \
    sprintf(remark, "sraiw %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
            shamt64);                                                  \
    regs[rd] = ((int32_t) regs[rs1] >> shamt);                         \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_ADDW                                               \
    sprintf(remark, "addw %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    regs[rd] = (int32_t) regs[rs1] + (int32_t) regs[rs2];           \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_SUBW                                               \
    sprintf(remark, "subw %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    regs[rd] = (int32_t) regs[rs1] - (int32_t) regs[rs2];           \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_MULW                                               \
    sprintf(remark, "mulw %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    require_extension('M');                                         \
    regs[rd] = (regs[rs1] * regs[rs2]) << 32 >> 32;                 \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_SLLW                                               \
    sprintf(remark, "sllw %s,%s,%s", regs_name[rd], regs_name[rs1], \
            regs_name[rs2]);                                        \
    regs[rd] = (int32_t) regs[rs1] << (regs[rs2] & 0x1F);           \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_DIVW                                                      \
    sprintf(remark, "divw %s,%s,%s", regs_name[rd], regs_name[rs1],        \
            regs_name[rs2]);                                               \
    require_extension('M');                                                \
    if (!(int32_t) regs[rs2])                                              \
        regs[rd] = -1UL;                                                   \
    else                                                                   \
        regs[rd] =                                                         \
            (int32_t)((int64_t)(int32_t) regs[rs1] / (int32_t) regs[rs2]); \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_SRLW                                                    \
    sprintf(remark, "srlw %s,%s,%s", regs_name[rd], regs_name[rs1],      \
            regs_name[rs2]);                                             \
    regs[rd] =                                                           \
        (int32_t)((uint32_t) regs[rs1] >> (uint32_t)(regs[rs2] & 0x1F)); \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_SRAW                                                 \
    sprintf(remark, "sraw %s,%s,%s", regs_name[rd], regs_name[rs1],   \
            regs_name[rs2]);                                          \
    regs[rd] = ((int32_t) regs[rs1] >> (uint32_t)(regs[rs2] & 0x1F)); \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_DIVUW                                                     \
    sprintf(remark, "divuw %s,%s,%s", regs_name[rd], regs_name[rs1],       \
            regs_name[rs2]);                                               \
    require_extension('M');                                                \
    if (!(uint32_t) regs[rs2])                                             \
        regs[rd] = -1UL;                                                   \
    else                                                                   \
        regs[rd] = (int32_t)((uint32_t) regs[rs1] / (uint32_t) regs[rs2]); \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_REMW                                                      \
    sprintf(remark, "remw %s,%s,%s", regs_name[rd], regs_name[rs1],        \
            regs_name[rs2]);                                               \
    require_extension('M');                                                \
    if (!(int32_t) regs[rs2])                                              \
        regs[rd] = (int32_t) regs[rs1];                                    \
    else                                                                   \
        regs[rd] =                                                         \
            (int32_t)((int64_t)(int32_t) regs[rs1] % (int32_t) regs[rs2]); \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_REMUW                                                     \
    sprintf(remark, "remuw %s,%s,%s", regs_name[rd], regs_name[rs1],       \
            regs_name[rs2]);                                               \
    require_extension('M');                                                \
    if (!(uint32_t) regs[rs2])                                             \
        regs[rd] = (int32_t) regs[rs1];                                    \
    else                                                                   \
        regs[rd] = (int32_t)((uint32_t) regs[rs1] % (uint32_t) regs[rs2]); \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_C_ADDI4SPN                                   \
    sprintf(remark, "c.addi4spn %s,%s,%ld", regs_name[_c_rd], \
            regs_name[REG_SP], imm_c_addi4spn);               \
    require_extension('C');                                   \
    confirm_insn_legal(imm_c_addi4spn);                       \
    regs[_c_rd] = regs[REG_SP] + imm_c_addi4spn;              \
    pc += 2UL;                                                \
    break;

#define INSTRUCT_C_LW                                                  \
    sprintf(remark, "c.lw %s,%ld(%s)", regs_name[_c_rd], imm_c_slw,    \
            regs_name[_c_rs1]);                                        \
    require_extension('C');                                            \
    regs[_c_rd] = mmu->load(regs[_c_rs1] + imm_c_slw, DATA_TYPE_WORD); \
    pc += 2UL;                                                         \
    break;

#define INSTRUCT_C_LD                                                   \
    sprintf(remark, "c.ld %s,%ld(%s)", regs_name[_c_rd], imm_c_sld,     \
            regs_name[_c_rs1]);                                         \
    require_extension('C');                                             \
    regs[_c_rd] = mmu->load(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD); \
    pc += 2UL;                                                          \
    break;

#define INSTRUCT_C_FLD                                                   \
    sprintf(remark, "c.fld %s,%ld(%s)", fregs_name[_c_rd], imm_c_sld,    \
            regs_name[_c_rs1]);                                          \
    require_extension('C');                                              \
    require_extension('D');                                              \
    fregs[_c_rd] = mmu->load(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD); \
    pc += 2UL;                                                           \
    break;

#define INSTRUCT_C_SW                                                   \
    sprintf(remark, "c.sw %s,%ld(%s)", regs_name[_c_rs2], imm_c_slw,    \
            regs_name[_c_rs1]);                                         \
    require_extension('C');                                             \
    mmu->store(regs[_c_rs1] + imm_c_slw, DATA_TYPE_WORD, regs[_c_rs2]); \
    pc += 2UL;                                                          \
    break;

#define INSTRUCT_C_SD                                                    \
    sprintf(remark, "c.sd %s,%ld(%s)", regs_name[_c_rs2], imm_c_sld,     \
            regs_name[_c_rs1]);                                          \
    require_extension('C');                                              \
    mmu->store(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD, regs[_c_rs2]); \
    pc += 2UL;                                                           \
    break;

#define INSTRUCT_C_FSD                                                    \
    sprintf(remark, "c.fsd %s,%ld(%s)", fregs_name[_c_rs2], imm_c_sld,    \
            regs_name[_c_rs1]);                                           \
    require_extension('C');                                               \
    require_extension('D');                                               \
    mmu->store(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD, fregs[_c_rs2]); \
    pc += 2UL;                                                            \
    break;

#define INSTRUCT_C_ADDI                                                        \
    if (!rd && !imm_c_addi) {                                                  \
        sprintf(remark, "c.nop");                                              \
        require_extension('C');                                                \
    } else {                                                                   \
        sprintf(remark, "c.addi %s,%ld", regs_name[rd], (int64_t) imm_c_addi); \
        require_extension('C');                                                \
        confirm_insn_legal(rd &&imm_c_addi);                                   \
    }                                                                          \
    regs[rd] = regs[rd] + imm_c_addi;                                          \
    pc += 2UL;                                                                 \
    break;

#define INSTRUCT_C_ADDIW                                                    \
    sprintf(remark, "c.addiw %s,%ld", regs_name[rd], (int64_t) imm_c_addi); \
    require_extension('C');                                                 \
    confirm_insn_legal(rd);                                                 \
    regs[rd] = (int32_t) regs[rd] + (int32_t) imm_c_addi;                   \
    pc += 2UL;                                                              \
    break;

#define INSTRUCT_C_JAL                                         \
    sprintf(remark, "c.jal %08lx", pc + imm_c_j & 0xffffffff); \
    sprintf(remark, "%s (check)", remark);                     \
    require_extension('C');                                    \
    throw TrapIllegalInstruction(insn); /* RV32C only */       \
    regs[REG_RA] = pc + 2UL;                                   \
    pc += imm_c_j;                                             \
    break;

#define INSTRUCT_C_LI                                                    \
    sprintf(remark, "c.li %s,%ld", regs_name[rd], (int64_t) imm_c_addi); \
    require_extension('C');                                              \
    confirm_insn_legal(rd);                                              \
    regs[rd] = imm_c_addi;                                               \
    pc += 2UL;                                                           \
    break;

#define INSTRUCT_C_ADDI16SP                                 \
    sprintf(remark, "c.addi16sp %s,%ld", regs_name[REG_SP], \
            (int64_t) imm_c_addi16sp);                      \
    require_extension('C');                                 \
    confirm_insn_legal(imm_c_addi16sp &&rd == REG_SP);      \
    regs[REG_SP] = regs[REG_SP] + imm_c_addi16sp;           \
    pc += 2UL;                                              \
    break;

#define INSTRUCT_C_LUI                                 \
    sprintf(remark, "c.lui %s,0x%lx", regs_name[rd],   \
            imm_c_lui >> 12 & 0xfffff);                \
    require_extension('C');                            \
    confirm_insn_legal(rd &&imm_c_lui &&rd != REG_SP); \
    regs[rd] = imm_c_lui;                              \
    pc += 2UL;                                         \
    break;

#define INSTRUCT_C_SUB                                                    \
    sprintf(remark, "c.sub %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                               \
    regs[_c_rs1] = regs[_c_rs1] - regs[_c_rs2];                           \
    pc += 2UL;                                                            \
    break;

#define INSTRUCT_C_XOR                                                    \
    sprintf(remark, "c.xor %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                               \
    regs[_c_rs1] = regs[_c_rs1] ^ regs[_c_rs2];                           \
    pc += 2UL;                                                            \
    break;

#define INSTRUCT_C_OR                                                    \
    sprintf(remark, "c.or %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                              \
    regs[_c_rs1] = regs[_c_rs1] | regs[_c_rs2];                          \
    pc += 2UL;                                                           \
    break;

#define INSTRUCT_C_AND                                                    \
    sprintf(remark, "c.and %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                               \
    regs[_c_rs1] = regs[_c_rs1] & regs[_c_rs2];                           \
    pc += 2UL;                                                            \
    break;

#define INSTRUCT_C_SUBW                                                    \
    sprintf(remark, "c.subw %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                \
    regs[_c_rs1] = (int32_t) regs[_c_rs1] - (int32_t) regs[_c_rs2];        \
    pc += 2UL;                                                             \
    break;

#define INSTRUCT_C_ADDW                                                    \
    sprintf(remark, "c.addw %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                \
    regs[_c_rs1] = (int32_t) regs[_c_rs1] + (int32_t) regs[_c_rs2];        \
    pc += 2UL;                                                             \
    break;

#define INSTRUCT_C_SRLI                                            \
    sprintf(remark, "c.srli %s,0x%x", regs_name[_c_rs1], c_shamt); \
    require_extension('C');                                        \
    confirm_insn_legal(c_shamt);                                   \
    regs[_c_rs1] = regs[_c_rs1] >> c_shamt;                        \
    pc += 2UL;                                                     \
    break;

#define INSTRUCT_C_SRAI                                            \
    sprintf(remark, "c.srai %s,0x%x", regs_name[_c_rs1], c_shamt); \
    require_extension('C');                                        \
    confirm_insn_legal(c_shamt);                                   \
    regs[_c_rs1] = (int64_t) regs[_c_rs1] >> c_shamt;              \
    pc += 2UL;                                                     \
    break;

#define INSTRUCT_C_ANDI                                                        \
    sprintf(remark, "c.andi %s,%ld", regs_name[_c_rs1], (int64_t) imm_c_addi); \
    require_extension('C');                                                    \
    regs[_c_rs1] = regs[_c_rs1] & imm_c_addi;                                  \
    pc += 2UL;                                                                 \
    break;

#define INSTRUCT_C_J                                         \
    sprintf(remark, "c.j %08lx", pc + imm_c_j & 0xffffffff); \
    require_extension('C');                                  \
    pc += imm_c_j;                                           \
    break;

#define INSTRUCT_C_BEQZ                                   \
    sprintf(remark, "c.beqz %s,%08lx", regs_name[_c_rs1], \
            pc + imm_c_b & 0xffffffff);                   \
    require_extension('C');                               \
    pc += !regs[_c_rs1] ? imm_c_b : 2UL;                  \
    break;

#define INSTRUCT_C_BNEZ                                   \
    sprintf(remark, "c.bnez %s,%08lx", regs_name[_c_rs1], \
            pc + imm_c_b & 0xffffffff);                   \
    require_extension('C');                               \
    pc += regs[_c_rs1] ? imm_c_b : 2UL;                   \
    break;

#define INSTRUCT_C_SLLI                                        \
    sprintf(remark, "c.slli %s,0x%x", regs_name[rd], c_shamt); \
    require_extension('C');                                    \
    confirm_insn_legal(rd &&c_shamt);                          \
    regs[rd] = regs[rd] << c_shamt;                            \
    pc += 2UL;                                                 \
    break;

#define INSTRUCT_C_LWSP                                              \
    sprintf(remark, "c.lwsp %s,%ld(%s)", regs_name[rd], imm_c_lwsp,  \
            regs_name[REG_SP]);                                      \
    require_extension('C');                                          \
    confirm_insn_legal(rd);                                          \
    regs[rd] = mmu->load(regs[REG_SP] + imm_c_lwsp, DATA_TYPE_WORD); \
    pc += 2UL;                                                       \
    break;

#define INSTRUCT_C_LDSP                                               \
    sprintf(remark, "c.ldsp %s,%ld(%s)", regs_name[rd], imm_c_ldsp,   \
            regs_name[REG_SP]);                                       \
    require_extension('C');                                           \
    confirm_insn_legal(rd);                                           \
    regs[rd] = mmu->load(regs[REG_SP] + imm_c_ldsp, DATA_TYPE_DWORD); \
    pc += 2UL;                                                        \
    break;

#define INSTRUCT_C_FLDSP                                               \
    sprintf(remark, "c.fldsp %s,%ld(%s)", fregs_name[rd], imm_c_ldsp,  \
            regs_name[REG_SP]);                                        \
    require_extension('C');                                            \
    require_extension('D');                                            \
    fregs[rd] = mmu->load(regs[REG_SP] + imm_c_ldsp, DATA_TYPE_DWORD); \
    pc += 2UL;                                                         \
    break;

#define INSTRUCT_C_JR                                 \
    {                                                 \
        sprintf(remark, "c.jr %s", regs_name[c_rs1]); \
        require_extension('C');                       \
        confirm_insn_legal(c_rs1);                    \
        pc = (regs[c_rs1]) & ~1UL;                    \
    }                                                 \
    break;

#define INSTRUCT_C_MV                                               \
    sprintf(remark, "c.mv %s,%s", regs_name[rd], regs_name[c_rs2]); \
    require_extension('C');                                         \
    confirm_insn_legal(rd &&c_rs2);                                 \
    regs[rd] = regs[c_rs2];                                         \
    pc += 2UL;                                                      \
    break;

#define INSTRUCT_C_EBREAK        \
    sprintf(remark, "c.ebreak"); \
    require_extension('C');      \
    throw TrapBreakpoint(pc);    \
    break;

#define INSTRUCT_C_JALR                                 \
    {                                                   \
        sprintf(remark, "c.jalr %s", regs_name[c_rs1]); \
        require_extension('C');                         \
        confirm_insn_legal(c_rs1);                      \
        uint64_t rs1_data(regs[c_rs1]);                 \
        regs[REG_RA] = pc + 2UL;                        \
        pc = (rs1_data) & ~1UL;                         \
    }                                                   \
    break;

#define INSTRUCT_C_ADD                                               \
    sprintf(remark, "c.add %s,%s", regs_name[rd], regs_name[c_rs2]); \
    require_extension('C');                                          \
    confirm_insn_legal(rd &&c_rs2);                                  \
    regs[rd] = regs[rd] + regs[c_rs2];                               \
    pc += 2UL;                                                       \
    break;

#define INSTRUCT_C_SWSP                                                 \
    sprintf(remark, "c.swsp %s,%ld(%s)", regs_name[c_rs2], imm_c_swsp,  \
            regs_name[REG_SP]);                                         \
    require_extension('C');                                             \
    mmu->store(regs[REG_SP] + imm_c_swsp, DATA_TYPE_WORD, regs[c_rs2]); \
    pc += 2UL;                                                          \
    break;

#define INSTRUCT_C_SDSP                                                  \
    sprintf(remark, "c.sdsp %s,%ld(%s)", regs_name[c_rs2], imm_c_sdsp,   \
            regs_name[REG_SP]);                                          \
    require_extension('C');                                              \
    mmu->store(regs[REG_SP] + imm_c_sdsp, DATA_TYPE_DWORD, regs[c_rs2]); \
    pc += 2UL;                                                           \
    break;

#define INSTRUCT_C_FSDSP                                                  \
    sprintf(remark, "c.fsdsp %s,%ld(%s)", fregs_name[c_rs2], imm_c_sdsp,  \
            regs_name[REG_SP]);                                           \
    require_extension('C');                                               \
    require_extension('D');                                               \
    mmu->store(regs[REG_SP] + imm_c_sdsp, DATA_TYPE_DWORD, fregs[c_rs2]); \
    pc += 2UL;                                                            \
    break;

#define STR_AQ_RL(_aq, _rl) \
    (_aq) && (_rl) ? ".aq.rl" : (_aq) ? ".aq" : (_rl) ? ".rl" : ""

#define INSTRUCT_LR_W                                                    \
    sprintf(remark, "lr.w%s %s, (%s)", STR_AQ_RL(aq, rl), regs_name[rd], \
            regs_name[rs1]);                                             \
    sprintf(remark, "%s (check)", remark);                               \
    require_extension('A');                                              \
    if (regs[rs1] & 3UL)                                                 \
        throw TrapLoadAccessFault(regs[rs1]);                            \
    mmu->acquire_load_reservation(regs[rs1]);                            \
    regs[rd] = mmu->load(regs[rs1], DATA_TYPE_WORD);                     \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_SC_W                                             \
    {                                                             \
        sprintf(remark, "sc.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);   \
        require_extension('A');                                   \
        if (regs[rs1] & 3UL)                                      \
            throw TrapLoadAccessFault(regs[rs1]);                 \
        bool is_reserv(mmu->check_load_reservation(regs[rs1]));   \
        if (is_reserv)                                            \
            mmu->store(regs[rs1], DATA_TYPE_WORD, regs[rs2]);     \
        regs[rd] = !is_reserv;                                    \
        mmu->release_load_reservation();                          \
        pc += 4UL;                                                \
    }                                                             \
    break;

#define INSTRUCT_AMOSWAP_W                                            \
    sprintf(remark, "amoswap.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),    \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoswap_w_func);                 \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOADD_W                                             \
    sprintf(remark, "amoadd.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoadd_w_func);                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOXOR_W                                             \
    sprintf(remark, "amoxor.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoxor_w_func);                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOAND_W                                             \
    sprintf(remark, "amoand.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoand_w_func);                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOOR_W                                              \
    sprintf(remark, "amoor.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),      \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoor_w_func);                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOMIN_W                                             \
    sprintf(remark, "amomin.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amomin_w_func);                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOMAX_W                                             \
    sprintf(remark, "amomax.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amomax_w_func);                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOMINU_W                                            \
    sprintf(remark, "amominu.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),    \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amominu_w_func);                 \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOMAXU_W                                            \
    sprintf(remark, "amoswap.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),    \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);           \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amomaxu_w_func);                 \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_LR_D                                                    \
    sprintf(remark, "lr.d%s %s, (%s)", STR_AQ_RL(aq, rl), regs_name[rd], \
            regs_name[rs1]);                                             \
    sprintf(remark, "%s (check)", remark);                               \
    require_extension('A');                                              \
    if (regs[rs1] & 7UL)                                                 \
        throw TrapLoadAccessFault(regs[rs1]);                            \
    mmu->acquire_load_reservation(regs[rs1]);                            \
    regs[rd] = mmu->load(regs[rs1], DATA_TYPE_DWORD);                    \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_SC_D                                             \
    {                                                             \
        sprintf(remark, "sc.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);   \
        require_extension('A');                                   \
        if (regs[rs1] & 7UL)                                      \
            throw TrapLoadAccessFault(regs[rs1]);                 \
        bool is_reserv(mmu->check_load_reservation(regs[rs1]));   \
        if (is_reserv)                                            \
            mmu->store(regs[rs1], DATA_TYPE_DWORD, regs[rs2]);    \
        regs[rd] = !is_reserv;                                    \
        mmu->release_load_reservation();                          \
        pc += 4UL;                                                \
    }                                                             \
    break;

#define INSTRUCT_AMOSWAP_D                                             \
    sprintf(remark, "amoswap.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoswap_d_func);                  \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOADD_D                                              \
    sprintf(remark, "amoadd.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),      \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoadd_d_func);                   \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOXOR_D                                              \
    sprintf(remark, "amoxor.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),      \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoxor_d_func);                   \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOAND_D                                              \
    sprintf(remark, "amoand.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),      \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoand_d_func);                   \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOOR_D                                               \
    sprintf(remark, "amoor.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),       \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoor_d_func);                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMIN_D                                              \
    sprintf(remark, "amomin.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),      \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amomin_d_func);                   \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMAX_D                                              \
    sprintf(remark, "amomax.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),      \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amomax_d_func);                   \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMINU_D                                             \
    sprintf(remark, "amominu.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amominu_d_func);                  \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMAXU_D                                             \
    sprintf(remark, "amomaxu.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),     \
            regs_name[rd], regs_name[rs2], regs_name[rs1]);            \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amomaxu_d_func);                  \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FLW                                                   \
    sprintf(remark, "flw %s,%ld(%s)", fregs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                           \
    require_extension('F');                                            \
    require_fp;                                                        \
    fregs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD);          \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                   \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FSW                                                    \
    sprintf(remark, "fsw %s,%ld(%s)", fregs_name[rs2], (int64_t) imm_s, \
            regs_name[rs1]);                                            \
    require_extension('F');                                             \
    require_fp;                                                         \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_WORD, fregs[rs2]);          \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMADD_S                                                    \
    sprintf(remark, "fmadd.s %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2], fregs_name[rs3]);                              \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_add(f32_mul(fregs[rs1], fregs[rs2]), fregs[rs3]);       \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMSUB_S                                                       \
    sprintf(remark, "fmsub.s %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1],    \
            fregs_name[rs2], fregs_name[rs3]);                                 \
    require_extension('F');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f32_add(f32_mul(fregs[rs1], fregs[rs2]), F32_NEG(fregs[rs3])); \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                           \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMSUB_S                                                      \
    sprintf(remark, "fnmsub.s %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1],   \
            fregs_name[rs2], fregs_name[rs3]);                                 \
    require_extension('F');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f32_add(F32_NEG(f32_mul(fregs[rs1], fregs[rs2])), fregs[rs3]); \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                           \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMADD_S                                                    \
    sprintf(remark, "fnmadd.s %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2], fregs_name[rs3]);                               \
    require_extension('F');                                                  \
    require_fp;                                                              \
    clean_fflags_value();                                                    \
    fregs[rd] = f32_add(F32_NEG(f32_mul(fregs[rs1], fregs[rs2])),            \
                        F32_NEG(fregs[rs3]));                                \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                         \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                       \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_FADD_S                                                 \
    sprintf(remark, "fadd.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_add(fregs[rs1], fregs[rs2]);                        \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FSUB_S                                                 \
    sprintf(remark, "fsub.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_add(fregs[rs1], F32_NEG(fregs[rs2]));               \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMUL_S                                                 \
    sprintf(remark, "fmul.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_mul(fregs[rs1], fregs[rs2]);                        \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FDIV_S                                                 \
    sprintf(remark, "fdiv.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_div(fregs[rs1], fregs[rs2]);                        \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FSQRT_S                                               \
    sprintf(remark, "fsqrt.s %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    fregs[rd] = f32_sqrt(fregs[rs1]);                                  \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                   \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FSGNJ_S                                                   \
    sprintf(remark, "fsgnj.s %s,%s,%s", fregs_name[rd], fregs_name[rs1],   \
            fregs_name[rs2]);                                              \
    require_extension('F');                                                \
    require_fp;                                                            \
    if (!F32_IS_VALID(fregs[rs1]))                                         \
        fregs[rd] = !F32_IS_VALID(fregs[rs2])                              \
                        ? F32_DEFAULT_QUIET_NAN                            \
                        : F32_NEG(F32_DEFAULT_QUIET_NAN);                  \
    else if (!F32_IS_VALID(fregs[rs2]))                                    \
        fregs[rd] = fregs[rs1];                                            \
    else                                                                   \
        fregs[rd] = (fregs[rs2] & (1U << 31)) | (fregs[rs1] & (-1U >> 1)); \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                       \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FSGNJN_S                                                   \
    sprintf(remark, "fsgnjn.s %s,%s,%s", fregs_name[rd], fregs_name[rs1],   \
            fregs_name[rs2]);                                               \
    require_extension('F');                                                 \
    require_fp;                                                             \
    if (!F32_IS_VALID(fregs[rs1]))                                          \
        fregs[rd] = !F32_IS_VALID(fregs[rs2])                               \
                        ? F32_DEFAULT_QUIET_NAN                             \
                        : F32_NEG(F32_DEFAULT_QUIET_NAN);                   \
    else if (!F32_IS_VALID(fregs[rs2]))                                     \
        fregs[rd] = fregs[rs1];                                             \
    else                                                                    \
        fregs[rd] = (~fregs[rs2] & (1U << 31)) | (fregs[rs1] & (-1U >> 1)); \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FSGNJX_S                                                 \
    sprintf(remark, "fsgnjx.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                             \
    require_extension('F');                                               \
    require_fp;                                                           \
    if (!F32_IS_VALID(fregs[rs1]))                                        \
        fregs[rd] = !F32_IS_VALID(fregs[rs2])                             \
                        ? F32_DEFAULT_QUIET_NAN                           \
                        : F32_NEG(F32_DEFAULT_QUIET_NAN);                 \
    else if (!F32_IS_VALID(fregs[rs2]))                                   \
        fregs[rd] = fregs[rs1];                                           \
    else                                                                  \
        fregs[rd] = (fregs[rs2] & (1U << 31)) ^ fregs[rs1];               \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                      \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FMIN_S                                                 \
    sprintf(remark, "fmin.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_min(fregs[rs1], fregs[rs2]);                        \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMAX_S                                                 \
    sprintf(remark, "fmax.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_max(fregs[rs1], fregs[rs2]);                        \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FCVT_W_S                                              \
    sprintf(remark, "fcvt.w.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    regs[rd] = f32_cvt_w_s(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_WU_S                                              \
    sprintf(remark, "fcvt.wu.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    regs[rd] = (int32_t) f32_cvt_wu_s(fregs[rs1]);                      \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMV_X_W                                              \
    sprintf(remark, "fmv.x.w %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                           \
    require_fp;                                                       \
    regs[rd] = (int32_t) fregs[rs1];                                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FEQ_S                                                \
    sprintf(remark, "feq.s %s,%s,%s", regs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                         \
    require_extension('F');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    regs[rd] = f32_eq(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FLT_S                                                \
    sprintf(remark, "flt.s %s,%s,%s", regs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                         \
    require_extension('F');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    regs[rd] = f32_lt(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FLE_S                                                \
    sprintf(remark, "fle.s %s,%s,%s", regs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                         \
    require_extension('F');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    regs[rd] = f32_le(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FCLASS_S                                              \
    sprintf(remark, "fclass.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                            \
    require_fp;                                                        \
    regs[rd] = f32_classify(fregs[rs1]);                               \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_S_W                                              \
    sprintf(remark, "fcvt.s.w %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    fregs[rd] = f32_cvt_s_w(regs[rs1]);                                \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                   \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_S_WU                                              \
    sprintf(remark, "fcvt.s.wu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_cvt_s_wu(regs[rs1]);                                \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMV_W_X                                              \
    sprintf(remark, "fmv.w.x %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                           \
    require_fp;                                                       \
    fregs[rd] = (uint32_t) regs[rs1];                                 \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                  \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FCVT_L_S                                              \
    sprintf(remark, "fcvt.l.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    regs[rd] = f32_cvt_l_s(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_LU_S                                              \
    sprintf(remark, "fcvt.lu.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    regs[rd] = f32_cvt_lu_s(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FCVT_S_L                                              \
    sprintf(remark, "fcvt.s.l %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    fregs[rd] = f32_cvt_s_l(regs[rs1]);                                \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                   \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_S_LU                                              \
    sprintf(remark, "fcvt.s.lu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f32_cvt_s_lu(regs[rs1]);                                \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FLD                                                   \
    sprintf(remark, "fld %s,%ld(%s)", fregs_name[rd], (int64_t) imm_i, \
            regs_name[rs1]);                                           \
    require_extension('D');                                            \
    require_fp;                                                        \
    fregs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_DWORD);         \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FSD                                                    \
    sprintf(remark, "fsd %s,%ld(%s)", fregs_name[rs2], (int64_t) imm_s, \
            regs_name[rs1]);                                            \
    require_extension('D');                                             \
    require_fp;                                                         \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_DWORD, fregs[rs2]);         \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMADD_D                                                    \
    sprintf(remark, "fmadd.d %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2], fregs_name[rs3]);                              \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_add(f64_mul(fregs[rs1], fregs[rs2]), fregs[rs3]);       \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMSUB_D                                                       \
    sprintf(remark, "fmsub.d %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1],    \
            fregs_name[rs2], fregs_name[rs3]);                                 \
    require_extension('D');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f64_add(f64_mul(fregs[rs1], fregs[rs2]), F64_NEG(fregs[rs3])); \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMSUB_D                                                      \
    sprintf(remark, "fnmsub.d %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1],   \
            fregs_name[rs2], fregs_name[rs3]);                                 \
    require_extension('D');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f64_add(F64_NEG(f64_mul(fregs[rs1], fregs[rs2])), fregs[rs3]); \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMADD_D                                                    \
    sprintf(remark, "fnmadd.d %s,%s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2], fregs_name[rs3]);                               \
    require_extension('D');                                                  \
    require_fp;                                                              \
    clean_fflags_value();                                                    \
    fregs[rd] = f64_add(F64_NEG(f64_mul(fregs[rs1], fregs[rs2])),            \
                        F64_NEG(fregs[rs3]));                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                       \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_FADD_D                                                 \
    sprintf(remark, "fadd.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_add(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FSUB_D                                                 \
    sprintf(remark, "fsub.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_add(fregs[rs1], F64_NEG(fregs[rs2]));               \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMUL_D                                                 \
    sprintf(remark, "fmul.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_mul(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FDIV_D                                                 \
    sprintf(remark, "fdiv.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_div(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FSQRT_D                                               \
    sprintf(remark, "fsqrt.d %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    fregs[rd] = f64_sqrt(fregs[rs1]);                                  \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FSGNJ_D                                                 \
    sprintf(remark, "fsgnj.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                            \
    require_extension('D');                                              \
    require_fp;                                                          \
    fregs[rd] = (fregs[rs2] & (1UL << 63)) | (fregs[rs1] & (-1UL >> 1)); \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_FSGNJN_D                                                 \
    sprintf(remark, "fsgnjn.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                             \
    require_extension('D');                                               \
    require_fp;                                                           \
    fregs[rd] = (~fregs[rs2] & (1UL << 63)) | (fregs[rs1] & (-1UL >> 1)); \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FSGNJX_D                                                 \
    sprintf(remark, "fsgnjx.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                             \
    require_extension('D');                                               \
    require_fp;                                                           \
    fregs[rd] = (fregs[rs2] & (1UL << 63)) ^ fregs[rs1];                  \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FMIN_D                                                 \
    sprintf(remark, "fmin.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_min(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMAX_D                                                 \
    sprintf(remark, "fmax.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                           \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_max(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FCVT_S_D                                               \
    sprintf(remark, "fcvt.s.d %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_cvt_s_d(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FCVT_D_S                                               \
    sprintf(remark, "fcvt.d.s %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_cvt_d_s(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FEQ_D                                                \
    sprintf(remark, "feq.d %s,%s,%s", regs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                         \
    require_extension('D');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    regs[rd] = f64_eq(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FLT_D                                                \
    sprintf(remark, "flt.d %s,%s,%s", regs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                         \
    require_extension('D');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    regs[rd] = f64_lt(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FLE_D                                                \
    sprintf(remark, "fle.d %s,%s,%s", regs_name[rd], fregs_name[rs1], \
            fregs_name[rs2]);                                         \
    require_extension('D');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    regs[rd] = f64_le(fregs[rs1], fregs[rs2]);                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FCLASS_D                                              \
    sprintf(remark, "fclass.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                            \
    require_fp;                                                        \
    regs[rd] = f64_classify(fregs[rs1]);                               \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_W_D                                              \
    sprintf(remark, "fcvt.w.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    regs[rd] = f64_cvt_w_d(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_WU_D                                              \
    sprintf(remark, "fcvt.wu.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    regs[rd] = f64_cvt_wu_d(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FCVT_D_W                                              \
    sprintf(remark, "fcvt.d.w %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    fregs[rd] = f64_cvt_d_w(regs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_D_WU                                              \
    sprintf(remark, "fcvt.d.wu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_cvt_d_wu(regs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FCVT_L_D                                              \
    sprintf(remark, "fcvt.l.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    regs[rd] = f64_cvt_l_d(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_LU_D                                              \
    sprintf(remark, "fcvt.lu.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    regs[rd] = f64_cvt_lu_d(fregs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMV_X_D                                              \
    sprintf(remark, "fmv.x.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                           \
    require_fp;                                                       \
    regs[rd] = fregs[rs1];                                            \
    pc += 4UL;                                                        \
    break;
#define INSTRUCT_FCVT_D_L                                              \
    sprintf(remark, "fcvt.d.l %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                            \
    require_fp;                                                        \
    clean_fflags_value();                                              \
    fregs[rd] = f64_cvt_d_l(regs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                 \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FCVT_D_LU                                              \
    sprintf(remark, "fcvt.d.lu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                             \
    require_fp;                                                         \
    clean_fflags_value();                                               \
    fregs[rd] = f64_cvt_d_lu(regs[rs1]);                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                  \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FMV_D_X                                              \
    sprintf(remark, "fmv.d.x %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                           \
    require_fp;                                                       \
    fregs[rd] = regs[rs1];                                            \
    pc += 4UL;                                                        \
    break;

#endif
