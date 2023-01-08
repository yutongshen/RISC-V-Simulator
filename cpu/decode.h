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

#define confirm_insn_legal(x)                   \
    do {                                        \
        if (!(x)) {                             \
            throw TrapIllegalInstruction(insn); \
        }                                       \
    } while (0)

#define require_privilege(x) confirm_insn_legal(csr->prv >= (x))
#define require_extension(x) confirm_insn_legal(support_extension(x))
#define require_fp confirm_insn_legal(csr->mstatus &MSTATUS_FS)
#define set_sd_fs_dirty                                                     \
    {                                                                       \
        csr->mstatus |= MSTATUS_FS | (csr->max_xlen == 32) ? MSTATUS_32_SD  \
                                                           : MSTATUS_64_SD; \
    }

#define confirm_csr_legal(x, write)                                 \
    do {                                                            \
        if ((csr->mstatus & MSTATUS_TVM) && x == CSR_SATP_ADDR)     \
            require_privilege(PRV_M);                               \
        else                                                        \
            require_privilege(get_field((x), 0x300));               \
        confirm_insn_legal(!(get_field((x), 0xC00) == 3 && write)); \
    } while (0)

#define INSTRUCT_UNKNOWN                        \
    if (verbose)                                \
        sprintf(remark, "unknown instruction"); \
    throw TrapIllegalInstruction(insn);         \
    pc += 4UL;                                  \
    break;

#define INSTRUCT_LUI                                       \
    {                                                      \
        uint64_t imm_u(IMM_U(insn));                       \
        if (verbose)                                       \
            sprintf(remark, "lui %s,0x%lx", regs_name[rd], \
                    imm_u >> 12 & 0xfffff);                \
        regs[rd] = imm_u;                                  \
        write_reg = rd;                                    \
        pc += 4UL;                                         \
    }                                                      \
    break;

#define INSTRUCT_AUIPC                                       \
    {                                                        \
        uint64_t imm_u(IMM_U(insn));                         \
        if (verbose)                                         \
            sprintf(remark, "auipc %s,0x%lx", regs_name[rd], \
                    imm_u >> 12 & 0xfffff);                  \
        regs[rd] = imm_u + pc;                               \
        write_reg = rd;                                      \
        pc += 4UL;                                           \
    }                                                        \
    break;

#define INSTRUCT_JAL                                                        \
    {                                                                       \
        uint64_t imm_j(IMM_J(insn));                                        \
        if (verbose) {                                                      \
            if (!rd)                                                        \
                sprintf(remark, "j %08lx", pc + imm_j);                     \
            else if (rd == REG_RA)                                          \
                sprintf(remark, "jal %08lx", pc + imm_j);                   \
            else                                                            \
                sprintf(remark, "jal %s,%08lx", regs_name[rd], pc + imm_j); \
        }                                                                   \
        ras_push = rd == REG_RA;                                            \
        regs[rd] = pc + 4UL;                                                \
        write_reg = rd;                                                     \
        pc += imm_j;                                                        \
    }                                                                       \
    break;

#define INSTRUCT_JALR                                             \
    {                                                             \
        uint64_t imm_i(IMM_I(insn));                              \
        if (verbose) {                                            \
            if (imm_i || !(rd == REG_ZERO || rd == REG_RA))       \
                sprintf(remark, "jalr %s,%ld(%s)", regs_name[rd], \
                        (int64_t) imm_i, regs_name[rs1]);         \
            else if (rd == REG_RA)                                \
                sprintf(remark, "jalr %s", regs_name[rs1]);       \
            else if (rs1 == REG_RA)                               \
                sprintf(remark, "ret");                           \
            else                                                  \
                sprintf(remark, "jr %s", regs_name[rs1]);         \
        }                                                         \
        ras_push = rd == REG_RA;                                  \
        ras_pop = rs1 == REG_RA && !imm_i;                        \
        uint64_t rs1_data(regs[rs1]);                             \
        regs[rd] = pc + 4UL;                                      \
        write_reg = rd;                                           \
        pc = (imm_i + rs1_data) & ~1UL;                           \
    }                                                             \
    break;

