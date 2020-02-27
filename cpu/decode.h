#ifndef __DECODE__
#define __DECODE__

#include <assert.h>

// case format
#define DECLARECASE(LABEL, EXECUTE)                                            \
  case LABEL:                                                                  \
    EXECUTE
#define DECLAREDEFAULTCASE(EXECUTE)                                            \
  default:                                                                     \
    EXECUTE

// RV32I Base Instruction Set
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

#define FUNCT3_MUL 0x0
#define FUNCT3_MULH 0x1
#define FUNCT3_MULHSU 0x2
#define FUNCT3_MULHU 0x3
#define FUNCT3_DIV 0x4
#define FUNCT3_DIVU 0x5
#define FUNCT3_REM 0x6
#define FUNCT3_REMU 0x7

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

// RV32M Standard Extension
#define OPCODE_MUL 0x33
#define OPCODE_MULH 0x33
#define OPCODE_MULHSU 0x33
#define OPCODE_MULHU 0x33
#define OPCODE_DIV 0x33
#define OPCODE_DIVU 0x33
#define OPCODE_REM 0x33
#define OPCODE_REMU 0x33

// RV64M Standard Extension (in addition to RV32M)
#define OPCODE_MULW 0x3B
#define OPCODE_DIVW 0x3B
#define OPCODE_DIVUW 0x3B
#define OPCODE_REMW 0x3B
#define OPCODE_REMUW 0x3B

// RV32A Standard Extension
#define OPCODE_LR_W 0x2F
#define OPCODE_SC_W 0x2F
#define OPCODE_AMOSWAP_W 0x2F
#define OPCODE_AMOADD_W 0x2F
#define OPCODE_AMOXOR_W 0x2F
#define OPCODE_AMOAND_W 0x2F
#define OPCODE_AMOOR_W 0x2F
#define OPCODE_AMOMIN_W 0x2F
#define OPCODE_AMOMAX_W 0x2F
#define OPCODE_AMOMINU_W 0x2F
#define OPCODE_AMOMAXU_W 0x2F

// RV64A Standard Extension (in addition to RV32A)
#define OPCODE_LR_D 0x2F
#define OPCODE_SC_D 0x2F
#define OPCODE_AMOSWAP_D 0x2F
#define OPCODE_AMOADD_D 0x2F
#define OPCODE_AMOXOR_D 0x2F
#define OPCODE_AMOAND_D 0x2F
#define OPCODE_AMOOR_D 0x2F
#define OPCODE_AMOMIN_D 0x2F
#define OPCODE_AMOMAX_D 0x2F
#define OPCODE_AMOMINU_D 0x2F
#define OPCODE_AMOMAXU_D 0x2F

// RV32F Standard Extension
#define OPCODE_FLW 0x07
#define OPCODE_FSW 0x27
#define OPCODE_FMADD_S 0x43
#define OPCODE_FMSUB_S 0x47
#define OPCODE_FNMSUB_S 0x4B
#define OPCODE_FNMADD_S 0x4F
#define OPCODE_FADD_S 0x53
#define OPCODE_FSUB_S 0x53
#define OPCODE_FMUL_S 0x53
#define OPCODE_FDIV_S 0x53
#define OPCODE_FSQRT_S 0x53
#define OPCODE_FSGNJ_S 0x53
#define OPCODE_FSGNJN_S 0x53
#define OPCODE_FSGNJX_S 0x53
#define OPCODE_FMIN_S 0x53
#define OPCODE_FMAX_S 0x53
#define OPCODE_FCVT_W_S 0x53
#define OPCODE_FCVT_WU_S 0x53
#define OPCODE_FMV_X_W 0x53
#define OPCODE_FEQ_S 0x53
#define OPCODE_FLT_S 0x53
#define OPCODE_FLE_S 0x53
#define OPCODE_FCLASS_S 0x53
#define OPCODE_FCVT_S_W 0x53
#define OPCODE_FCVT_S_WU 0x53
#define OPCODE_FMV_W_X 0x53

