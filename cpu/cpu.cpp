#include "cpu/cpu.h"
#include "bus/datatype.h"
#include "cpu/decode.h"
#include "util/util.h"
#include <iostream>
using namespace std;

const char CPU::regs_name[32][5] = {
    "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0", "s1", "a0",
    "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3", "s4", "s5",
    "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

const char *CPU::fence_flag(const uint8_t &arg) {
  char *str = new char[4];
  uint8_t ptr(0);
  if (arg & 0x8)
    str[ptr++] = 'i';
  if (arg & 0x4)
    str[ptr++] = 'o';
  if (arg & 0x2)
    str[ptr++] = 'r';
  if (arg & 0x1)
    str[ptr++] = 'w';
  if (!ptr)
    str[ptr++] = '-';
  str[ptr] = 0;
  return str;
}

CPU::CPU(uint64_t pc) : low_power(0), pc(pc), regs{0}, csr(new CSR(&pc)), mmu(new MMU(csr)) {}

CPU::~CPU() {
  delete mmu;
  delete csr;
}

void CPU::_init() {}

void CPU::run() {
  char remark[100] = "Unknown instruction";

  try {
    // Check interrupts
    take_interrupt(csr->mip & csr->mie);

    if (low_power) return;

    // Instruction Execute
    uint32_t insn(mmu->fetch(pc));
    uint8_t opcode(bits_zext(insn, 6, 0)), rd(bits_zext(insn, 11, 7)),
        funct3(bits_zext(insn, 14, 12)), funct7(bits_zext(insn, 31, 25)),
        rs1(bits_zext(insn, 19, 15)), rs2(bits_zext(insn, 24, 20)),
        shamt(bits_zext(insn, 24, 20)), shamt64(bits_zext(insn, 25, 20)),
        pred(bits_zext(insn, 27, 24)), succ(bits_zext(insn, 23, 20));
    uint16_t funct12(bits_zext(insn, 31, 20)),
        csr_addr(bits_zext(insn, 31, 20));
    uint64_t imm_i(bits_sext(insn, 31, 20)),
        imm_s(bits_sext(insn, 31, 25) << 5 | bits_zext(insn, 11, 7)),
        imm_b(bit_signed(insn, 31) << 12 | bit(insn, 7) << 11 |
              bits_zext(insn, 30, 25) << 5 | bits_zext(insn, 11, 8) << 1),
        imm_u(bits_sext(insn, 31, 12) << 12),
        imm_j(bit_signed(insn, 31) << 20 | bits_zext(insn, 19, 12) << 12 |
              bit(insn, 20) << 11 | bits_zext(insn, 30, 21) << 1);

    printf("%08lx: %08x %s: %08lx %s: %08lx %s: %08lx %s: %08lx    ", pc, insn,
           regs_name[10], regs[10], regs_name[11], regs[11], regs_name[5],
           regs[5], regs_name[6], regs[6]);

    switch (opcode) {
      DECLARECASE(OPCODE_LUI, INSTRUCT_LUI)
      DECLARECASE(OPCODE_AUIPC, INSTRUCT_AUIPC)
      DECLARECASE(OPCODE_JAL, INSTRUCT_JAL)
      DECLARECASE(OPCODE_JALR, INSTRUCT_JALR)
    case OPCODE_BRANCH:
      switch (funct3) {
        DECLARECASE(FUNCT3_BEQ, INSTRUCT_BEQ)
        DECLARECASE(FUNCT3_BNE, INSTRUCT_BNE)
        DECLARECASE(FUNCT3_BLT, INSTRUCT_BLT)
        DECLARECASE(FUNCT3_BGE, INSTRUCT_BGE)
        DECLARECASE(FUNCT3_BLTU, INSTRUCT_BLTU)
        DECLARECASE(FUNCT3_BGEU, INSTRUCT_BGEU)
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    case OPCODE_LOAD:
      switch (funct3) {
        DECLARECASE(FUNCT3_LB, INSTRUCT_LB)
        DECLARECASE(FUNCT3_LH, INSTRUCT_LH)
        DECLARECASE(FUNCT3_LW, INSTRUCT_LW)
        DECLARECASE(FUNCT3_LBU, INSTRUCT_LBU)
        DECLARECASE(FUNCT3_LHU, INSTRUCT_LHU)
        DECLARECASE(FUNCT3_LWU, INSTRUCT_LWU) // RV64
        DECLARECASE(FUNCT3_LD, INSTRUCT_LD)   // RV64
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    case OPCODE_STORE:
      switch (funct3) {
        DECLARECASE(FUNCT3_SB, INSTRUCT_SB)
        DECLARECASE(FUNCT3_SH, INSTRUCT_SH)
        DECLARECASE(FUNCT3_SW, INSTRUCT_SW)
        DECLARECASE(FUNCT3_SD, INSTRUCT_SD) // RV64
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    case OPCODE_OP_IMM:
      switch (funct3) {
        DECLARECASE(FUNCT3_ADDI, INSTRUCT_ADDI)
        DECLARECASE(FUNCT3_SLTI, INSTRUCT_SLTI)
        DECLARECASE(FUNCT3_SLTIU, INSTRUCT_SLTIU)
        DECLARECASE(FUNCT3_XORI, INSTRUCT_XORI)
        DECLARECASE(FUNCT3_ORI, INSTRUCT_ORI)
        DECLARECASE(FUNCT3_ANDI, INSTRUCT_ANDI)
        DECLARECASE(FUNCT3_SLLI, INSTRUCT_SLLI)
      case FUNCT3_SRI:
        switch (funct7 & 0x3e) {
          DECLARECASE(FUNCT7_SRLI, INSTRUCT_SRLI)
          DECLARECASE(FUNCT7_SRAI, INSTRUCT_SRAI)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    case OPCODE_OP:
      switch (funct7) {
      case FUNCT7_ADD:
        switch (funct3) {
          DECLARECASE(FUNCT3_ADD, INSTRUCT_ADD)
          DECLARECASE(FUNCT3_SLL, INSTRUCT_SLL)
          DECLARECASE(FUNCT3_SLT, INSTRUCT_SLT)
          DECLARECASE(FUNCT3_SLTU, INSTRUCT_SLTU)
          DECLARECASE(FUNCT3_XOR, INSTRUCT_XOR)
          DECLARECASE(FUNCT3_SRR, INSTRUCT_SRL)
          DECLARECASE(FUNCT3_OR, INSTRUCT_OR)
          DECLARECASE(FUNCT3_AND, INSTRUCT_AND)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
      case FUNCT7_SUB:
        switch (funct3) {
          DECLARECASE(FUNCT3_ADD, INSTRUCT_SUB)
          DECLARECASE(FUNCT3_SRR, INSTRUCT_SRA)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
      case FUNCT7_MULDIV:
        switch (funct3) {
          DECLARECASE(FUNCT3_MUL, INSTRUCT_MUL)
          DECLARECASE(FUNCT3_MULH, INSTRUCT_MULH)
          DECLARECASE(FUNCT3_MULHSU, INSTRUCT_MULHSU)
          DECLARECASE(FUNCT3_MULHU, INSTRUCT_MULHU)
          DECLARECASE(FUNCT3_DIV, INSTRUCT_DIV)
          DECLARECASE(FUNCT3_DIVU, INSTRUCT_DIVU)
          DECLARECASE(FUNCT3_REM, INSTRUCT_REM)
          DECLARECASE(FUNCT3_REMU, INSTRUCT_REMU)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    case OPCODE_MISC_MEM:
      switch (funct3) {
        DECLARECASE(FUNCT3_FENCE, INSTRUCT_FENCE)
        DECLARECASE(FUNCT3_FENCE_I, INSTRUCT_FENCE_I)
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    case OPCODE_SYSTEM:
      switch (funct3) {
      case FUNCT3_PRIV:
        switch (funct12) {
          DECLARECASE(FUNCT12_ECALL, INSTRUCT_ECALL)
          DECLARECASE(FUNCT12_EBREAK, INSTRUCT_EBREAK)
          DECLARECASE(FUNCT12_WFI, INSTRUCT_WFI)
          DECLARECASE(FUNCT12_MRET, INSTRUCT_MRET)
          DECLARECASE(FUNCT12_SRET, INSTRUCT_SRET)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
        DECLARECASE(FUNCT3_CSRRW, INSTRUCT_CSRRW)
        DECLARECASE(FUNCT3_CSRRS, INSTRUCT_CSRRS)
        DECLARECASE(FUNCT3_CSRRC, INSTRUCT_CSRRC)
        DECLARECASE(FUNCT3_CSRRWI, INSTRUCT_CSRRWI)
        DECLARECASE(FUNCT3_CSRRSI, INSTRUCT_CSRRSI)
        DECLARECASE(FUNCT3_CSRRCI, INSTRUCT_CSRRCI)
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    // RV64
    case OPCODE_OP_IMM_32:
      switch (funct3) {
        DECLARECASE(FUNCT3_ADDI, INSTRUCT_ADDIW)
        DECLARECASE(FUNCT3_SLLI, INSTRUCT_SLLIW)
      case FUNCT3_SRI:
        switch (funct7) {
          DECLARECASE(FUNCT7_SRLI, INSTRUCT_SRLIW)
          DECLARECASE(FUNCT7_SRAI, INSTRUCT_SRAIW)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
    case OPCODE_OP_32:
      switch (funct3) {
      case FUNCT3_ADD:
        switch (funct7) {
          DECLARECASE(FUNCT7_ADD, INSTRUCT_ADDW)
          DECLARECASE(FUNCT7_SUB, INSTRUCT_SUBW)
          DECLARECASE(FUNCT7_MULDIV, INSTRUCT_MULW)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
        DECLARECASE(FUNCT3_SLL, INSTRUCT_SLLW)
        DECLARECASE(FUNCT3_DIV, INSTRUCT_DIVW)
      case FUNCT3_SRR:
        switch (funct7) {
          DECLARECASE(FUNCT7_SRL, INSTRUCT_SRLW)
          DECLARECASE(FUNCT7_SRA, INSTRUCT_SRAW)
          DECLARECASE(FUNCT7_MULDIV, INSTRUCT_DIVUW)
          DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
        }
        break;
        DECLARECASE(FUNCT3_REM, INSTRUCT_REMW)
        DECLARECASE(FUNCT3_REMU, INSTRUCT_REMUW)
        DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
      }
      break;
      DECLAREDEFAULTCASE(INSTRUCT_UNKNOWN)
    }
    cout << remark << endl;
  } catch (Trap &t) {
    if ((int64_t) t.get_cause() >= 0) cout << remark << endl;
    cout << hex<< "MIE : " << csr->mie << endl;
    cout << hex<< "MIP : " << csr->mip << endl;
    cout << hex<< "MIDELEG : " << csr->mideleg << endl;
    cout << hex<< "MTVEC : " << csr->mtvec << endl;
    printf("%s, epc = %08lx, tval = %08lx\n", t.get_name(), pc, t.get_tval());
    trap_handling(t, pc);
  } catch (WaitForInterrupt &t) {
    cout << remark << endl;
    low_power = 1;
  }
  regs[0] = 0UL;
  cout << hex << "PRV : " << csr->prv << endl;
}

void CPU::trap_handling(Trap t, uint64_t epc) {
  uint64_t n(t.get_cause());
  uint64_t deleg;
  bool interrupt((int64_t) n < 0L);
  if (interrupt) {
    deleg = csr->mideleg;
    n &= ~(1UL << 63);
  }
  if (csr->prv <= PRV_S && n < 64 && (deleg & (1UL << n))) { // Using S-mode
    uint64_t vector(interrupt && (csr->stvec & 1UL) ? (n << 2) : 0);
    pc = (csr->stvec & ~3UL) + vector;
    csr->scause = t.get_cause();
    csr->sepc = epc;
    csr->stval = 0UL;

    uint64_t status = csr->mstatus;
    status = set_field(status, MSTATUS_SPIE, get_field(status, MSTATUS_SIE));
    status = set_field(status, MSTATUS_SPP, csr->prv);
    status = set_field(status, MSTATUS_SIE, 0);
    csr->mstatus = status;
    csr->prv = PRV_S;
  } else { // Using M-mode
    uint64_t vector(interrupt && (csr->mtvec & 1UL) ? (n << 2) : 0);
    pc = (csr->mtvec & ~3UL) + vector;
    csr->mcause = t.get_cause();
    csr->mepc = epc;
    csr->mtval = 0UL;

    uint64_t status = csr->mstatus;
    status = set_field(status, MSTATUS_MPIE, get_field(status, MSTATUS_MIE));
    status = set_field(status, MSTATUS_MPP, csr->prv);
    status = set_field(status, MSTATUS_MIE, 0);
    csr->mstatus = status;
    csr->prv = PRV_M;
  }
}

void CPU::take_interrupt(uint64_t ints) {
  uint64_t npc(pc);

  // Set CPU low power disable
  if (ints && low_power) low_power = 0, npc += 4UL;

  // Check M-mode interrupts
  bool m_en((csr->prv == PRV_M && (csr->mstatus & MSTATUS_MIE)) || csr->prv < PRV_M);
  uint64_t ints_en(m_en ? ints & ~(csr->mideleg) : 0UL);

  // Check S-mode interrupts, if M-mode has no match interrupts
  if (!ints_en) {
    bool s_en((csr->prv == PRV_S && (csr->mstatus & MSTATUS_SIE)) || csr->prv < PRV_S);
    ints_en = s_en ? ints & csr->mideleg : 0UL;
  }

  if (ints_en) {
    if (ints_en >> IRQ_NONSTANDARD) ints_en = ints_en >> IRQ_NONSTANDARD << IRQ_NONSTANDARD;
    else if (ints_en & MIP_MEIP) ints_en = MIP_MEIP;
    else if (ints_en & MIP_MSIP) ints_en = MIP_MSIP;
    else if (ints_en & MIP_MTIP) ints_en = MIP_MTIP;
    else if (ints_en & MIP_SEIP) ints_en = MIP_SEIP;
    else if (ints_en & MIP_SSIP) ints_en = MIP_SSIP;
    else if (ints_en & MIP_STIP) ints_en = MIP_STIP;
    else abort();

    throw Trap((1UL << 63) | lsb_leading_zero(ints_en));
  }
  
  // Execute instruction after wfi
  pc = npc;
}

void CPU::bus_connect(pBus bus) { mmu->connect(bus); }