#define INSTRUCT_BEQ                                                           \
    if (verbose) {                                                             \
        if (rs2)                                                               \
            sprintf(remark, "beq %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                    pc + imm_b);                                               \
        else                                                                   \
            sprintf(remark, "beqz %s,%08lx", regs_name[rs1], pc + imm_b);      \
    }                                                                          \
    pc += regs[rs1] == regs[rs2] ? imm_b : 4UL;                                \
    break;

#define INSTRUCT_BNE                                                           \
    if (verbose) {                                                             \
        if (rs2)                                                               \
            sprintf(remark, "bne %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                    pc + imm_b);                                               \
        else                                                                   \
            sprintf(remark, "bnez %s,%08lx", regs_name[rs1], pc + imm_b);      \
    }                                                                          \
    pc += regs[rs1] != regs[rs2] ? imm_b : 4UL;                                \
    break;

#define INSTRUCT_BLT                                                           \
    if (verbose) {                                                             \
        if (!rs1)                                                              \
            sprintf(remark, "bgtz %s,%08lx", regs_name[rs2], pc + imm_b);      \
        else if (!rs2)                                                         \
            sprintf(remark, "bltz %s,%08lx", regs_name[rs1], pc + imm_b);      \
        else                                                                   \
            sprintf(remark, "blt %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                    pc + imm_b);                                               \
    }                                                                          \
    pc += (int64_t) regs[rs1] < (int64_t) regs[rs2] ? imm_b : 4UL;             \
    break;

#define INSTRUCT_BGE                                                           \
    if (verbose) {                                                             \
        if (!rs1)                                                              \
            sprintf(remark, "blez %s,%08lx", regs_name[rs2], pc + imm_b);      \
        else if (!rs2)                                                         \
            sprintf(remark, "bgez %s,%08lx", regs_name[rs1], pc + imm_b);      \
        else                                                                   \
            sprintf(remark, "bge %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                    pc + imm_b);                                               \
    }                                                                          \
    pc += (int64_t) regs[rs1] >= (int64_t) regs[rs2] ? imm_b : 4UL;            \
    break;

#define INSTRUCT_BLTU                                                       \
    if (verbose)                                                            \
        sprintf(remark, "bltu %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                pc + imm_b);                                                \
    pc += regs[rs1] < regs[rs2] ? imm_b : 4UL;                              \
    break;

#define INSTRUCT_BGEU                                                       \
    if (verbose)                                                            \
        sprintf(remark, "bgeu %s,%s,%08lx", regs_name[rs1], regs_name[rs2], \
                pc + imm_b);                                                \
    pc += regs[rs1] >= regs[rs2] ? imm_b : 4UL;                             \
    break;

#define INSTRUCT_LB                                                      \
    if (verbose)                                                         \
        sprintf(remark, "lb %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                         \
    mem_addr = regs[rs1] + imm_i;                                        \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_BYTE);             \
    load_store = LS_LOAD;                                                \
    mem_val = regs[rd];                                                  \
    mem_val_type = DATA_TYPE_BYTE;                                       \
    write_reg = rd;                                                      \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_LH                                                      \
    if (verbose)                                                         \
        sprintf(remark, "lh %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                         \
    mem_addr = regs[rs1] + imm_i;                                        \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_HWORD);            \
    load_store = LS_LOAD;                                                \
    mem_val = regs[rd];                                                  \
    mem_val_type = DATA_TYPE_HWORD;                                      \
    write_reg = rd;                                                      \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_LW                                                      \
    if (verbose)                                                         \
        sprintf(remark, "lw %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                         \
    mem_addr = regs[rs1] + imm_i;                                        \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD);             \
    load_store = LS_LOAD;                                                \
    mem_val = regs[rd];                                                  \
    mem_val_type = DATA_TYPE_WORD;                                       \
    write_reg = rd;                                                      \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_LBU                                                      \
    if (verbose)                                                          \
        sprintf(remark, "lbu %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                          \
    mem_addr = regs[rs1] + imm_i;                                         \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_BYTE_UNSIGNED);     \
    load_store = LS_LOAD;                                                 \
    mem_val = regs[rd];                                                   \
    mem_val_type = DATA_TYPE_BYTE_UNSIGNED;                               \
    write_reg = rd;                                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_LHU                                                      \
    if (verbose)                                                          \
        sprintf(remark, "lhu %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                          \
    mem_addr = regs[rs1] + imm_i;                                         \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_HWORD_UNSIGNED);    \
    load_store = LS_LOAD;                                                 \
    mem_val = regs[rd];                                                   \
    mem_val_type = DATA_TYPE_HWORD_UNSIGNED;                              \
    write_reg = rd;                                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_LWU                                                      \
    if (verbose)                                                          \
        sprintf(remark, "lwu %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                          \
    mem_addr = regs[rs1] + imm_i;                                         \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD_UNSIGNED);     \
    load_store = LS_LOAD;                                                 \
    mem_val = regs[rd];                                                   \
    mem_val_type = DATA_TYPE_WORD_UNSIGNED;                               \
    write_reg = rd;                                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_LD                                                      \
    if (verbose)                                                         \
        sprintf(remark, "ld %s,%ld(%s)", regs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                         \
    mem_addr = regs[rs1] + imm_i;                                        \
    regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_DWORD);            \
    load_store = LS_LOAD;                                                \
    mem_val = regs[rd];                                                  \
    mem_val_type = DATA_TYPE_DWORD;                                      \
    write_reg = rd;                                                      \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_SB                                                       \
    if (verbose)                                                          \
        sprintf(remark, "sb %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
                regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_BYTE, regs[rs2]);             \
    load_store = LS_STORE;                                                \
    mem_addr = regs[rs1] + imm_s;                                         \
    mem_val = regs[rs2];                                                  \
    mem_val_type = DATA_TYPE_BYTE;                                        \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_SH                                                       \
    if (verbose)                                                          \
        sprintf(remark, "sh %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
                regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_HWORD, regs[rs2]);            \
    load_store = LS_STORE;                                                \
    mem_addr = regs[rs1] + imm_s;                                         \
    mem_val = regs[rs2];                                                  \
    mem_val_type = DATA_TYPE_HWORD;                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_SW                                                       \
    if (verbose)                                                          \
        sprintf(remark, "sw %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
                regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_WORD, regs[rs2]);             \
    load_store = LS_STORE;                                                \
    mem_addr = regs[rs1] + imm_s;                                         \
    mem_val = regs[rs2];                                                  \
    mem_val_type = DATA_TYPE_WORD;                                        \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_SD                                                       \
    if (verbose)                                                          \
        sprintf(remark, "sd %s,%ld(%s)", regs_name[rs2], (int64_t) imm_s, \
                regs_name[rs1]);                                          \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_DWORD, regs[rs2]);            \
    load_store = LS_STORE;                                                \
    mem_addr = regs[rs1] + imm_s;                                         \
    mem_val = regs[rs2];                                                  \
    mem_val_type = DATA_TYPE_DWORD;                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_ADDI                                                        \
    if (verbose) {                                                           \
        if (!rd && !rs1 && !imm_i)                                           \
            sprintf(remark, "nop");                                          \
        else if (!rs1)                                                       \
            sprintf(remark, "li %s,%ld", regs_name[rd], (int64_t) imm_i);    \
        else if (!imm_i)                                                     \
            sprintf(remark, "mv %s,%s", regs_name[rd], regs_name[rs1]);      \
        else                                                                 \
            sprintf(remark, "addi %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                    (int64_t) imm_i);                                        \
    }                                                                        \
    regs[rd] = regs[rs1] + imm_i;                                            \
    write_reg = rd;                                                          \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_SLTI                                                    \
    if (verbose)                                                         \
        sprintf(remark, "slti %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                (int64_t) imm_i);                                        \
    regs[rd] = (int64_t) regs[rs1] < (int64_t) imm_i;                    \
    write_reg = rd;                                                      \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_SLTIU                                                    \
    if (verbose)                                                          \
        sprintf(remark, "sltiu %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                (int64_t) imm_i);                                         \
    regs[rd] = regs[rs1] < imm_i;                                         \
    write_reg = rd;                                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_XORI                                                        \
    if (verbose) {                                                           \
        if (imm_i == -1U)                                                    \
            sprintf(remark, "not %s,%s", regs_name[rd], regs_name[rs1]);     \
        else                                                                 \
            sprintf(remark, "xori %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                    (int64_t) imm_i);                                        \
    }                                                                        \
    regs[rd] = regs[rs1] ^ imm_i;                                            \
    write_reg = rd;                                                          \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_ORI                                                    \
    if (verbose)                                                        \
        sprintf(remark, "ori %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                (int64_t) imm_i);                                       \
    regs[rd] = regs[rs1] | imm_i;                                       \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_ANDI                                                    \
    if (verbose)                                                         \
        sprintf(remark, "andi %s,%s,%ld", regs_name[rd], regs_name[rs1], \
                (int64_t) imm_i);                                        \
    regs[rd] = regs[rs1] & imm_i;                                        \
    write_reg = rd;                                                      \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_SLLI                                                         \
    {                                                                         \
        uint32_t shamt64(SHAMT64(insn));                                      \
        if (verbose)                                                          \
            sprintf(remark, "slli %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
                    shamt64);                                                 \
        regs[rd] = regs[rs1] << shamt64;                                      \
        write_reg = rd;                                                       \
        pc += 4UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_SRLI                                                         \
    {                                                                         \
        uint32_t shamt64(SHAMT64(insn));                                      \
        if (verbose)                                                          \
            sprintf(remark, "srli %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
                    shamt64);                                                 \
        regs[rd] = regs[rs1] >> shamt64;                                      \
        write_reg = rd;                                                       \
        pc += 4UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_SRAI                                                         \
    {                                                                         \
        uint32_t shamt64(SHAMT64(insn));                                      \
        if (verbose)                                                          \
            sprintf(remark, "srai %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
                    shamt64);                                                 \
        regs[rd] = (int64_t) regs[rs1] >> shamt64;                            \
        write_reg = rd;                                                       \
        pc += 4UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_ADD                                                   \
    if (verbose)                                                       \
        sprintf(remark, "add %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] + regs[rs2];                                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_SLL                                                   \
    if (verbose)                                                       \
        sprintf(remark, "sll %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] << (regs[rs2] & 0x3F);                        \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_SLT                                                   \
    if (verbose)                                                       \
        sprintf(remark, "slt %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = (int64_t) regs[rs1] < (int64_t) regs[rs2];              \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_SLTU                                                   \
    if (verbose)                                                        \
        sprintf(remark, "sltu %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    regs[rd] = regs[rs1] < regs[rs2];                                   \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_XOR                                                   \
    if (verbose)                                                       \
        sprintf(remark, "xor %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] ^ regs[rs2];                                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_SRL                                                   \
    if (verbose)                                                       \
        sprintf(remark, "srl %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] >> (regs[rs2] & 0x3F);                        \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_OR                                                   \
    if (verbose)                                                      \
        sprintf(remark, "or %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                      \
    regs[rd] = regs[rs1] | regs[rs2];                                 \
    write_reg = rd;                                                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AND                                                   \
    if (verbose)                                                       \
        sprintf(remark, "and %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] & regs[rs2];                                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_SUB                                                   \
    if (verbose)                                                       \
        sprintf(remark, "sub %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = regs[rs1] - regs[rs2];                                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_SRA                                                   \
    if (verbose)                                                       \
        sprintf(remark, "sra %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    regs[rd] = (int64_t) regs[rs1] >> (regs[rs2] & 0x3F);              \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_MUL                                                   \
    if (verbose)                                                       \
        sprintf(remark, "mul %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    require_extension('M');                                            \
    regs[rd] = regs[rs1] * regs[rs2];                                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_MULH                                                   \
    if (verbose)                                                        \
        sprintf(remark, "mulh %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    require_extension('M');                                             \
    regs[rd] = mulh(regs[rs1], regs[rs2]);                              \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_MULHSU                                                   \
    if (verbose)                                                          \
        sprintf(remark, "mulhsu %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                          \
    require_extension('M');                                               \
    regs[rd] = mulhsu(regs[rs1], regs[rs2]);                              \
    write_reg = rd;                                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_MULHU                                                   \
    if (verbose)                                                         \
        sprintf(remark, "mulhu %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                         \
    require_extension('M');                                              \
    regs[rd] = mulhu(regs[rs1], regs[rs2]);                              \
    write_reg = rd;                                                      \
    pc += 4UL;                                                           \
    break;

#define INSTRUCT_DIV                                                   \
    if (verbose)                                                       \
        sprintf(remark, "div %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    require_extension('M');                                            \
    if (!regs[rs2])                                                    \
        regs[rd] = -1UL;                                               \
    else if (regs[rs1] == 1UL << 63 && regs[rs2] == -1UL)              \
        regs[rd] = regs[rs1];                                          \
    else                                                               \
        regs[rd] = (int64_t) regs[rs1] / (int64_t) regs[rs2];          \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_DIVU                                                   \
    if (verbose)                                                        \
        sprintf(remark, "divu %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    require_extension('M');                                             \
    if (!regs[rs2])                                                     \
        regs[rd] = -1UL;                                                \
    else                                                                \
        regs[rd] = regs[rs1] / regs[rs2];                               \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_REM                                                   \
    if (verbose)                                                       \
        sprintf(remark, "rem %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                       \
    require_extension('M');                                            \
    if (!regs[rs2])                                                    \
        regs[rd] = regs[rs1];                                          \
    else if (regs[rs1] == (1UL << 63) && regs[rs2] == -1UL)            \
        regs[rd] = 0;                                                  \
    else                                                               \
        regs[rd] = (int64_t) regs[rs1] % (int64_t) regs[rs2];          \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_REMU                                                   \
    if (verbose)                                                        \
        sprintf(remark, "remu %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    require_extension('M');                                             \
    if (!regs[rs2])                                                     \
        regs[rd] = regs[rs1];                                           \
    else                                                                \
        regs[rd] = regs[rs1] % regs[rs2];                               \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_FENCE                                             \
    if (verbose) {                                                 \
        if (PRED(insn) != 0xf | SUCC(insn) != 0xf)                 \
            sprintf(remark, "fence %s,%s", fence_flag(PRED(insn)), \
                    fence_flag(SUCC(insn)));                       \
        else                                                       \
            sprintf(remark, "fence");                              \
    }                                                              \
    pc += 4UL;                                                     \
    break;

#define INSTRUCT_FENCE_I            \
    if (verbose)                    \
        sprintf(remark, "fence.i"); \
    pc += 4UL;                      \
    break;

#define INSTRUCT_ECALL               \
    if (verbose)                     \
        sprintf(remark, "ecall");    \
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

#define INSTRUCT_EBREAK            \
    if (verbose)                   \
        sprintf(remark, "ebreak"); \
    throw TrapBreakpoint(pc);      \
    break;

#define INSTRUCT_WFI                                                \
    if (verbose)                                                    \
        sprintf(remark, "wfi");                                     \
    require_privilege((csr->mstatus & MSTATUS_TW) ? PRV_M : PRV_S); \
    throw WaitForInterrupt();                                       \
    break;

#define INSTRUCT_SRET                                                        \
    {                                                                        \
        if (verbose)                                                         \
            sprintf(remark, "sret");                                         \
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
        if (verbose)                                                         \
            sprintf(remark, "mret");                                         \
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
    if (verbose)                                                             \
        sprintf(remark, "sfence.vma %s,%s", regs_name[rs1], regs_name[rs2]); \
    require_privilege((csr->mstatus & MSTATUS_TVM) ? PRV_M : PRV_S);         \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_CSRRW                                                 \
    {                                                                  \
        uint32_t csr_addr(CSR_ADDR(insn));                             \
        if (verbose) {                                                 \
            if (rd)                                                    \
                sprintf(remark, "csrrw %s,%s,%s", regs_name[rd],       \
                        csr->csr_name(csr_addr), regs_name[rs1]);      \
            else                                                       \
                sprintf(remark, "csrw %s,%s", csr->csr_name(csr_addr), \
                        regs_name[rs1]);                               \
        }                                                              \
        confirm_csr_legal(csr_addr, 1);                                \
        uint64_t tmp(csr->get_csr(csr_addr));                          \
        csr->set_csr(csr_addr, regs[rs1]);                             \
        regs[rd] = tmp;                                                \
        write_reg = rd;                                                \
        write_csr = csr_addr;                                          \
        pc += 4UL;                                                     \
    }                                                                  \
    break;

#define INSTRUCT_CSRRS                                                 \
    {                                                                  \
        uint32_t csr_addr(CSR_ADDR(insn));                             \
        if (verbose) {                                                 \
            if (!rs1)                                                  \
                sprintf(remark, "csrr %s,%s", regs_name[rd],           \
                        csr->csr_name(csr_addr));                      \
            else if (!rd)                                              \
                sprintf(remark, "csrs %s,%s", csr->csr_name(csr_addr), \
                        regs_name[rs1]);                               \
            else                                                       \
                sprintf(remark, "csrrs %s,%s,%s", regs_name[rd],       \
                        csr->csr_name(csr_addr), regs_name[rs1]);      \
        }                                                              \
        confirm_csr_legal(csr_addr, rs1);                              \
        uint64_t tmp(csr->get_csr(csr_addr));                          \
        csr->set_csr(csr_addr, tmp | regs[rs1]);                       \
        regs[rd] = tmp;                                                \
        write_reg = rd;                                                \
        write_csr = csr_addr;                                          \
        pc += 4UL;                                                     \
    }                                                                  \
    break;

#define INSTRUCT_CSRRC                                                 \
    {                                                                  \
        uint32_t csr_addr(CSR_ADDR(insn));                             \
        if (verbose) {                                                 \
            if (rd)                                                    \
                sprintf(remark, "csrrc %s,%s,%s", regs_name[rd],       \
                        csr->csr_name(csr_addr), regs_name[rs1]);      \
            else                                                       \
                sprintf(remark, "csrc %s,%s", csr->csr_name(csr_addr), \
                        regs_name[rs1]);                               \
        }                                                              \
        confirm_csr_legal(csr_addr, rs1);                              \
        uint64_t tmp(csr->get_csr(csr_addr));                          \
        csr->set_csr(csr_addr, tmp & ~regs[rs1]);                      \
        regs[rd] = tmp;                                                \
        write_reg = rd;                                                \
        write_csr = csr_addr;                                          \
        pc += 4UL;                                                     \
    }                                                                  \
    break;

#define INSTRUCT_CSRRWI                                                       \
    {                                                                         \
        uint32_t csr_addr(CSR_ADDR(insn));                                    \
        if (verbose) {                                                        \
            if (rd)                                                           \
                sprintf(remark, "csrrwi %s,%s,%d", regs_name[rd],             \
                        csr->csr_name(csr_addr), rs1);                        \
            else                                                              \
                sprintf(remark, "csrwi %s,%d", csr->csr_name(csr_addr), rs1); \
        }                                                                     \
        confirm_csr_legal(csr_addr, 1);                                       \
        regs[rd] = csr->get_csr(csr_addr);                                    \
        csr->set_csr(csr_addr, zext(rs1, 5));                                 \
        write_reg = rd;                                                       \
        write_csr = csr_addr;                                                 \
        pc += 4UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_CSRRSI                                                       \
    {                                                                         \
        uint32_t csr_addr(CSR_ADDR(insn));                                    \
        if (verbose) {                                                        \
            if (rd)                                                           \
                sprintf(remark, "csrrsi %s,%s,%d", regs_name[rd],             \
                        csr->csr_name(csr_addr), rs1);                        \
            else                                                              \
                sprintf(remark, "csrsi %s,%d", csr->csr_name(csr_addr), rs1); \
        }                                                                     \
        confirm_csr_legal(csr_addr, rs1);                                     \
        uint64_t tmp(csr->get_csr(csr_addr));                                 \
        csr->set_csr(csr_addr, tmp | zext(rs1, 5));                           \
        regs[rd] = tmp;                                                       \
        write_reg = rd;                                                       \
        write_csr = csr_addr;                                                 \
        pc += 4UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_CSRRCI                                                       \
    {                                                                         \
        uint32_t csr_addr(CSR_ADDR(insn));                                    \
        if (verbose) {                                                        \
            if (rd)                                                           \
                sprintf(remark, "csrrci %s,%s,%d", regs_name[rd],             \
                        csr->csr_name(csr_addr), rs1);                        \
            else                                                              \
                sprintf(remark, "csrci %s,%d", csr->csr_name(csr_addr), rs1); \
        }                                                                     \
        confirm_csr_legal(csr_addr, rs1);                                     \
        uint64_t tmp(csr->get_csr(csr_addr));                                 \
        csr->set_csr(csr_addr, tmp &(~zext(rs1, 5)));                         \
        regs[rd] = tmp;                                                       \
        write_reg = rd;                                                       \
        write_csr = csr_addr;                                                 \
        pc += 4UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_ADDIW                                            \
    {                                                             \
        uint64_t imm_i(IMM_I(insn));                              \
        if (verbose) {                                            \
            if (imm_i)                                            \
                sprintf(remark, "addiw %s,%s,%ld", regs_name[rd], \
                        regs_name[rs1], (int64_t) imm_i);         \
            else                                                  \
                sprintf(remark, "sext.w %s,%s", regs_name[rd],    \
                        regs_name[rs1]);                          \
        }                                                         \
        regs[rd] = (int32_t) regs[rs1] + (int32_t) imm_i;         \
        write_reg = rd;                                           \
        pc += 4UL;                                                \
    }                                                             \
    break;

#define INSTRUCT_SLLIW                                                     \
    if (verbose)                                                           \
        sprintf(remark, "slliw %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
                shamt);                                                    \
    regs[rd] = (int32_t) regs[rs1] << shamt;                               \
    write_reg = rd;                                                        \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_SRLIW                                                     \
    if (verbose)                                                           \
        sprintf(remark, "srliw %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
                shamt);                                                    \
    regs[rd] = (int32_t) ((uint32_t) regs[rs1] >> shamt);                  \
    write_reg = rd;                                                        \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_SRAIW                                                     \
    if (verbose)                                                           \
        sprintf(remark, "sraiw %s,%s,0x%x", regs_name[rd], regs_name[rs1], \
                shamt);                                                    \
    regs[rd] = ((int32_t) regs[rs1] >> shamt);                             \
    write_reg = rd;                                                        \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_ADDW                                                   \
    if (verbose)                                                        \
        sprintf(remark, "addw %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    regs[rd] = (int32_t) regs[rs1] + (int32_t) regs[rs2];               \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_SUBW                                                   \
    if (verbose)                                                        \
        sprintf(remark, "subw %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    regs[rd] = (int32_t) regs[rs1] - (int32_t) regs[rs2];               \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_MULW                                                   \
    if (verbose)                                                        \
        sprintf(remark, "mulw %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    require_extension('M');                                             \
    regs[rd] = (regs[rs1] * regs[rs2]) << 32 >> 32;                     \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_SLLW                                                   \
    if (verbose)                                                        \
        sprintf(remark, "sllw %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    regs[rd] = (int32_t) regs[rs1] << (regs[rs2] & 0x1F);               \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_DIVW                                                        \
    if (verbose)                                                             \
        sprintf(remark, "divw %s,%s,%s", regs_name[rd], regs_name[rs1],      \
                regs_name[rs2]);                                             \
    require_extension('M');                                                  \
    if (!(int32_t) regs[rs2])                                                \
        regs[rd] = -1UL;                                                     \
    else                                                                     \
        regs[rd] =                                                           \
            (int32_t) ((int64_t) (int32_t) regs[rs1] / (int32_t) regs[rs2]); \
    write_reg = rd;                                                          \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_SRLW                                                      \
    if (verbose)                                                           \
        sprintf(remark, "srlw %s,%s,%s", regs_name[rd], regs_name[rs1],    \
                regs_name[rs2]);                                           \
    regs[rd] =                                                             \
        (int32_t) ((uint32_t) regs[rs1] >> (uint32_t) (regs[rs2] & 0x1F)); \
    write_reg = rd;                                                        \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_SRAW                                                   \
    if (verbose)                                                        \
        sprintf(remark, "sraw %s,%s,%s", regs_name[rd], regs_name[rs1], \
                regs_name[rs2]);                                        \
    regs[rd] = ((int32_t) regs[rs1] >> (uint32_t) (regs[rs2] & 0x1F));  \
    write_reg = rd;                                                     \
    pc += 4UL;                                                          \
    break;

#define INSTRUCT_DIVUW                                                      \
    if (verbose)                                                            \
        sprintf(remark, "divuw %s,%s,%s", regs_name[rd], regs_name[rs1],    \
                regs_name[rs2]);                                            \
    require_extension('M');                                                 \
    if (!(uint32_t) regs[rs2])                                              \
        regs[rd] = -1UL;                                                    \
    else                                                                    \
        regs[rd] = (int32_t) ((uint32_t) regs[rs1] / (uint32_t) regs[rs2]); \
    write_reg = rd;                                                         \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_REMW                                                        \
    if (verbose)                                                             \
        sprintf(remark, "remw %s,%s,%s", regs_name[rd], regs_name[rs1],      \
                regs_name[rs2]);                                             \
    require_extension('M');                                                  \
    if (!(int32_t) regs[rs2])                                                \
        regs[rd] = (int32_t) regs[rs1];                                      \
    else                                                                     \
        regs[rd] =                                                           \
            (int32_t) ((int64_t) (int32_t) regs[rs1] % (int32_t) regs[rs2]); \
    write_reg = rd;                                                          \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_REMUW                                                      \
    if (verbose)                                                            \
        sprintf(remark, "remuw %s,%s,%s", regs_name[rd], regs_name[rs1],    \
                regs_name[rs2]);                                            \
    require_extension('M');                                                 \
    if (!(uint32_t) regs[rs2])                                              \
        regs[rd] = (int32_t) regs[rs1];                                     \
    else                                                                    \
        regs[rd] = (int32_t) ((uint32_t) regs[rs1] % (uint32_t) regs[rs2]); \
    write_reg = rd;                                                         \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_C_ADDI4SPN                                           \
    {                                                                 \
        uint64_t imm_c_addi4spn(IMM_C_ADDI4SPN(insn));                \
        if (verbose)                                                  \
            sprintf(remark, "c.addi4spn %s,%s,%ld", regs_name[_c_rd], \
                    regs_name[REG_SP], imm_c_addi4spn);               \
        require_extension('C');                                       \
        confirm_insn_legal(imm_c_addi4spn);                           \
        regs[_c_rd] = regs[REG_SP] + imm_c_addi4spn;                  \
        write_reg = _c_rd;                                            \
        pc += 2UL;                                                    \
    }                                                                 \
    break;

#define INSTRUCT_C_LW                                                       \
    {                                                                       \
        uint64_t imm_c_slw(IMM_C_SLW(insn));                                \
        if (verbose)                                                        \
            sprintf(remark, "c.lw %s,%ld(%s)", regs_name[_c_rd], imm_c_slw, \
                    regs_name[_c_rs1]);                                     \
        require_extension('C');                                             \
        mem_addr = regs[_c_rs1] + imm_c_slw;                                \
        regs[_c_rd] = mmu->load(regs[_c_rs1] + imm_c_slw, DATA_TYPE_WORD);  \
        load_store = LS_LOAD;                                               \
        mem_val = regs[_c_rd];                                              \
        mem_val_type = DATA_TYPE_WORD;                                      \
        write_reg = _c_rd;                                                  \
        pc += 2UL;                                                          \
    }                                                                       \
    break;

#define INSTRUCT_C_LD                                                       \
    {                                                                       \
        uint64_t imm_c_sld(IMM_C_SLD(insn));                                \
        if (verbose)                                                        \
            sprintf(remark, "c.ld %s,%ld(%s)", regs_name[_c_rd], imm_c_sld, \
                    regs_name[_c_rs1]);                                     \
        require_extension('C');                                             \
        mem_addr = regs[_c_rs1] + imm_c_sld;                                \
        regs[_c_rd] = mmu->load(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD); \
        load_store = LS_LOAD;                                               \
        mem_val = regs[_c_rd];                                              \
        mem_val_type = DATA_TYPE_DWORD;                                     \
        write_reg = _c_rd;                                                  \
        pc += 2UL;                                                          \
    }                                                                       \
    break;

#define INSTRUCT_C_FLD                                                        \
    {                                                                         \
        uint64_t imm_c_sld(IMM_C_SLD(insn));                                  \
        if (verbose)                                                          \
            sprintf(remark, "c.fld %s,%ld(%s)", fregs_name[_c_rd], imm_c_sld, \
                    regs_name[_c_rs1]);                                       \
        require_extension('C');                                               \
        require_extension('D');                                               \
        require_fp;                                                           \
        mem_addr = regs[_c_rs1] + imm_c_sld;                                  \
        fregs[_c_rd] = mmu->load(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD);  \
        set_sd_fs_dirty;                                                      \
        load_store = LS_LOAD;                                                 \
        mem_val = fregs[_c_rd];                                               \
        mem_val_type = DATA_TYPE_DWORD;                                       \
        write_reg = _c_rd | 0x20;                                             \
        pc += 2UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_C_SW                                                        \
    {                                                                        \
        uint64_t imm_c_slw(IMM_C_SLW(insn));                                 \
        if (verbose)                                                         \
            sprintf(remark, "c.sw %s,%ld(%s)", regs_name[_c_rs2], imm_c_slw, \
                    regs_name[_c_rs1]);                                      \
        require_extension('C');                                              \
        mmu->store(regs[_c_rs1] + imm_c_slw, DATA_TYPE_WORD, regs[_c_rs2]);  \
        load_store = LS_STORE;                                               \
        mem_addr = regs[_c_rs1] + imm_c_slw;                                 \
        mem_val = regs[_c_rs2];                                              \
        mem_val_type = DATA_TYPE_WORD;                                       \
        pc += 2UL;                                                           \
    }                                                                        \
    break;

#define INSTRUCT_C_SD                                                        \
    {                                                                        \
        uint64_t imm_c_sld(IMM_C_SLD(insn));                                 \
        if (verbose)                                                         \
            sprintf(remark, "c.sd %s,%ld(%s)", regs_name[_c_rs2], imm_c_sld, \
                    regs_name[_c_rs1]);                                      \
        require_extension('C');                                              \
        mmu->store(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD, regs[_c_rs2]); \
        load_store = LS_STORE;                                               \
        mem_addr = regs[_c_rs1] + imm_c_sld;                                 \
        mem_val = regs[_c_rs2];                                              \
        mem_val_type = DATA_TYPE_DWORD;                                      \
        pc += 2UL;                                                           \
    }                                                                        \
    break;

#define INSTRUCT_C_FSD                                                         \
    {                                                                          \
        uint64_t imm_c_sld(IMM_C_SLD(insn));                                   \
        if (verbose)                                                           \
            sprintf(remark, "c.fsd %s,%ld(%s)", fregs_name[_c_rs2], imm_c_sld, \
                    regs_name[_c_rs1]);                                        \
        require_extension('C');                                                \
        require_extension('D');                                                \
        require_fp;                                                            \
        mmu->store(regs[_c_rs1] + imm_c_sld, DATA_TYPE_DWORD, fregs[_c_rs2]);  \
        load_store = LS_STORE;                                                 \
        mem_addr = regs[_c_rs1] + imm_c_sld;                                   \
        mem_val = fregs[_c_rs2];                                               \
        mem_val_type = DATA_TYPE_DWORD;                                        \
        pc += 2UL;                                                             \
    }                                                                          \
    break;

#define INSTRUCT_C_ADDI                                                 \
    {                                                                   \
        uint64_t imm_c_addi(IMM_C_ADDI(insn));                          \
        if (verbose) {                                                  \
            if (!rd && !imm_c_addi) {                                   \
                sprintf(remark, "c.nop");                               \
            } else {                                                    \
                sprintf(remark, "c.addi %s,%ld", regs_name[rd],         \
                        (int64_t) imm_c_addi);                          \
            }                                                           \
        }                                                               \
        require_extension('C');                                         \
        confirm_insn_legal((rd && imm_c_addi) || (!rd && !imm_c_addi)); \
        regs[rd] = regs[rd] + imm_c_addi;                               \
        write_reg = rd;                                                 \
        pc += 2UL;                                                      \
    }                                                                   \
    break;

#define INSTRUCT_C_ADDIW                                      \
    {                                                         \
        uint64_t imm_c_addi(IMM_C_ADDI(insn));                \
        if (verbose)                                          \
            sprintf(remark, "c.addiw %s,%ld", regs_name[rd],  \
                    (int64_t) imm_c_addi);                    \
        require_extension('C');                               \
        confirm_insn_legal(rd);                               \
        regs[rd] = (int32_t) regs[rd] + (int32_t) imm_c_addi; \
        write_reg = rd;                                       \
        pc += 2UL;                                            \
    }                                                         \
    break;

#define INSTRUCT_C_JAL                                       \
    {                                                        \
        uint64_t imm_c_j(IMM_C_J(insn));                     \
        if (verbose)                                         \
            sprintf(remark, "c.jal %08lx", pc + imm_c_j);    \
        require_extension('C');                              \
        throw TrapIllegalInstruction(insn); /* RV32C only */ \
        ras_push = 1;                                        \
        regs[REG_RA] = pc + 2UL;                             \
        write_reg = REG_RA;                                  \
        pc += imm_c_j;                                       \
    }                                                        \
    break;

#define INSTRUCT_C_LI                                     \
    {                                                     \
        uint64_t imm_c_addi(IMM_C_ADDI(insn));            \
        if (verbose)                                      \
            sprintf(remark, "c.li %s,%ld", regs_name[rd], \
                    (int64_t) imm_c_addi);                \
        require_extension('C');                           \
        confirm_insn_legal(rd);                           \
        regs[rd] = imm_c_addi;                            \
        write_reg = rd;                                   \
        pc += 2UL;                                        \
    }                                                     \
    break;

#define INSTRUCT_C_ADDI16SP                                         \
    {                                                               \
        uint64_t imm_c_addi16sp(IMM_C_ADDI16SP(insn));              \
        if (verbose)                                                \
            sprintf(remark, "c.addi16sp %s,%ld", regs_name[REG_SP], \
                    (int64_t) imm_c_addi16sp);                      \
        require_extension('C');                                     \
        confirm_insn_legal(imm_c_addi16sp &&rd == REG_SP);          \
        regs[REG_SP] = regs[REG_SP] + imm_c_addi16sp;               \
        write_reg = REG_SP;                                         \
        pc += 2UL;                                                  \
    }                                                               \
    break;

#define INSTRUCT_C_LUI                                       \
    {                                                        \
        uint64_t imm_c_lui(IMM_C_LUI(insn));                 \
        if (verbose)                                         \
            sprintf(remark, "c.lui %s,0x%lx", regs_name[rd], \
                    imm_c_lui >> 12 & 0xfffff);              \
        require_extension('C');                              \
        confirm_insn_legal(rd &&imm_c_lui &&rd != REG_SP);   \
        regs[rd] = imm_c_lui;                                \
        write_reg = rd;                                      \
        pc += 2UL;                                           \
    }                                                        \
    break;

#define INSTRUCT_C_SUB                                                        \
    if (verbose)                                                              \
        sprintf(remark, "c.sub %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                   \
    regs[_c_rs1] = regs[_c_rs1] - regs[_c_rs2];                               \
    write_reg = _c_rs1;                                                       \
    pc += 2UL;                                                                \
    break;

#define INSTRUCT_C_XOR                                                        \
    if (verbose)                                                              \
        sprintf(remark, "c.xor %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                   \
    regs[_c_rs1] = regs[_c_rs1] ^ regs[_c_rs2];                               \
    write_reg = _c_rs1;                                                       \
    pc += 2UL;                                                                \
    break;

#define INSTRUCT_C_OR                                                        \
    if (verbose)                                                             \
        sprintf(remark, "c.or %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                  \
    regs[_c_rs1] = regs[_c_rs1] | regs[_c_rs2];                              \
    write_reg = _c_rs1;                                                      \
    pc += 2UL;                                                               \
    break;

#define INSTRUCT_C_AND                                                        \
    if (verbose)                                                              \
        sprintf(remark, "c.and %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                   \
    regs[_c_rs1] = regs[_c_rs1] & regs[_c_rs2];                               \
    write_reg = _c_rs1;                                                       \
    pc += 2UL;                                                                \
    break;

#define INSTRUCT_C_SUBW                                                        \
    if (verbose)                                                               \
        sprintf(remark, "c.subw %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                    \
    regs[_c_rs1] = (int32_t) regs[_c_rs1] - (int32_t) regs[_c_rs2];            \
    write_reg = _c_rs1;                                                        \
    pc += 2UL;                                                                 \
    break;

#define INSTRUCT_C_ADDW                                                        \
    if (verbose)                                                               \
        sprintf(remark, "c.addw %s,%s", regs_name[_c_rs1], regs_name[_c_rs2]); \
    require_extension('C');                                                    \
    regs[_c_rs1] = (int32_t) regs[_c_rs1] + (int32_t) regs[_c_rs2];            \
    write_reg = _c_rs1;                                                        \
    pc += 2UL;                                                                 \
    break;

#define INSTRUCT_C_SRLI                                                \
    if (verbose)                                                       \
        sprintf(remark, "c.srli %s,0x%x", regs_name[_c_rs1], c_shamt); \
    require_extension('C');                                            \
    confirm_insn_legal(c_shamt);                                       \
    regs[_c_rs1] = regs[_c_rs1] >> c_shamt;                            \
    write_reg = _c_rs1;                                                \
    pc += 2UL;                                                         \
    break;

#define INSTRUCT_C_SRAI                                                \
    if (verbose)                                                       \
        sprintf(remark, "c.srai %s,0x%x", regs_name[_c_rs1], c_shamt); \
    require_extension('C');                                            \
    confirm_insn_legal(c_shamt);                                       \
    regs[_c_rs1] = (int64_t) regs[_c_rs1] >> c_shamt;                  \
    write_reg = _c_rs1;                                                \
    pc += 2UL;                                                         \
    break;

#define INSTRUCT_C_ANDI                                         \
    {                                                           \
        uint64_t imm_c_addi(IMM_C_ADDI(insn));                  \
        if (verbose)                                            \
            sprintf(remark, "c.andi %s,%ld", regs_name[_c_rs1], \
                    (int64_t) imm_c_addi);                      \
        require_extension('C');                                 \
        regs[_c_rs1] = regs[_c_rs1] & imm_c_addi;               \
        write_reg = _c_rs1;                                     \
        pc += 2UL;                                              \
    }                                                           \
    break;

#define INSTRUCT_C_J                                    \
    {                                                   \
        uint64_t imm_c_j(IMM_C_J(insn));                \
        if (verbose)                                    \
            sprintf(remark, "c.j %08lx", pc + imm_c_j); \
        require_extension('C');                         \
        pc += imm_c_j;                                  \
    }                                                   \
    break;

#define INSTRUCT_C_BEQZ                                           \
    {                                                             \
        uint64_t imm_c_b(IMM_C_B(insn));                          \
        if (verbose)                                              \
            sprintf(remark, "c.beqz %s,%08lx", regs_name[_c_rs1], \
                    pc + imm_c_b);                                \
        require_extension('C');                                   \
        pc += !regs[_c_rs1] ? imm_c_b : 2UL;                      \
    }                                                             \
    break;

#define INSTRUCT_C_BNEZ                                           \
    {                                                             \
        uint64_t imm_c_b(IMM_C_B(insn));                          \
        if (verbose)                                              \
            sprintf(remark, "c.bnez %s,%08lx", regs_name[_c_rs1], \
                    pc + imm_c_b);                                \
        require_extension('C');                                   \
        pc += regs[_c_rs1] ? imm_c_b : 2UL;                       \
    }                                                             \
    break;

#define INSTRUCT_C_SLLI                                            \
    if (verbose)                                                   \
        sprintf(remark, "c.slli %s,0x%x", regs_name[rd], c_shamt); \
    require_extension('C');                                        \
    confirm_insn_legal(rd &&c_shamt);                              \
    regs[rd] = regs[rd] << c_shamt;                                \
    write_reg = rd;                                                \
    pc += 2UL;                                                     \
    break;

#define INSTRUCT_C_LWSP                                                     \
    {                                                                       \
        uint64_t imm_c_lwsp(IMM_C_LWSP(insn));                              \
        if (verbose)                                                        \
            sprintf(remark, "c.lwsp %s,%ld(%s)", regs_name[rd], imm_c_lwsp, \
                    regs_name[REG_SP]);                                     \
        require_extension('C');                                             \
        confirm_insn_legal(rd);                                             \
        mem_addr = regs[REG_SP] + imm_c_lwsp;                               \
        regs[rd] = mmu->load(regs[REG_SP] + imm_c_lwsp, DATA_TYPE_WORD);    \
        load_store = LS_LOAD;                                               \
        mem_val = regs[rd];                                                 \
        mem_val_type = DATA_TYPE_WORD;                                      \
        write_reg = rd;                                                     \
        pc += 2UL;                                                          \
    }                                                                       \
    break;

#define INSTRUCT_C_LDSP                                                     \
    {                                                                       \
        uint64_t imm_c_ldsp(IMM_C_LDSP(insn));                              \
        if (verbose)                                                        \
            sprintf(remark, "c.ldsp %s,%ld(%s)", regs_name[rd], imm_c_ldsp, \
                    regs_name[REG_SP]);                                     \
        require_extension('C');                                             \
        confirm_insn_legal(rd);                                             \
        mem_addr = regs[REG_SP] + imm_c_ldsp;                               \
        regs[rd] = mmu->load(regs[REG_SP] + imm_c_ldsp, DATA_TYPE_DWORD);   \
        load_store = LS_LOAD;                                               \
        mem_val = regs[rd];                                                 \
        mem_val_type = DATA_TYPE_DWORD;                                     \
        write_reg = rd;                                                     \
        pc += 2UL;                                                          \
    }                                                                       \
    break;

#define INSTRUCT_C_FLDSP                                                      \
    {                                                                         \
        uint64_t imm_c_ldsp(IMM_C_LDSP(insn));                                \
        if (verbose)                                                          \
            sprintf(remark, "c.fldsp %s,%ld(%s)", fregs_name[rd], imm_c_ldsp, \
                    regs_name[REG_SP]);                                       \
        require_extension('C');                                               \
        require_extension('D');                                               \
        require_fp;                                                           \
        mem_addr = regs[REG_SP] + imm_c_ldsp;                                 \
        fregs[rd] = mmu->load(regs[REG_SP] + imm_c_ldsp, DATA_TYPE_DWORD);    \
        set_sd_fs_dirty;                                                      \
        load_store = LS_LOAD;                                                 \
        mem_val = fregs[rd];                                                  \
        mem_val_type = DATA_TYPE_DWORD;                                       \
        write_reg = rd | 0x20;                                                \
        pc += 2UL;                                                            \
    }                                                                         \
    break;

#define INSTRUCT_C_JR                                     \
    {                                                     \
        if (verbose)                                      \
            sprintf(remark, "c.jr %s", regs_name[c_rs1]); \
        require_extension('C');                           \
        confirm_insn_legal(c_rs1);                        \
        ras_pop = c_rs1 == REG_RA;                        \
        pc = (regs[c_rs1]) & ~1UL;                        \
    }                                                     \
    break;

#define INSTRUCT_C_MV                                                   \
    if (verbose)                                                        \
        sprintf(remark, "c.mv %s,%s", regs_name[rd], regs_name[c_rs2]); \
    require_extension('C');                                             \
    confirm_insn_legal(rd &&c_rs2);                                     \
    regs[rd] = regs[c_rs2];                                             \
    write_reg = rd;                                                     \
    pc += 2UL;                                                          \
    break;

#define INSTRUCT_C_EBREAK            \
    if (verbose)                     \
        sprintf(remark, "c.ebreak"); \
    require_extension('C');          \
    throw TrapBreakpoint(pc);        \
    break;

#define INSTRUCT_C_JALR                                     \
    {                                                       \
        if (verbose)                                        \
            sprintf(remark, "c.jalr %s", regs_name[c_rs1]); \
        require_extension('C');                             \
        confirm_insn_legal(c_rs1);                          \
        uint64_t rs1_data(regs[c_rs1]);                     \
        ras_push = 1;                                       \
        regs[REG_RA] = pc + 2UL;                            \
        write_reg = REG_RA;                                 \
        pc = (rs1_data) & ~1UL;                             \
    }                                                       \
    break;

#define INSTRUCT_C_ADD                                                   \
    if (verbose)                                                         \
        sprintf(remark, "c.add %s,%s", regs_name[rd], regs_name[c_rs2]); \
    require_extension('C');                                              \
    confirm_insn_legal(rd &&c_rs2);                                      \
    regs[rd] = regs[rd] + regs[c_rs2];                                   \
    write_reg = rd;                                                      \
    pc += 2UL;                                                           \
    break;

#define INSTRUCT_C_SWSP                                                        \
    {                                                                          \
        uint64_t imm_c_swsp(IMM_C_SWSP(insn));                                 \
        if (verbose)                                                           \
            sprintf(remark, "c.swsp %s,%ld(%s)", regs_name[c_rs2], imm_c_swsp, \
                    regs_name[REG_SP]);                                        \
        require_extension('C');                                                \
        mmu->store(regs[REG_SP] + imm_c_swsp, DATA_TYPE_WORD, regs[c_rs2]);    \
        load_store = LS_STORE;                                                 \
        mem_addr = regs[REG_SP] + imm_c_swsp;                                  \
        mem_val = regs[c_rs2];                                                 \
        mem_val_type = DATA_TYPE_WORD;                                         \
        pc += 2UL;                                                             \
    }                                                                          \
    break;

#define INSTRUCT_C_SDSP                                                        \
    {                                                                          \
        uint64_t imm_c_sdsp(IMM_C_SDSP(insn));                                 \
        if (verbose)                                                           \
            sprintf(remark, "c.sdsp %s,%ld(%s)", regs_name[c_rs2], imm_c_sdsp, \
                    regs_name[REG_SP]);                                        \
        require_extension('C');                                                \
        mmu->store(regs[REG_SP] + imm_c_sdsp, DATA_TYPE_DWORD, regs[c_rs2]);   \
        load_store = LS_STORE;                                                 \
        mem_addr = regs[REG_SP] + imm_c_sdsp;                                  \
        mem_val = regs[c_rs2];                                                 \
        mem_val_type = DATA_TYPE_DWORD;                                        \
        pc += 2UL;                                                             \
    }                                                                          \
    break;

#define INSTRUCT_C_FSDSP                                                      \
    {                                                                         \
        uint64_t imm_c_sdsp(IMM_C_SDSP(insn));                                \
        if (verbose)                                                          \
            sprintf(remark, "c.fsdsp %s,%ld(%s)", fregs_name[c_rs2],          \
                    imm_c_sdsp, regs_name[REG_SP]);                           \
        require_extension('C');                                               \
        require_extension('D');                                               \
        require_fp;                                                           \
        mmu->store(regs[REG_SP] + imm_c_sdsp, DATA_TYPE_DWORD, fregs[c_rs2]); \
        load_store = LS_STORE;                                                \
        mem_addr = regs[REG_SP] + imm_c_sdsp;                                 \
        mem_val = fregs[c_rs2];                                               \
        mem_val_type = DATA_TYPE_DWORD;                                       \
        pc += 2UL;                                                            \
    }                                                                         \
    break;

#define STR_AQ_RL(_aq, _rl) \
    (_aq) && (_rl) ? ".aq.rl" : (_aq) ? ".aq" : (_rl) ? ".rl" : ""

#define INSTRUCT_LR_W                                                        \
    if (verbose)                                                             \
        sprintf(remark, "lr.w%s %s, (%s)", STR_AQ_RL(aq, rl), regs_name[rd], \
                regs_name[rs1]);                                             \
    require_extension('A');                                                  \
    if (regs[rs1] & 3UL)                                                     \
        throw TrapLoadAccessFault(regs[rs1]);                                \
    mmu->acquire_load_reservation(regs[rs1]);                                \
    mem_addr = regs[rs1];                                                    \
    regs[rd] = mmu->load(regs[rs1], DATA_TYPE_WORD);                         \
    load_store = LS_LOAD;                                                    \
    mem_val = regs[rd];                                                      \
    mem_val_type = DATA_TYPE_WORD;                                           \
    write_reg = rd;                                                          \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_SC_W                                                 \
    {                                                                 \
        if (verbose)                                                  \
            sprintf(remark, "sc.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                    regs_name[rd], regs_name[rs2], regs_name[rs1]);   \
        require_extension('A');                                       \
        if (regs[rs1] & 3UL)                                          \
            throw TrapLoadAccessFault(regs[rs1]);                     \
        bool is_reserv(mmu->check_load_reservation(regs[rs1]));       \
        if (is_reserv) {                                              \
            mmu->store(regs[rs1], DATA_TYPE_WORD, regs[rs2]);         \
            load_store = LS_STORE;                                    \
            mem_addr = regs[rs1];                                     \
            mem_val = regs[rs2];                                      \
            mem_val_type = DATA_TYPE_WORD;                            \
        }                                                             \
        regs[rd] = !is_reserv;                                        \
        write_reg = rd;                                               \
        mmu->release_load_reservation();                              \
        pc += 4UL;                                                    \
    }                                                                 \
    break;

#define INSTRUCT_AMOSWAP_W                                             \
    if (verbose)                                                       \
        sprintf(remark, "amoswap.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2],  \
                                &amo_amoswap_w_func);                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOADD_W                                             \
    if (verbose)                                                      \
        sprintf(remark, "amoadd.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);       \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoadd_w_func);                  \
    write_reg = rd;                                                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOXOR_W                                             \
    if (verbose)                                                      \
        sprintf(remark, "amoxor.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);       \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoxor_w_func);                  \
    write_reg = rd;                                                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOAND_W                                             \
    if (verbose)                                                      \
        sprintf(remark, "amoand.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);       \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoand_w_func);                  \
    write_reg = rd;                                                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOOR_W                                              \
    if (verbose)                                                      \
        sprintf(remark, "amoor.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl),  \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);       \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amoor_w_func);                   \
    write_reg = rd;                                                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOMIN_W                                             \
    if (verbose)                                                      \
        sprintf(remark, "amomin.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);       \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amomin_w_func);                  \
    write_reg = rd;                                                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOMAX_W                                             \
    if (verbose)                                                      \
        sprintf(remark, "amomax.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);       \
    require_extension('A');                                           \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2], \
                                &amo_amomax_w_func);                  \
    write_reg = rd;                                                   \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_AMOMINU_W                                             \
    if (verbose)                                                       \
        sprintf(remark, "amominu.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2],  \
                                &amo_amominu_w_func);                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMAXU_W                                             \
    if (verbose)                                                       \
        sprintf(remark, "amoswap.w%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_WORD, regs[rs2],  \
                                &amo_amomaxu_w_func);                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_LR_D                                                        \
    if (verbose)                                                             \
        sprintf(remark, "lr.d%s %s, (%s)", STR_AQ_RL(aq, rl), regs_name[rd], \
                regs_name[rs1]);                                             \
    require_extension('A');                                                  \
    if (regs[rs1] & 7UL)                                                     \
        throw TrapLoadAccessFault(regs[rs1]);                                \
    mmu->acquire_load_reservation(regs[rs1]);                                \
    mem_addr = regs[rs1];                                                    \
    regs[rd] = mmu->load(regs[rs1], DATA_TYPE_DWORD);                        \
    load_store = LS_LOAD;                                                    \
    mem_val = regs[rd];                                                      \
    mem_val_type = DATA_TYPE_DWORD;                                          \
    write_reg = rd;                                                          \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_SC_D                                                 \
    {                                                                 \
        if (verbose)                                                  \
            sprintf(remark, "sc.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                    regs_name[rd], regs_name[rs2], regs_name[rs1]);   \
        require_extension('A');                                       \
        if (regs[rs1] & 7UL)                                          \
            throw TrapLoadAccessFault(regs[rs1]);                     \
        bool is_reserv(mmu->check_load_reservation(regs[rs1]));       \
        if (is_reserv) {                                              \
            mmu->store(regs[rs1], DATA_TYPE_DWORD, regs[rs2]);        \
            load_store = LS_STORE;                                    \
            mem_addr = regs[rs1];                                     \
            mem_val = regs[rs2];                                      \
            mem_val_type = DATA_TYPE_DWORD;                           \
        }                                                             \
        regs[rd] = !is_reserv;                                        \
        mmu->release_load_reservation();                              \
        write_reg = rd;                                               \
        pc += 4UL;                                                    \
    }                                                                 \
    break;

#define INSTRUCT_AMOSWAP_D                                             \
    if (verbose)                                                       \
        sprintf(remark, "amoswap.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoswap_d_func);                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOADD_D                                              \
    if (verbose)                                                       \
        sprintf(remark, "amoadd.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),  \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoadd_d_func);                   \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOXOR_D                                              \
    if (verbose)                                                       \
        sprintf(remark, "amoxor.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),  \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoxor_d_func);                   \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOAND_D                                              \
    if (verbose)                                                       \
        sprintf(remark, "amoand.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),  \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoand_d_func);                   \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOOR_D                                               \
    if (verbose)                                                       \
        sprintf(remark, "amoor.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),   \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amoor_d_func);                    \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMIN_D                                              \
    if (verbose)                                                       \
        sprintf(remark, "amomin.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),  \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amomin_d_func);                   \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMAX_D                                              \
    if (verbose)                                                       \
        sprintf(remark, "amomax.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl),  \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amomax_d_func);                   \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMINU_D                                             \
    if (verbose)                                                       \
        sprintf(remark, "amominu.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amominu_d_func);                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_AMOMAXU_D                                             \
    if (verbose)                                                       \
        sprintf(remark, "amomaxu.d%s %s, %s, (%s)", STR_AQ_RL(aq, rl), \
                regs_name[rd], regs_name[rs2], regs_name[rs1]);        \
    require_extension('A');                                            \
    regs[rd] = mmu->amo_operate(regs[rs1], DATA_TYPE_DWORD, regs[rs2], \
                                &amo_amomaxu_d_func);                  \
    write_reg = rd;                                                    \
    pc += 4UL;                                                         \
    break;

#define INSTRUCT_FLW                                                       \
    if (verbose)                                                           \
        sprintf(remark, "flw %s,%ld(%s)", fregs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                           \
    require_extension('F');                                                \
    require_fp;                                                            \
    fregs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD);              \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                       \
    set_sd_fs_dirty;                                                       \
    load_store = LS_LOAD;                                                  \
    mem_addr = regs[rs1] + imm_i;                                          \
    mem_val = fregs[rd];                                                   \
    mem_val_type = DATA_TYPE_WORD;                                         \
    write_reg = rd | 0x20;                                                 \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FSW                                                        \
    if (verbose)                                                            \
        sprintf(remark, "fsw %s,%ld(%s)", fregs_name[rs2], (int64_t) imm_s, \
                regs_name[rs1]);                                            \
    require_extension('F');                                                 \
    require_fp;                                                             \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_WORD, fregs[rs2]);              \
    load_store = LS_STORE;                                                  \
    mem_addr = regs[rs1] + imm_s;                                           \
    mem_val = fregs[rs2];                                                   \
    mem_val_type = DATA_TYPE_WORD;                                          \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMADD_S                                              \
    if (verbose)                                                      \
        sprintf(remark, "fmadd.s %s,%s,%s,%s", fregs_name[rd],        \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]);   \
    require_extension('F');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    fregs[rd] = f32_add(f32_mul(fregs[rs1], fregs[rs2]), fregs[rs3]); \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                  \
    set_sd_fs_dirty;                                                  \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    write_csr = CSR_FFLAGS_ADDR;                                      \
    write_reg = rd | 0x20;                                            \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FMSUB_S                                                       \
    if (verbose)                                                               \
        sprintf(remark, "fmsub.s %s,%s,%s,%s", fregs_name[rd],                 \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]);            \
    require_extension('F');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f32_add(f32_mul(fregs[rs1], fregs[rs2]), F32_NEG(fregs[rs3])); \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                           \
    set_sd_fs_dirty;                                                           \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    write_reg = rd | 0x20;                                                     \
    write_csr = CSR_FFLAGS_ADDR;                                               \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMSUB_S                                                      \
    if (verbose)                                                               \
        sprintf(remark, "fnmsub.s %s,%s,%s,%s", fregs_name[rd],                \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]);            \
    require_extension('F');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f32_add(F32_NEG(f32_mul(fregs[rs1], fregs[rs2])), fregs[rs3]); \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                           \
    set_sd_fs_dirty;                                                           \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    write_reg = rd | 0x20;                                                     \
    write_csr = CSR_FFLAGS_ADDR;                                               \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMADD_S                                           \
    if (verbose)                                                    \
        sprintf(remark, "fnmadd.s %s,%s,%s,%s", fregs_name[rd],     \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]); \
    require_extension('F');                                         \
    require_fp;                                                     \
    clean_fflags_value();                                           \
    fregs[rd] = f32_add(F32_NEG(f32_mul(fregs[rs1], fregs[rs2])),   \
                        F32_NEG(fregs[rs3]));                       \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                \
    set_sd_fs_dirty;                                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());              \
    write_reg = rd | 0x20;                                          \
    write_csr = CSR_FFLAGS_ADDR;                                    \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_FADD_S                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fadd.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_add(fregs[rs1], fregs[rs2]);                            \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FSUB_S                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fsub.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_add(fregs[rs1], F32_NEG(fregs[rs2]));                   \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMUL_S                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fmul.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_mul(fregs[rs1], fregs[rs2]);                            \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FDIV_S                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fdiv.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_div(fregs[rs1], fregs[rs2]);                            \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FSQRT_S                                                   \
    if (verbose)                                                           \
        sprintf(remark, "fsqrt.s %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    fregs[rd] = f32_sqrt(fregs[rs1]);                                      \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                       \
    set_sd_fs_dirty;                                                       \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd | 0x20;                                                 \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FSGNJ_S                                                     \
    if (verbose)                                                             \
        sprintf(remark, "fsgnj.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                            \
    require_extension('F');                                                  \
    require_fp;                                                              \
    if (!F32_IS_VALID(fregs[rs1]))                                           \
        fregs[rd] = !F32_IS_VALID(fregs[rs2])                                \
                        ? F32_DEFAULT_QUIET_NAN                              \
                        : F32_NEG(F32_DEFAULT_QUIET_NAN);                    \
    else if (!F32_IS_VALID(fregs[rs2]))                                      \
        fregs[rd] = fregs[rs1];                                              \
    else                                                                     \
        fregs[rd] = (fregs[rs2] & (1U << 31)) | (fregs[rs1] & (-1U >> 1));   \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                         \
    set_sd_fs_dirty;                                                         \
    write_reg = rd | 0x20;                                                   \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_FSGNJN_S                                                     \
    if (verbose)                                                              \
        sprintf(remark, "fsgnjn.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                             \
    require_extension('F');                                                   \
    require_fp;                                                               \
    if (!F32_IS_VALID(fregs[rs1]))                                            \
        fregs[rd] = !F32_IS_VALID(fregs[rs2])                                 \
                        ? F32_DEFAULT_QUIET_NAN                               \
                        : F32_NEG(F32_DEFAULT_QUIET_NAN);                     \
    else if (!F32_IS_VALID(fregs[rs2]))                                       \
        fregs[rd] = fregs[rs1];                                               \
    else                                                                      \
        fregs[rd] = (~fregs[rs2] & (1U << 31)) | (fregs[rs1] & (-1U >> 1));   \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                          \
    set_sd_fs_dirty;                                                          \
    write_reg = rd | 0x20;                                                    \
    pc += 4UL;                                                                \
    break;

#define INSTRUCT_FSGNJX_S                                                     \
    if (verbose)                                                              \
        sprintf(remark, "fsgnjx.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                             \
    require_extension('F');                                                   \
    require_fp;                                                               \
    if (!F32_IS_VALID(fregs[rs1]))                                            \
        fregs[rd] = !F32_IS_VALID(fregs[rs2])                                 \
                        ? F32_DEFAULT_QUIET_NAN                               \
                        : F32_NEG(F32_DEFAULT_QUIET_NAN);                     \
    else if (!F32_IS_VALID(fregs[rs2]))                                       \
        fregs[rd] = fregs[rs1];                                               \
    else                                                                      \
        fregs[rd] = (fregs[rs2] & (1U << 31)) ^ fregs[rs1];                   \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                          \
    set_sd_fs_dirty;                                                          \
    write_reg = rd | 0x20;                                                    \
    pc += 4UL;                                                                \
    break;

#define INSTRUCT_FMIN_S                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fmin.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_min(fregs[rs1], fregs[rs2]);                            \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMAX_S                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fmax.s %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_max(fregs[rs1], fregs[rs2]);                            \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FCVT_W_S                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.w.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    regs[rd] = f32_cvt_w_s(fregs[rs1]);                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd;                                                        \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_WU_S                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.wu.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    regs[rd] = (int32_t) f32_cvt_wu_s(fregs[rs1]);                          \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd;                                                         \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMV_X_W                                                  \
    if (verbose)                                                          \
        sprintf(remark, "fmv.x.w %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                               \
    require_fp;                                                           \
    regs[rd] = (int32_t) fregs[rs1];                                      \
    write_reg = rd;                                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FEQ_S                                                    \
    if (verbose)                                                          \
        sprintf(remark, "feq.s %s,%s,%s", regs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                         \
    require_extension('F');                                               \
    require_fp;                                                           \
    clean_fflags_value();                                                 \
    regs[rd] = f32_eq(fregs[rs1], fregs[rs2]);                            \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                    \
    write_reg = rd;                                                       \
    write_csr = CSR_FFLAGS_ADDR;                                          \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FLT_S                                                    \
    if (verbose)                                                          \
        sprintf(remark, "flt.s %s,%s,%s", regs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                         \
    require_extension('F');                                               \
    require_fp;                                                           \
    clean_fflags_value();                                                 \
    regs[rd] = f32_lt(fregs[rs1], fregs[rs2]);                            \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                    \
    write_reg = rd;                                                       \
    write_csr = CSR_FFLAGS_ADDR;                                          \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FLE_S                                                    \
    if (verbose)                                                          \
        sprintf(remark, "fle.s %s,%s,%s", regs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                         \
    require_extension('F');                                               \
    require_fp;                                                           \
    clean_fflags_value();                                                 \
    regs[rd] = f32_le(fregs[rs1], fregs[rs2]);                            \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                    \
    write_reg = rd;                                                       \
    write_csr = CSR_FFLAGS_ADDR;                                          \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FCLASS_S                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fclass.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                                \
    require_fp;                                                            \
    regs[rd] = f32_classify(fregs[rs1]);                                   \
    write_reg = rd;                                                        \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_S_W                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.s.w %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    fregs[rd] = f32_cvt_s_w(regs[rs1]);                                    \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                       \
    set_sd_fs_dirty;                                                       \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd | 0x20;                                                 \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_S_WU                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.s.wu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_cvt_s_wu(regs[rs1]);                                    \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMV_W_X                                                  \
    if (verbose)                                                          \
        sprintf(remark, "fmv.w.x %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                               \
    require_fp;                                                           \
    fregs[rd] = (uint32_t) regs[rs1];                                     \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                      \
    set_sd_fs_dirty;                                                      \
    write_reg = rd | 0x20;                                                \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FCVT_L_S                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.l.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    regs[rd] = f32_cvt_l_s(fregs[rs1]);                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd;                                                        \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_LU_S                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.lu.s %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    regs[rd] = f32_cvt_lu_s(fregs[rs1]);                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd;                                                         \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FCVT_S_L                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.s.l %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    fregs[rd] = f32_cvt_s_l(regs[rs1]);                                    \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                       \
    set_sd_fs_dirty;                                                       \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd | 0x20;                                                 \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_S_LU                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.s.lu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('F');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f32_cvt_s_lu(regs[rs1]);                                    \
    fregs[rd] |= FREG_FILL_MSB_WORD;                                        \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FLD                                                       \
    if (verbose)                                                           \
        sprintf(remark, "fld %s,%ld(%s)", fregs_name[rd], (int64_t) imm_i, \
                regs_name[rs1]);                                           \
    require_extension('D');                                                \
    require_fp;                                                            \
    fregs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_DWORD);             \
    set_sd_fs_dirty;                                                       \
    load_store = LS_LOAD;                                                  \
    mem_addr = regs[rs1] + imm_i;                                          \
    mem_val = fregs[rd];                                                   \
    mem_val_type = DATA_TYPE_DWORD;                                        \
    write_reg = rd | 0x20;                                                 \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FSD                                                        \
    if (verbose)                                                            \
        sprintf(remark, "fsd %s,%ld(%s)", fregs_name[rs2], (int64_t) imm_s, \
                regs_name[rs1]);                                            \
    require_extension('D');                                                 \
    require_fp;                                                             \
    mmu->store(regs[rs1] + imm_s, DATA_TYPE_DWORD, fregs[rs2]);             \
    load_store = LS_STORE;                                                  \
    mem_addr = regs[rs1] + imm_s;                                           \
    mem_val = fregs[rs2];                                                   \
    mem_val_type = DATA_TYPE_DWORD;                                         \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMADD_D                                              \
    if (verbose)                                                      \
        sprintf(remark, "fmadd.d %s,%s,%s,%s", fregs_name[rd],        \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]);   \
    require_extension('D');                                           \
    require_fp;                                                       \
    clean_fflags_value();                                             \
    fregs[rd] = f64_add(f64_mul(fregs[rs1], fregs[rs2]), fregs[rs3]); \
    set_sd_fs_dirty;                                                  \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                \
    write_reg = rd | 0x20;                                            \
    write_csr = CSR_FFLAGS_ADDR;                                      \
    pc += 4UL;                                                        \
    break;

#define INSTRUCT_FMSUB_D                                                       \
    if (verbose)                                                               \
        sprintf(remark, "fmsub.d %s,%s,%s,%s", fregs_name[rd],                 \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]);            \
    require_extension('D');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f64_add(f64_mul(fregs[rs1], fregs[rs2]), F64_NEG(fregs[rs3])); \
    set_sd_fs_dirty;                                                           \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    write_reg = rd | 0x20;                                                     \
    write_csr = CSR_FFLAGS_ADDR;                                               \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMSUB_D                                                      \
    if (verbose)                                                               \
        sprintf(remark, "fnmsub.d %s,%s,%s,%s", fregs_name[rd],                \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]);            \
    require_extension('D');                                                    \
    require_fp;                                                                \
    clean_fflags_value();                                                      \
    fregs[rd] = f64_add(F64_NEG(f64_mul(fregs[rs1], fregs[rs2])), fregs[rs3]); \
    set_sd_fs_dirty;                                                           \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                         \
    write_reg = rd | 0x20;                                                     \
    write_csr = CSR_FFLAGS_ADDR;                                               \
    pc += 4UL;                                                                 \
    break;

#define INSTRUCT_FNMADD_D                                           \
    if (verbose)                                                    \
        sprintf(remark, "fnmadd.d %s,%s,%s,%s", fregs_name[rd],     \
                fregs_name[rs1], fregs_name[rs2], fregs_name[rs3]); \
    require_extension('D');                                         \
    require_fp;                                                     \
    clean_fflags_value();                                           \
    fregs[rd] = f64_add(F64_NEG(f64_mul(fregs[rs1], fregs[rs2])),   \
                        F64_NEG(fregs[rs3]));                       \
    set_sd_fs_dirty;                                                \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());              \
    write_reg = rd | 0x20;                                          \
    write_csr = CSR_FFLAGS_ADDR;                                    \
    pc += 4UL;                                                      \
    break;

#define INSTRUCT_FADD_D                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fadd.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_add(fregs[rs1], fregs[rs2]);                            \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FSUB_D                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fsub.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_add(fregs[rs1], F64_NEG(fregs[rs2]));                   \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMUL_D                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fmul.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_mul(fregs[rs1], fregs[rs2]);                            \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FDIV_D                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fdiv.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_div(fregs[rs1], fregs[rs2]);                            \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FSQRT_D                                                   \
    if (verbose)                                                           \
        sprintf(remark, "fsqrt.d %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    fregs[rd] = f64_sqrt(fregs[rs1]);                                      \
    set_sd_fs_dirty;                                                       \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd | 0x20;                                                 \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FSGNJ_D                                                     \
    if (verbose)                                                             \
        sprintf(remark, "fsgnj.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                            \
    require_extension('D');                                                  \
    require_fp;                                                              \
    fregs[rd] = (fregs[rs2] & (1UL << 63)) | (fregs[rs1] & (-1UL >> 1));     \
    set_sd_fs_dirty;                                                         \
    write_reg = rd | 0x20;                                                   \
    pc += 4UL;                                                               \
    break;

#define INSTRUCT_FSGNJN_D                                                     \
    if (verbose)                                                              \
        sprintf(remark, "fsgnjn.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                             \
    require_extension('D');                                                   \
    require_fp;                                                               \
    fregs[rd] = (~fregs[rs2] & (1UL << 63)) | (fregs[rs1] & (-1UL >> 1));     \
    set_sd_fs_dirty;                                                          \
    write_reg = rd | 0x20;                                                    \
    pc += 4UL;                                                                \
    break;

#define INSTRUCT_FSGNJX_D                                                     \
    if (verbose)                                                              \
        sprintf(remark, "fsgnjx.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                             \
    require_extension('D');                                                   \
    require_fp;                                                               \
    fregs[rd] = (fregs[rs2] & (1UL << 63)) ^ fregs[rs1];                      \
    set_sd_fs_dirty;                                                          \
    write_reg = rd | 0x20;                                                    \
    pc += 4UL;                                                                \
    break;

#define INSTRUCT_FMIN_D                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fmin.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_min(fregs[rs1], fregs[rs2]);                            \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMAX_D                                                     \
    if (verbose)                                                            \
        sprintf(remark, "fmax.d %s,%s,%s", fregs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                           \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_max(fregs[rs1], fregs[rs2]);                            \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FCVT_S_D                                                   \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.s.d %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_cvt_s_d(fregs[rs1]);                                    \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FCVT_D_S                                                   \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.d.s %s,%s", fregs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_cvt_d_s(fregs[rs1]);                                    \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FEQ_D                                                    \
    if (verbose)                                                          \
        sprintf(remark, "feq.d %s,%s,%s", regs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                         \
    require_extension('D');                                               \
    require_fp;                                                           \
    clean_fflags_value();                                                 \
    regs[rd] = f64_eq(fregs[rs1], fregs[rs2]);                            \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                    \
    write_reg = rd;                                                       \
    write_csr = CSR_FFLAGS_ADDR;                                          \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FLT_D                                                    \
    if (verbose)                                                          \
        sprintf(remark, "flt.d %s,%s,%s", regs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                         \
    require_extension('D');                                               \
    require_fp;                                                           \
    clean_fflags_value();                                                 \
    regs[rd] = f64_lt(fregs[rs1], fregs[rs2]);                            \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                    \
    write_reg = rd;                                                       \
    write_csr = CSR_FFLAGS_ADDR;                                          \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FLE_D                                                    \
    if (verbose)                                                          \
        sprintf(remark, "fle.d %s,%s,%s", regs_name[rd], fregs_name[rs1], \
                fregs_name[rs2]);                                         \
    require_extension('D');                                               \
    require_fp;                                                           \
    clean_fflags_value();                                                 \
    regs[rd] = f64_le(fregs[rs1], fregs[rs2]);                            \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                    \
    write_reg = rd;                                                       \
    write_csr = CSR_FFLAGS_ADDR;                                          \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FCLASS_D                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fclass.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                \
    require_fp;                                                            \
    regs[rd] = f64_classify(fregs[rs1]);                                   \
    pc += 4UL;                                                             \
    write_reg = rd;                                                        \
    break;

#define INSTRUCT_FCVT_W_D                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.w.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    regs[rd] = f64_cvt_w_d(fregs[rs1]);                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd;                                                        \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_WU_D                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.wu.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    regs[rd] = f64_cvt_wu_d(fregs[rs1]);                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd;                                                         \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FCVT_D_W                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.d.w %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    fregs[rd] = f64_cvt_d_w(regs[rs1]);                                    \
    set_sd_fs_dirty;                                                       \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd | 0x20;                                                 \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_D_WU                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.d.wu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_cvt_d_wu(regs[rs1]);                                    \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FCVT_L_D                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.l.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    regs[rd] = f64_cvt_l_d(fregs[rs1]);                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd;                                                        \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_LU_D                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.lu.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    regs[rd] = f64_cvt_lu_d(fregs[rs1]);                                    \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd;                                                         \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMV_X_D                                                  \
    if (verbose)                                                          \
        sprintf(remark, "fmv.x.d %s,%s", regs_name[rd], fregs_name[rs1]); \
    require_extension('D');                                               \
    require_fp;                                                           \
    regs[rd] = fregs[rs1];                                                \
    write_reg = rd;                                                       \
    pc += 4UL;                                                            \
    break;

#define INSTRUCT_FCVT_D_L                                                  \
    if (verbose)                                                           \
        sprintf(remark, "fcvt.d.l %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                                \
    require_fp;                                                            \
    clean_fflags_value();                                                  \
    fregs[rd] = f64_cvt_d_l(regs[rs1]);                                    \
    set_sd_fs_dirty;                                                       \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                     \
    write_reg = rd | 0x20;                                                 \
    write_csr = CSR_FFLAGS_ADDR;                                           \
    pc += 4UL;                                                             \
    break;

#define INSTRUCT_FCVT_D_LU                                                  \
    if (verbose)                                                            \
        sprintf(remark, "fcvt.d.lu %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                                 \
    require_fp;                                                             \
    clean_fflags_value();                                                   \
    fregs[rd] = f64_cvt_d_lu(regs[rs1]);                                    \
    set_sd_fs_dirty;                                                        \
    csr->set_csr(CSR_FFLAGS_ADDR, get_fflags_value());                      \
    write_reg = rd | 0x20;                                                  \
    write_csr = CSR_FFLAGS_ADDR;                                            \
    pc += 4UL;                                                              \
    break;

#define INSTRUCT_FMV_D_X                                              \
    sprintf(remark, "fmv.d.x %s,%s", fregs_name[rd], regs_name[rs1]); \
    require_extension('D');                                           \
    require_fp;                                                       \
    fregs[rd] = regs[rs1];                                            \
    set_sd_fs_dirty;                                                  \
    write_reg = rd | 0x20;                                            \
    pc += 4UL;                                                        \
    break;

#endif