// RV64F Standard Extension (in addition to RV32F)
#define OPCODE_FCVT_L_S 0x53
#define OPCODE_FCVT_LU_S 0x53
#define OPCODE_FCVT_S_L 0x53
#define OPCODE_FCVT_S_LU 0x53

// RV32D Standard Extension
#define OPCODE_FLD 0x07
#define OPCODE_FSD 0x27
#define OPCODE_FMADD_D 0x43
#define OPCODE_FMSUB_D 0x47
#define OPCODE_FNMSUB_D 0x4B
#define OPCODE_FNMADD_D 0x4F
#define OPCODE_FADD_D 0x53
#define OPCODE_FSUB_D 0x53
#define OPCODE_FMUL_D 0x53
#define OPCODE_FDIV_D 0x53
#define OPCODE_FSQRT_D 0x53
#define OPCODE_FSGNJ_D 0x53
#define OPCODE_FSGNJN_D 0x53
#define OPCODE_FSGNJX_D 0x53
#define OPCODE_FMIN_D 0x53
#define OPCODE_FMAX_D 0x53
#define OPCODE_FCVT_S_D 0x53
#define OPCODE_FCVT_D_S 0x53
#define OPCODE_FEQ_D 0x53
#define OPCODE_FLT_D 0x53
#define OPCODE_FLE_D 0x53
#define OPCODE_FCLASS_D 0x53
#define OPCODE_FCVT_W_D 0x53
#define OPCODE_FCVT_WU_D 0x53
#define OPCODE_FCVT_D_W 0x53
#define OPCODE_FCVT_D_WU 0x53

// RV64D Standard Extension (in addition to RV32D)
#define OPCODE_FCVT_L_D 0x53
#define OPCODE_FCVT_LU_D 0x53
#define OPCODE_FMV_X_D 0x53
#define OPCODE_FCVT_D_L 0x53
#define OPCODE_FCVT_D_LU 0x53
#define OPCODE_FMV_D_X 0x53

#define INSTRUCT_UNKNOWN                                                       \
  sprintf(remark, "unknown instruction");                                      \
  throw TrapIllegalInstruction(insn);                                              \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_LUI                                                           \
  sprintf(remark, "lui %s,0x%lx", regs_name[rd], imm_u >> 12 & 0xfffff);       \
  regs[rd] = imm_u;                                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_AUIPC                                                         \
  sprintf(remark, "auipc %s,0x%lx", regs_name[rd], imm_u >> 12 & 0xfffff);     \
  regs[rd] = imm_u + pc;                                                       \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_JAL                                                           \
  if (rd)                                                                      \
    sprintf(remark, "jal %s,%08lx", regs_name[rd], pc + imm_j & 0xffffffff);   \
  else                                                                         \
    sprintf(remark, "j %08lx", pc + imm_j & 0xffffffff);                       \
  regs[rd] = pc + 4UL;                                                         \
  pc += imm_j;                                                                 \
  break;

