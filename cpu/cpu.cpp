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

CPU::CPU(uint64_t pc)
    : low_power(0), pc(pc), regs{0}, csr(new CSR(&this->pc)),
      mmu(new MMU(csr)) {}

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

    if (low_power)
      return;

    // Instruction Execute
    uint32_t insn(mmu->fetch(pc));
    uint8_t opcode(bits_zext(insn, 6, 0)), c_opcode(bits_zext(insn, 1, 0)),
        rd(bits_zext(insn, 11, 7)), funct3(bits_zext(insn, 14, 12)),
        funct7(bits_zext(insn, 31, 25)), c_funct3(bits_zext(insn, 15, 13)),
        c_funct4(bits_zext(insn, 15, 12)), c_funct6(bits_zext(insn, 15, 10)),
        c_funct2(bits_zext(insn, 6, 5)), _c_funct2(bits_zext(insn, 11, 10)),
        rs1(bits_zext(insn, 19, 15)), rs2(bits_zext(insn, 24, 20)),
        c_rs1(bits_zext(insn, 11, 7)), c_rs2(bits_zext(insn, 6, 2)),
        _c_rs1(bits_zext(insn, 9, 7)), _c_rs2(bits_zext(insn, 4, 2)),
        _c_rd(bits_zext(insn, 4, 2)), shamt(bits_zext(insn, 24, 20)),
        shamt64(bits_zext(insn, 25, 20)),
        c_shamt(bit(insn, 12) << 5 | bits_zext(insn, 6, 2)),
        pred(bits_zext(insn, 27, 24)), succ(bits_zext(insn, 23, 20));
    uint16_t funct12(bits_zext(insn, 31, 20)),
        csr_addr(bits_zext(insn, 31, 20));
    uint64_t imm_i(bits_sext(insn, 31, 20)),
        imm_s(bits_sext(insn, 31, 25) << 5 | bits_zext(insn, 11, 7)),
        imm_b(bit_signed(insn, 31) << 12 | bit(insn, 7) << 11 |
              bits_zext(insn, 30, 25) << 5 | bits_zext(insn, 11, 8) << 1),
        imm_u(bits_sext(insn, 31, 12) << 12),
        imm_j(bit_signed(insn, 31) << 20 | bits_zext(insn, 19, 12) << 12 |
              bit(insn, 20) << 11 | bits_zext(insn, 30, 21) << 1),
        imm_c_lwsp(bits_zext(insn, 3, 2) << 6 | bit(insn, 12) << 5 |
                   bits_zext(insn, 6, 4) << 2),
        imm_c_ldsp(bits_zext(insn, 4, 2) << 6 | bit(insn, 12) << 5 |
                   bits_zext(insn, 6, 5) << 3),
        imm_c_swsp(bits_zext(insn, 8, 7) << 6 | bits_zext(insn, 12, 9) << 2),
        imm_c_sdsp(bits_zext(insn, 9, 7) << 6 | bits_zext(insn, 12, 10) << 3),
        imm_c_slw(bit(insn, 5) << 6 | bits_zext(insn, 12, 10) << 3 |
                  bit(insn, 6) << 2),
        imm_c_sld(bits_zext(insn, 6, 5) << 6 | bits_zext(insn, 12, 10) << 3),
        imm_c_j(bit_signed(insn, 12) << 11 | bit(insn, 8) << 10 |
                bits_zext(insn, 10, 9) << 8 | bit(insn, 6) << 7 |
                bit(insn, 7) << 6 | bit(insn, 2) << 5 | bit(insn, 11) << 4 |
                bits_zext(insn, 5, 3) << 1),
        imm_c_b(bit_signed(insn, 12) << 8 | bits_zext(insn, 6, 5) << 6 |
                bit(insn, 2) << 5 | bits_zext(insn, 11, 10) << 3 |
                bits_zext(insn, 4, 3) << 1),
        imm_c_lui(bit_signed(insn, 12) << 17 | bits_zext(insn, 6, 2) << 12),
        imm_c_addi(bit_signed(insn, 12) << 5 | bits_zext(insn, 6, 2)),
        imm_c_addi16sp(bit_signed(insn, 12) << 9 | bits_zext(insn, 4, 3) << 7 |
                       bit(insn, 5) << 6 | bit(insn, 2) << 5 |
                       bit(insn, 6) << 4),
        imm_c_addi4spn(bits_zext(insn, 10, 7) << 6 |
                       bits_zext(insn, 12, 11) << 4 | bit(insn, 5) << 3 |
                       bit(insn, 6) << 2);

    _c_rs1 = (_c_rs1 & 0x7) | 0x8;
    _c_rs2 = (_c_rs2 & 0x7) | 0x8;
    _c_rd = (_c_rd & 0x7) | 0x8;

    int reg_num(0);
    printf("%08lx: %08x ", pc, insn);
    reg_num = REG_A0, printf("%s: %08lx ", regs_name[reg_num], regs[reg_num]);
    reg_num = REG_S0, printf("%s: %08lx ", regs_name[reg_num], regs[reg_num]);
    reg_num = REG_A4, printf("%s: %08lx ", regs_name[reg_num], regs[reg_num]);
    reg_num = REG_T0, printf("%s: %08lx ", regs_name[reg_num], regs[reg_num]);
    reg_num = REG_T1, printf("%s: %08lx ", regs_name[reg_num], regs[reg_num]);
    // reg_num = REG_T4, printf("%s: %08lx ", regs_name[reg_num], regs[reg_num]);

