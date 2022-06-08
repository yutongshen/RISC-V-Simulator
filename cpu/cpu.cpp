#include "cpu/cpu.h"
#include <iomanip>
#include <iostream>
#include "bus/datatype.h"
#include "cpu/arith.h"
#include "cpu/decode.h"
#include "cpu/rule_define.h"
#include "mmap/clint_reg.h"
#include "mmap/mmap.h"
#include "util/util.h"
using namespace std;

#define LS_NONE 0x0
#define LS_LOAD 0x1
#define LS_STORE 0x2

extern bool verbose;
extern bool __exit;

void CPU::get_funct_offset(char *str, const uint64_t &pc)
{
    uint64_t pc_ref0, pc_ref1;
    char str_ref0[64], str_ref1[64];
    FILE *flist = fopen("./build/prog/funct.list", "r");
    // std::cout << "[DBG]: " << (uint64_t) flist << "\r\n";
    if (flist) {
        pc_ref0 = 0;
        sprintf(str_ref0, "__unknown__");
        while (!feof(flist)) {
            fscanf(flist, "%lx %s", &pc_ref1, str_ref1);
            // std::cout << "[DBG]: " << pc_ref1 << ", " << str_ref1 << "\r\n";
            if (pc < pc_ref1) {
                sprintf(str, "<%s+0x%lx>", str_ref0, pc - pc_ref0);
                return;
            }
            pc_ref0 = pc_ref1;
            sprintf(str_ref0, "%s", str_ref1);
        }
        if (pc >= pc_ref0) {
            sprintf(str, "<%s+0x%lx>", str_ref0, pc - pc_ref0);
            return;
        }
    }
    sprintf(str, "<__unknown__+0x%lx>", pc);
}