#define INSTRUCT_JALR                                                          \
  {                                                                            \
    if (imm_i || !(rd == 0 || rd == 1))                                        \
      sprintf(remark, "jalr %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,        \
              regs_name[rs1]);                                                 \
    else if (rd == 1)                                                          \
      sprintf(remark, "jalr %s", regs_name[rs1]);                              \
    else if (rs1 == 1)                                                         \
      sprintf(remark, "ret");                                                  \
    else                                                                       \
      sprintf(remark, "jr %s", regs_name[rs1]);                                \
    uint64_t rs1_data(regs[rs1]);                                              \
    regs[rd] = pc + 4UL;                                                       \
    pc = (imm_i + rs1_data) & ~1UL;                                            \
  }                                                                            \
  break;

#define INSTRUCT_BEQ                                                           \
  if (rs2)                                                                     \
    sprintf(remark, "beq %s,%s,%08lx", regs_name[rs1], regs_name[rs2],         \
            pc + imm_b & 0xffffffff);                                          \
  else                                                                         \
    sprintf(remark, "beqz %s,%08lx", regs_name[rs1], pc + imm_b & 0xffffffff); \
  pc += regs[rs1] == regs[rs2] ? imm_b : 4UL;                                  \
  break;

#define INSTRUCT_BNE                                                           \
  if (rs2)                                                                     \
    sprintf(remark, "bne %s,%s,%08lx", regs_name[rs1], regs_name[rs2],         \
            pc + imm_b & 0xffffffff);                                          \
  else                                                                         \
    sprintf(remark, "bnez %s,%08lx", regs_name[rs1], pc + imm_b & 0xffffffff); \
  pc += regs[rs1] != regs[rs2] ? imm_b : 4UL;                                  \
  break;

#define INSTRUCT_BLT                                                           \
  if (!rs1)                                                                    \
    sprintf(remark, "bgtz %s,%08lx", regs_name[rs2], pc + imm_b & 0xffffffff); \
  else if (!rs2)                                                               \
    sprintf(remark, "bltz %s,%08lx", regs_name[rs1], pc + imm_b & 0xffffffff); \
  else                                                                         \
    sprintf(remark, "blt %s,%s,%08lx", regs_name[rs1], regs_name[rs2],         \
            pc + imm_b & 0xffffffff);                                          \
  pc += (int64_t)regs[rs1] < (int64_t)regs[rs2] ? imm_b : 4UL;                 \
  break;

#define INSTRUCT_BGE                                                           \
  if (!rs1)                                                                    \
    sprintf(remark, "blez %s,%08lx", regs_name[rs2], pc + imm_b & 0xffffffff); \
  else if (!rs2)                                                               \
    sprintf(remark, "bgez %s,%08lx", regs_name[rs1], pc + imm_b & 0xffffffff); \
  else                                                                         \
    sprintf(remark, "bge %s,%s,%08lx", regs_name[rs1], regs_name[rs2],         \
            pc + imm_b & 0xffffffff);                                          \
  pc += (int64_t)regs[rs1] >= (int64_t)regs[rs2] ? imm_b : 4UL;                \
  break;

#define INSTRUCT_BLTU                                                          \
  sprintf(remark, "bltu %s,%s,%08lx", regs_name[rs1], regs_name[rs2],          \
          pc + imm_b & 0xffffffff);                                            \
  pc += regs[rs1] < regs[rs2] ? imm_b : 4UL;                                   \
  break;

#define INSTRUCT_BGEU                                                          \
  sprintf(remark, "bgeu %s,%s,%08lx", regs_name[rs1], regs_name[rs2],          \
          pc + imm_b & 0xffffffff);                                            \
  pc += regs[rs1] >= regs[rs2] ? imm_b : 4UL;                                  \
  break;

#define INSTRUCT_LB                                                            \
  sprintf(remark, "lb %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,              \
          regs_name[rs1]);                                                     \
  regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_BYTE);                     \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_LH                                                            \
  sprintf(remark, "lh %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,              \
          regs_name[rs1]);                                                     \
  regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_HWORD);                    \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_LW                                                            \
  sprintf(remark, "lw %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,              \
          regs_name[rs1]);                                                     \
  regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD);                     \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_LBU                                                           \
  sprintf(remark, "lbu %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,             \
          regs_name[rs1]);                                                     \
  regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_BYTE_UNSIGNED);            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_LHU                                                           \
  sprintf(remark, "lhu %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,             \
          regs_name[rs1]);                                                     \
  regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_HWORD_UNSIGNED);           \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_LWU                                                           \
  sprintf(remark, "lwu %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,             \
          regs_name[rs1]);                                                     \
  regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_WORD_UNSIGNED);            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_LD                                                            \
  sprintf(remark, "ld %s,%ld(%s)", regs_name[rd], (int64_t)imm_i,              \
          regs_name[rs1]);                                                     \
  regs[rd] = mmu->load(regs[rs1] + imm_i, DATA_TYPE_DWORD);                    \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SB                                                            \
  sprintf(remark, "sb %s,%ld(%s)", regs_name[rs2], (int64_t)imm_s,             \
          regs_name[rs1]);                                                     \
  mmu->store(regs[rs1] + imm_s, DATA_TYPE_BYTE, regs[rs2]);                    \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SH                                                            \
  sprintf(remark, "sh %s,%ld(%s)", regs_name[rs2], (int64_t)imm_s,             \
          regs_name[rs1]);                                                     \
  mmu->store(regs[rs1] + imm_s, DATA_TYPE_HWORD, regs[rs2]);                   \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SW                                                            \
  sprintf(remark, "sw %s,%ld(%s)", regs_name[rs2], (int64_t)imm_s,             \
          regs_name[rs1]);                                                     \
  mmu->store(regs[rs1] + imm_s, DATA_TYPE_WORD, regs[rs2]);                    \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SD                                                            \
  sprintf(remark, "sd %s,%ld(%s)", regs_name[rs2], (int64_t)imm_s,             \
          regs_name[rs1]);                                                     \
  mmu->store(regs[rs1] + imm_s, DATA_TYPE_DWORD, regs[rs2]);                   \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_ADDI                                                          \
  if (!rd && !rs1 && !imm_i)                                                   \
    sprintf(remark, "nop");                                                    \
  else if (!rs1)                                                               \
    sprintf(remark, "li %s,%ld", regs_name[rd], (int64_t)imm_i);               \
  else if (!imm_i)                                                             \
    sprintf(remark, "mv %s,%s", regs_name[rd], regs_name[rs1]);                \
  else                                                                         \
    sprintf(remark, "addi %s,%s,%ld", regs_name[rd], regs_name[rs1],           \
            (int64_t)imm_i);                                                   \
  regs[rd] = regs[rs1] + imm_i;                                                \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLTI                                                          \
  sprintf(remark, "slti %s,%s,%ld", regs_name[rd], regs_name[rs1],             \
          (int64_t)imm_i);                                                     \
  regs[rd] = (int64_t)regs[rs1] < (int64_t)imm_i;                              \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLTIU                                                         \
  sprintf(remark, "sltiu %s,%s,%ld", regs_name[rd], regs_name[rs1],            \
          (int64_t)imm_i);                                                     \
  regs[rd] = regs[rs1] < imm_i;                                                \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_XORI                                                          \
  if (imm_i == -1U)                                                            \
    sprintf(remark, "not %s,%s", regs_name[rd], regs_name[rs1]);               \
  else                                                                         \
    sprintf(remark, "xori %s,%s,%ld", regs_name[rd], regs_name[rs1],           \
            (int64_t)imm_i);                                                   \
  regs[rd] = regs[rs1] ^ imm_i;                                                \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_ORI                                                           \
  sprintf(remark, "ori %s,%s,%ld", regs_name[rd], regs_name[rs1],              \
          (int64_t)imm_i);                                                     \
  regs[rd] = regs[rs1] | imm_i;                                                \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_ANDI                                                          \
  sprintf(remark, "andi %s,%s,%ld", regs_name[rd], regs_name[rs1],             \
          (int64_t)imm_i);                                                     \
  regs[rd] = regs[rs1] & imm_i;                                                \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLLI                                                          \
  sprintf(remark, "slli %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt64);  \
  regs[rd] = regs[rs1] << shamt64;                                             \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRLI                                                          \
  sprintf(remark, "srli %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt64);  \
  regs[rd] = regs[rs1] >> shamt64;                                             \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRAI                                                          \
  sprintf(remark, "srai %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt64);  \
  regs[rd] = (int64_t)regs[rs1] >> shamt64;                                    \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_ADD                                                           \
  sprintf(remark, "add %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] + regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLL                                                           \
  sprintf(remark, "sll %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] << (regs[rs2] & 0x3F);                                  \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLT                                                           \
  sprintf(remark, "slt %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = (int64_t)regs[rs1] < (int64_t)regs[rs2];                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLTU                                                          \
  sprintf(remark, "sltu %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] < regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_XOR                                                           \
  sprintf(remark, "xor %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] ^ regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRL                                                           \
  sprintf(remark, "srl %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] >> (regs[rs2] & 0x3F);                                  \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_OR                                                            \
  sprintf(remark, "or %s,%s,%s", regs_name[rd], regs_name[rs1],                \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] | regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_AND                                                           \
  sprintf(remark, "and %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] & regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SUB                                                           \
  sprintf(remark, "sub %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] - regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRA                                                           \
  sprintf(remark, "sra %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = (int64_t)regs[rs1] >> (regs[rs2] & 0x3F);                         \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_MUL                                                           \
  sprintf(remark, "mul %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] * regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_MULH                                                          \
  sprintf(remark, "mulh %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] * regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_MULHSU                                                        \
  sprintf(remark, "mulhsu %s,%s,%s", regs_name[rd], regs_name[rs1],            \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] * regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_MULHU                                                         \
  sprintf(remark, "mulhu %s,%s,%s", regs_name[rd], regs_name[rs1],             \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] * regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_DIV                                                           \
  sprintf(remark, "div %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = (int64_t)regs[rs1] / (int64_t)regs[rs2];                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_DIVU                                                          \
  sprintf(remark, "divu %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] / regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_REM                                                           \
  sprintf(remark, "rem %s,%s,%s", regs_name[rd], regs_name[rs1],               \
          regs_name[rs2]);                                                     \
  regs[rd] = (int64_t)regs[rs1] % (int64_t)regs[rs2];                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_REMU                                                          \
  sprintf(remark, "remu %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = regs[rs1] % regs[rs2];                                            \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_FENCE                                                         \
  if (pred != 0xf | succ != 0xf)                                               \
    sprintf(remark, "fence %s,%s", fence_flag(pred), fence_flag(succ));        \
  else                                                                         \
    sprintf(remark, "fence");                                                  \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_FENCE_I                                                       \
  sprintf(remark, "fence.i");                                                  \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_ECALL                                                         \
  sprintf(remark, "ecall");                                                    \
  switch (csr->prv) {                                                          \
  case PRV_U:                                                                  \
    throw TrapUserEcall();                                                     \
    break;                                                                     \
  case PRV_S:                                                                  \
    throw TrapSupervisorEcall();                                               \
    break;                                                                     \
  case PRV_H:                                                                  \
    throw TrapHypervisorEcall();                                               \
    break;                                                                     \
  case PRV_M:                                                                  \
    throw TrapMachineEcall();                                                  \
    break;                                                                     \
  default:                                                                     \
    abort();                                                                   \
  }                                                                            \
  break;

#define INSTRUCT_EBREAK                                                        \
  sprintf(remark, "ebreak");                                                   \
  sprintf(remark, "%s (no implement)", remark);                                \
  break;

#define INSTRUCT_WFI \
  sprintf(remark, "wfi");                                                      \
  throw WaitForInterrupt();                                                    \
  break;


#define INSTRUCT_SRET                                                          \
  {                                                                            \
    sprintf(remark, "sret");                                                   \
    pc = csr->sepc;                                                            \
    uint64_t status(csr->mstatus);                                             \
    csr->prv = get_field(status, MSTATUS_SPP);                                 \
    status = set_field(status, MSTATUS_SIE, get_field(status, MSTATUS_SPIE));  \
    status = set_field(status, MSTATUS_SPIE, 1);                               \
    status = set_field(status, MSTATUS_SPP, PRV_U);                            \
    csr->mstatus = status;                                                     \
  }                                                                            \
  break;

#define INSTRUCT_MRET                                                          \
  {                                                                            \
    sprintf(remark, "mret");                                                   \
    pc = csr->mepc;                                                            \
    uint64_t status(csr->mstatus);                                             \
    csr->prv = get_field(status, MSTATUS_MPP);                                 \
    status = set_field(status, MSTATUS_MIE, get_field(status, MSTATUS_MPIE));  \
    status = set_field(status, MSTATUS_MPIE, 1);                               \
    status = set_field(status, MSTATUS_MPP, PRV_U);                            \
    csr->mstatus = status;                                                     \
  }                                                                            \
  break;

#define INSTRUCT_CSRRW                                                         \
  {                                                                            \
    if (rd)                                                                    \
      sprintf(remark, "csrrw %s,%s,%s", regs_name[rd],                         \
              csr->csr_name(csr_addr), regs_name[rs1]);                        \
    else                                                                       \
      sprintf(remark, "csrw %s,%s", csr->csr_name(csr_addr), regs_name[rs1]);  \
    uint64_t tmp(csr->get_csr(csr_addr));                                      \
    csr->set_csr(csr_addr, regs[rs1]);                                         \
    regs[rd] = tmp;                                                            \
    pc += 4UL;                                                                 \
  }                                                                            \
  break;

#define INSTRUCT_CSRRS                                                         \
  {                                                                            \
    if (!rs1)                                                                  \
      sprintf(remark, "csrr %s,%s", regs_name[rd], csr->csr_name(csr_addr));   \
    else if (!rd)                                                              \
      sprintf(remark, "csrs %s,%s", csr->csr_name(csr_addr), regs_name[rs1]);  \
    else                                                                       \
      sprintf(remark, "csrrs %s,%s,%s", regs_name[rd],                         \
              csr->csr_name(csr_addr), regs_name[rs1]);                        \
    uint64_t tmp(csr->get_csr(csr_addr));                                      \
    csr->set_csr(csr_addr, tmp | regs[rs1]);                                   \
    regs[rd] = tmp;                                                            \
    pc += 4UL;                                                                 \
  }                                                                            \
  break;

#define INSTRUCT_CSRRC                                                         \
  {                                                                            \
    if (rd)                                                                    \
      sprintf(remark, "csrrc %s,%s,%s", regs_name[rd],                         \
              csr->csr_name(csr_addr), regs_name[rs1]);                        \
    else                                                                       \
      sprintf(remark, "csrc %s,%s", csr->csr_name(csr_addr), regs_name[rs1]);  \
    uint64_t tmp(csr->get_csr(csr_addr));                                      \
    csr->set_csr(csr_addr, tmp & ~regs[rs1]);                                  \
    regs[rd] = tmp;                                                            \
    pc += 4UL;                                                                 \
  }                                                                            \
  break;

#define INSTRUCT_CSRRWI                                                        \
  if (rd)                                                                      \
    sprintf(remark, "csrrwi %s,%s,%d", regs_name[rd], csr->csr_name(csr_addr), \
            rs1);                                                              \
  else                                                                         \
    sprintf(remark, "csrwi %s,%d", csr->csr_name(csr_addr), rs1);              \
  regs[rd] = csr->get_csr(csr_addr);                                           \
  csr->set_csr(csr_addr, zext(rs1, 5));                                        \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_CSRRSI                                                        \
  {                                                                            \
    if (rd)                                                                    \
      sprintf(remark, "csrrsi %s,%s,%d", regs_name[rd],                        \
              csr->csr_name(csr_addr), rs1);                                   \
    else                                                                       \
      sprintf(remark, "csrsi %s,%d", csr->csr_name(csr_addr), rs1);            \
    uint64_t tmp(csr->get_csr(csr_addr));                                      \
    csr->set_csr(csr_addr, tmp | zext(rs1, 5));                                \
    regs[rd] = tmp;                                                            \
    pc += 4UL;                                                                 \
  }                                                                            \
  break;

#define INSTRUCT_CSRRCI                                                        \
  {                                                                            \
    if (rd)                                                                    \
      sprintf(remark, "csrrci %s,%s,%d", regs_name[rd],                        \
              csr->csr_name(csr_addr), rs1);                                   \
    else                                                                       \
      sprintf(remark, "csrci %s,%d", csr->csr_name(csr_addr), rs1);            \
    uint64_t tmp(csr->get_csr(csr_addr));                                      \
    csr->set_csr(csr_addr, tmp &(~zext(rs1, 5)));                              \
    regs[rd] = tmp;                                                            \
    pc += 4UL;                                                                 \
  }                                                                            \
  break;

#define INSTRUCT_ADDIW                                                         \
  if (imm_i)                                                                   \
    sprintf(remark, "addiw %s,%s,%ld", regs_name[rd], regs_name[rs1],          \
            (int64_t)imm_i);                                                   \
  else                                                                         \
    sprintf(remark, "sext.w %s,%s", regs_name[rd], regs_name[rs1]);            \
  regs[rd] = (int32_t)regs[rs1] + (int32_t)imm_i;                              \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLLIW                                                         \
  sprintf(remark, "slliw %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt);   \
  regs[rd] = (int32_t)regs[rs1] << shamt;                                      \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRLIW                                                         \
  sprintf(remark, "srliw %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt);   \
  regs[rd] = (uint32_t)regs[rs1] >> shamt;                                     \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRAIW                                                         \
  sprintf(remark, "sraiw %s,%s,0x%x", regs_name[rd], regs_name[rs1], shamt64); \
  regs[rd] = (uint32_t)((int32_t)regs[rs1] >> shamt);                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_ADDW                                                          \
  sprintf(remark, "addw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (int32_t)regs[rs1] + (int32_t)regs[rs2];                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SUBW                                                          \
  sprintf(remark, "subw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (int32_t)regs[rs1] - (int32_t)regs[rs2];                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_MULW                                                          \
  sprintf(remark, "mulw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (regs[rs1] * regs[rs2]) << 32 >> 32;                              \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SLLW                                                          \
  sprintf(remark, "sllw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (int32_t)regs[rs1] << (regs[rs2] & 0x1F);                         \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_DIVW                                                          \
  sprintf(remark, "divw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (int32_t)regs[rs1] / (int32_t)regs[rs2];                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRLW                                                          \
  sprintf(remark, "srlw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (uint32_t)regs[rs1] >> (uint32_t)(regs[rs2] & 0x1F);              \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_SRAW                                                          \
  sprintf(remark, "sraw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (uint32_t)((int32_t)regs[rs1] >> (uint32_t)(regs[rs2] & 0x1F));   \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_DIVUW                                                         \
  sprintf(remark, "divuw %s,%s,%s", regs_name[rd], regs_name[rs1],             \
          regs_name[rs2]);                                                     \
  regs[rd] = (uint32_t)regs[rs1] / (uint32_t)regs[rs2];                        \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_REMW                                                          \
  sprintf(remark, "remw %s,%s,%s", regs_name[rd], regs_name[rs1],              \
          regs_name[rs2]);                                                     \
  regs[rd] = (int32_t)regs[rs1] / (int32_t)regs[rs2];                          \
  pc += 4UL;                                                                   \
  break;

#define INSTRUCT_REMUW                                                         \
  sprintf(remark, "remuw %s,%s,%s", regs_name[rd], regs_name[rs1],             \
          regs_name[rs2]);                                                     \
  regs[rd] = (uint32_t)regs[rs1] / (uint32_t)regs[rs2];                        \
  pc += 4UL;                                                                   \
  break;

#endif