#include "cpu/exec.h"

    cout << remark << endl;
  } catch (Trap &t) {
    if ((int64_t)t.get_cause() >= 0)
      cout << remark << endl;
    cout << hex << "MSTATUS : " << csr->mstatus << endl;
    printf("%s, epc = %08lx, tval = %08lx\n", t.get_name(), pc, t.get_tval());
    trap_handling(t, pc);
  } catch (WaitForInterrupt &t) {
    cout << remark << endl;
    low_power = 1;
  }
  regs[0] = 0UL;

  // uint64_t addr(0x80002008), rdata;
  // cout << hex
  //      << "MEM[2008] : " << (mmu->read(addr, DATA_TYPE_DWORD, rdata), rdata)
  //      << endl;
  // cout << hex << "PRV : " << csr->prv << endl;
  // cout << hex << "STVAL : " << csr->stval << endl;
  // cout << hex << "SEPC : " << csr->sepc << endl;
  // cout << hex << "SIE : " << csr->get_csr(CSR_SIE_ADDR) << endl;
  // cout << hex << "SIP : " << csr->get_csr(CSR_SIP_ADDR) << endl;
  // cout << hex << "SATP : " << csr->satp << endl;
}

void CPU::trap_handling(const Trap &t, uint64_t epc) {
  uint64_t n(t.get_cause());
  uint64_t deleg(csr->medeleg);
  bool interrupt((int64_t)n < 0L);
  if (interrupt) {
    deleg = csr->mideleg;
    n &= ~(1UL << 63);
  }
  if (csr->prv <= PRV_S && n < 64 && (deleg & (1UL << n))) { // Using S-mode
    uint64_t vector(interrupt && (csr->stvec & 1UL) ? (n << 2) : 0);
    pc = (csr->stvec & ~3UL) + vector;
    csr->scause = t.get_cause();
    csr->sepc = epc;
    csr->stval = t.get_tval();
    cout << hex << "STVAL : " << csr->stval << endl;

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
    csr->mtval = t.get_tval();

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
  if (ints && low_power)
    low_power = 0, npc += 4UL;

  // Check M-mode interrupts
  bool m_en((csr->prv == PRV_M && (csr->mstatus & MSTATUS_MIE)) ||
            csr->prv < PRV_M);
  uint64_t ints_en(m_en ? ints & ~(csr->mideleg) : 0UL);

  // Check S-mode interrupts, if M-mode has no match interrupts
  if (!ints_en) {
    bool s_en((csr->prv == PRV_S && (csr->mstatus & MSTATUS_SIE)) ||
              csr->prv < PRV_S);
    ints_en = s_en ? ints & csr->mideleg : 0UL;
  }

  if (ints_en) {
    if (ints_en >> IRQ_NONSTANDARD)
      ints_en = ints_en >> IRQ_NONSTANDARD << IRQ_NONSTANDARD;
    else if (ints_en & MIP_MEIP)
      ints_en = MIP_MEIP;
    else if (ints_en & MIP_MSIP)
      ints_en = MIP_MSIP;
    else if (ints_en & MIP_MTIP)
      ints_en = MIP_MTIP;
    else if (ints_en & MIP_SEIP)
      ints_en = MIP_SEIP;
    else if (ints_en & MIP_SSIP)
      ints_en = MIP_SSIP;
    else if (ints_en & MIP_STIP)
      ints_en = MIP_STIP;
    else
      abort();

    throw Trap((1UL << 63) | lsb_leading_zero(ints_en));
  }

  // Execute instruction after wfi
  pc = npc;
}

void CPU::bus_connect(pBus bus) { mmu->connect(bus); }