const char CPU::regs_name[32][5] = {
    "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0", "s1", "a0",
    "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3", "s4", "s5",
    "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

const char CPU::fregs_name[32][5] = {
    "ft0", "ft1", "ft2",  "ft3",  "ft4", "ft5", "ft6",  "ft7",
    "fs0", "fs1", "fa0",  "fa1",  "fa2", "fa3", "fa4",  "fa5",
    "fa6", "fa7", "fs2",  "fs3",  "fs4", "fs5", "fs6",  "fs7",
    "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11"};

const char *CPU::fence_flag(const uint8_t &arg)
{
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

CPU::CPU(uint64_t cpuid, uint64_t pc)
    : power_sta(0),
      low_power(0),
      pc(pc),
      regs{0},
      fregs{0},
      ras(),
      pcs(),
      csr(new CSR(cpuid, &this->pc)),
      mmu(new MMU(csr))
{
    char trace_file_name[50];
    sprintf(trace_file_name, "build/cpu_trace_core%ld.out", cpuid);
    cpu_trace.open(trace_file_name);
}

CPU::~CPU()
{
    char str[64];
    get_funct_offset(str, pc);
    std::cout << "CPU" << csr->mhartid << ":\r\n";
    std::cout << "  At   " << std::hex << std::setfill('0') << std::setw(8)
              << pc << " " << str << "\r\n";
    while (!pcs.empty()) {
        get_funct_offset(str, pcs.top());
        std::cout << "  From " << std::hex << std::setfill('0') << std::setw(8)
                  << pcs.top() << " " << str << "\r\n";
        pcs.pop();
    }
    delete mmu;
    delete csr;
    cpu_trace.close();
}

void CPU::_init() {}

void CPU::single_step()
{
    if (!power_sta)
        return;

    char remark[100] = "Unknown instruction";

    try {
        // Check interrupts
        take_interrupt(csr->mip & csr->mie);

        if (low_power)
            return;

        // Instruction Execute
        uint64_t paddr;
        bool ras_push(0), ras_pop(0);
        uint64_t ipc(pc);
        DataType mem_val_type;
        uint16_t instr_len(0), write_reg(REG_ZERO), write_csr(-1),
            load_store(LS_NONE);
        uint64_t mem_addr, mem_val;

        uint32_t insn(mmu->fetch(pc, pc_alignment_mask));
        insn = (insn & 3) == 3 ? insn : (insn & 0xffff);
        bool aq(bit(insn, 26)), rl(bit(insn, 25));
        uint8_t opcode(bits_zext(insn, 6, 0)), c_opcode(bits_zext(insn, 1, 0)),
            rd(bits_zext(insn, 11, 7)), funct3(bits_zext(insn, 14, 12)),
            funct5(bits_zext(insn, 31, 27)), funct7(bits_zext(insn, 31, 25)),
            c_funct3(bits_zext(insn, 15, 13)),
            c_funct4(bits_zext(insn, 15, 12)),
            c_funct6(bits_zext(insn, 15, 10)), c_funct2(bits_zext(insn, 6, 5)),
            _c_funct2(bits_zext(insn, 11, 10)), rs1(bits_zext(insn, 19, 15)),
            rs2(bits_zext(insn, 24, 20)), rs3(bits_zext(insn, 31, 27)),
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
            imm_c_swsp(bits_zext(insn, 8, 7) << 6 | bits_zext(insn, 12, 9)
                                                        << 2),
            imm_c_sdsp(bits_zext(insn, 9, 7) << 6 | bits_zext(insn, 12, 10)
                                                        << 3),
            imm_c_slw(bit(insn, 5) << 6 | bits_zext(insn, 12, 10) << 3 |
                      bit(insn, 6) << 2),
            imm_c_sld(bits_zext(insn, 6, 5) << 6 | bits_zext(insn, 12, 10)
                                                       << 3),
            imm_c_j(bit_signed(insn, 12) << 11 | bit(insn, 8) << 10 |
                    bits_zext(insn, 10, 9) << 8 | bit(insn, 6) << 7 |
                    bit(insn, 7) << 6 | bit(insn, 2) << 5 | bit(insn, 11) << 4 |
                    bits_zext(insn, 5, 3) << 1),
            imm_c_b(bit_signed(insn, 12) << 8 | bits_zext(insn, 6, 5) << 6 |
                    bit(insn, 2) << 5 | bits_zext(insn, 11, 10) << 3 |
                    bits_zext(insn, 4, 3) << 1),
            imm_c_lui(bit_signed(insn, 12) << 17 | bits_zext(insn, 6, 2) << 12),
            imm_c_addi(bit_signed(insn, 12) << 5 | bits_zext(insn, 6, 2)),
            imm_c_addi16sp(bit_signed(insn, 12) << 9 |
                           bits_zext(insn, 4, 3) << 7 | bit(insn, 5) << 6 |
                           bit(insn, 2) << 5 | bit(insn, 6) << 4),
            imm_c_addi4spn(bits_zext(insn, 10, 7) << 6 |
                           bits_zext(insn, 12, 11) << 4 | bit(insn, 5) << 3 |
                           bit(insn, 6) << 2);

        _c_rs1 = (_c_rs1 & 0x7) | 0x8;
        _c_rs2 = (_c_rs2 & 0x7) | 0x8;
        _c_rd = (_c_rd & 0x7) | 0x8;

        if (verbose) {
            uint64_t mtime;
            switch (csr->prv) {
            case 0:
                cpu_trace << "[U] ";
                break;
            case 1:
                cpu_trace << "[S] ";
                break;
            case 3:
                cpu_trace << "[M] ";
                break;
            }
            mmu->read(CLINT_BASE + RG_TIME, DATA_TYPE_DWORD, mtime);
            cpu_trace << std::dec << std::setfill(' ') << std::setw(5) << mtime
                      << "ns ";
            cpu_trace << std::hex << std::setfill('0') << std::setw(16) << pc;
            paddr = mmu->trace_pt(pc, 8, ACCESS_TYPE_FETCH, csr->prv);
            if (pc != paddr) {
                cpu_trace << "(" << std::hex << std::setfill('0')
                          << std::setw(16) << paddr << ")";
            }
            cpu_trace << ": " << std::hex << std::setfill('0') << std::setw(8)
                      << insn << " ";
        }
#include "cpu/exec.h"

        // if (ras_push) {
        //     pcs.push(ipc);
        //     ras.push(regs[REG_RA]);
        // }
        // else if (ras_pop) {
        //     pcs.pop();
        //     ras.pop();
        // }
        // if (load_store == LS_STORE && mem_addr == 0xffffffe000000000) {
        //     printf("[CPU%d] verbose start!\r\n", csr->mhartid);
        //     verbose = 1;
        // }

        if (verbose) {
            cpu_trace << remark << std::endl;

            if (load_store) {
                uint8_t data_len;
                data_len =
                    mem_val_type == DATA_TYPE_BYTE
                        ? 8
                        : mem_val_type == DATA_TYPE_HWORD
                              ? 16
                              : mem_val_type == DATA_TYPE_HWORD_UNSIGNED
                                    ? 16
                                    : mem_val_type == DATA_TYPE_WORD
                                          ? 32
                                          : mem_val_type ==
                                                    DATA_TYPE_WORD_UNSIGNED
                                                ? 32
                                                : 64;
                mem_val &= data_len == 64 ? -1L : (1L << data_len) - 1L;

                cpu_trace << "  "
                          << ((load_store == LS_LOAD) ? "LOAD " : "STORE")
                          << "    [" << std::hex << std::setfill('0')
                          << std::setw(16) << std::right << mem_addr;

                paddr = mmu->trace_pt(mem_addr, data_len >> 3,
                                      load_store == LS_STORE ? ACCESS_TYPE_STORE
                                                             : ACCESS_TYPE_LOAD,
                                      csr->prv);
                if (mem_addr != paddr) {
                    cpu_trace << "(" << std::hex << std::setfill('0')
                              << std::setw(16) << paddr << ")";
                }
                cpu_trace << std::setfill('-')
                          << std::setw(18 - (data_len >> 2)) << std::left
                          << "] " << std::hex << std::setfill('0')
                          << std::setw(data_len >> 2) << std::right << mem_val
                          << std::endl;
            }

            bool freg_sel(write_reg >> 5);
            write_reg &= 0x1f;

            if (!freg_sel) {
                if (write_reg) {
                    cpu_trace << "  " << std::setfill(' ') << std::setw(10)
                              << std::left << regs_name[write_reg] << std::hex
                              << std::setfill('0') << std::setw(16)
                              << std::right << regs[write_reg] << std::endl;
                }
            } else {
                cpu_trace << "  " << std::setfill(' ') << std::setw(10)
                          << std::left << fregs_name[write_reg] << std::hex
                          << std::setfill('0') << std::setw(16) << std::right
                          << fregs[write_reg] << std::endl;
            }

            if (write_csr != 0xffff) {
                cpu_trace << "  " << std::setfill(' ') << std::setw(10)
                          << std::left << csr->csr_name(write_csr)
                          << std::setfill('0') << std::setw(16) << std::right
                          << csr->get_csr(write_csr) << std::endl;
            }
        }
        // if (!(csr->mstatus & MSTATUS_FS)) __exit = 1;
    } catch (Trap &t) {
        if ((int64_t) t.get_cause() >= 0)
            if (verbose) {
                cpu_trace << remark << std::endl;
            }
        trap_handling(t, pc);
    } catch (WaitForInterrupt &t) {
        if (verbose) {
            cpu_trace << remark << std::endl;
        }
        low_power = 1;
        pc += 4;
    }
    regs[0] = 0UL;

    ++csr->time;
    ++csr->mcycle;
    ++csr->minstret;
}

void CPU::trap_handling(Trap &t, uint64_t epc)
{
    uint64_t n(t.get_cause());
    uint64_t deleg(csr->medeleg);
    bool interrupt((int64_t) n < 0L);
    if (verbose) {
        cpu_trace << t.get_name() << ", epc = " << std::hex << std::setfill('0')
                  << std::setw(16) << epc << ", tval = " << std::hex
                  << std::setfill('0') << std::setw(16) << t.get_tval()
                  << std::endl;
        // printf("%s, epc = %08lx, tval = %08lx\r\n", t.get_name(), epc,
        //        t.get_tval());
    }
    if (interrupt) {
        deleg = csr->mideleg;
        n &= ~(1UL << 63);
    }
    if (csr->prv <= PRV_S && n < 64 && (deleg & (1UL << n))) {  // Using S-mode
        uint64_t vector(interrupt && (csr->stvec & 1UL) ? (n << 2) : 0);
        pc = (csr->stvec & ~3UL) + vector;
        csr->scause = t.get_cause();
        csr->sepc = epc;
        csr->stval = t.get_tval();

        uint64_t status = csr->mstatus;
        status =
            set_field(status, MSTATUS_SPIE, get_field(status, MSTATUS_SIE));
        status = set_field(status, MSTATUS_SPP, csr->prv);
        status = set_field(status, MSTATUS_SIE, 0);
        csr->mstatus = status;
        csr->prv = PRV_S;
    } else {  // Using M-mode
        uint64_t vector(interrupt && (csr->mtvec & 1UL) ? (n << 2) : 0);
        pc = (csr->mtvec & ~3UL) + vector;
        csr->mcause = t.get_cause();
        csr->mepc = epc;
        csr->mtval = t.get_tval();

        uint64_t status = csr->mstatus;
        status =
            set_field(status, MSTATUS_MPIE, get_field(status, MSTATUS_MIE));
        status = set_field(status, MSTATUS_MPP, csr->prv);
        status = set_field(status, MSTATUS_MIE, 0);
        csr->mstatus = status;
        csr->prv = PRV_M;
    }
}

void CPU::take_interrupt(uint64_t ints)
{
    // Set CPU low power disable
    if (ints && low_power)
        low_power = 0;

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

        throw Trap((1UL << 63) | ctz(ints_en));
    }
}

void CPU::set_power_on(bool power_sta)
{
    this->power_sta = power_sta;
    if (verbose) {
        uint64_t mtime;
        mmu->read(CLINT_BASE + RG_TIME, DATA_TYPE_DWORD, mtime);
        printf("%6ld ns: ** CORE%ld POWER %s!! **\r\n", mtime, csr->mhartid,
               power_sta ? "ON" : "OFF");
    }
}

bool CPU::get_power_sta()
{
    return this->power_sta;
}

void CPU::bus_connect(pBus bus)
{
    mmu->mb_connect(bus);
}

bool CPU::support_extension(char ext)
{
    return csr->support_extension(ext);
}
