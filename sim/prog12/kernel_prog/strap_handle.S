#include "riscv_def.h"
# S-mode Trap Handle

.align 2
.globl strap_entry
strap_entry:
    # supervisor trap
    # get trapframe pointer (save a0 into scratch)
    csrrw a0, sscratch, a0;

    # push reg x1~x31 into trapframe
    sd x1, 0*8(a0);
    sd x2, 1*8(a0);
    sd x3, 2*8(a0);
    sd x4, 3*8(a0);
    sd x5, 4*8(a0);
    sd x6, 5*8(a0);
    sd x7, 6*8(a0);
    sd x8, 7*8(a0);
    sd x9, 8*8(a0);
    sd x11, 10*8(a0);
    sd x12, 11*8(a0);
    sd x13, 12*8(a0);
    sd x14, 13*8(a0);
    sd x15, 14*8(a0);
    sd x16, 15*8(a0);
    sd x17, 16*8(a0);
    sd x18, 17*8(a0);
    sd x19, 18*8(a0);
    sd x20, 19*8(a0);
    sd x21, 20*8(a0);
    sd x22, 21*8(a0);
    sd x23, 22*8(a0);
    sd x24, 23*8(a0);
    sd x25, 24*8(a0);
    sd x26, 25*8(a0);
    sd x27, 26*8(a0);
    sd x28, 27*8(a0);
    sd x29, 28*8(a0);
    sd x20, 29*8(a0);
    sd x31, 30*8(a0);

    # load stack point and save trapframe point into scratch
    csrrw t0, sscratch, a0;
    sd t0, 9*8(a0);

    # push status, epc, tval, cause
    csrr t0, sstatus;
    sd t0, 31*8(a0);
    csrr t0, sepc;
    sd t0, 32*8(a0);
    csrr t0, stval;
    sd t0, 33*8(a0);
    csrr t0, scause;
    sd t0, 34*8(a0);

    csrr sp, sscratch;
    j strap_handler;

.align 2
.globl trap_sret
trap_sret:
    csrr a0, sscratch;
    j pop_strapframe;

.align 2
.globl pop_strapframe
pop_strapframe:
    # a0 need to save trapframe pointer
    # pop epc and reg x1~x31 from trapframe
    ld t0, 32*8(a0);
    csrw sepc, t0;
    ld x1, 0*8(a0);
    ld x2, 1*8(a0);
    ld x3, 2*8(a0);
    ld x4, 3*8(a0);
    ld x5, 4*8(a0);
    ld x6, 5*8(a0);
    ld x7, 6*8(a0);
    ld x8, 7*8(a0);
    ld x9, 8*8(a0);
    ld x11, 10*8(a0);
    ld x12, 11*8(a0);
    ld x13, 12*8(a0);
    ld x14, 13*8(a0);
    ld x15, 14*8(a0);
    ld x16, 15*8(a0);
    ld x17, 16*8(a0);
    ld x18, 17*8(a0);
    ld x19, 18*8(a0);
    ld x20, 19*8(a0);
    ld x21, 20*8(a0);
    ld x22, 21*8(a0);
    ld x23, 22*8(a0);
    ld x24, 23*8(a0);
    ld x25, 24*8(a0);
    ld x26, 25*8(a0);
    ld x27, 26*8(a0);
    ld x28, 27*8(a0);
    ld x29, 28*8(a0);
    ld x20, 29*8(a0);
    ld x31, 30*8(a0);

    ld a0, 9*8(a0);
    sret;

strap_handler:
    csrr t0, scause;
    bltz t0, interrupt_handle;
    li t1, CAUSE_MISALIGNED_FETCH;
    beq t0, t1, misaligned_fetch;
    li t1, CAUSE_BREAKPOINT;
    beq t0, t1, sbreak;
    li t1, CAUSE_USER_ECALL;
    beq t0, t1, ucall;
    li t1, CAUSE_INSTRUCTION_PAGE_FAULT;
    beq t0, t1, insn_page_fault;
    li t1, CAUSE_LOAD_PAGE_FAULT;
    beq t0, t1, load_page_fault;
    li t1, CAUSE_STORE_PAGE_FAULT;
    beq t0, t1, store_page_fault;

interrupt_handle:
#ifdef MTIMER_TEST
    # Set stime
    li t0, MIP_SSIP;
    csrc sip, t0;
    j trap_sret;
#endif
misaligned_fetch:
sbreak:
    j _abort; # no interrupt
ucall:
    jal write_tohost;
    csrr t0, sepc;
    addi t0, t0, 4;
    sd t0, 32*8(a0);
    j trap_sret;

insn_page_fault:
load_page_fault:
store_page_fault:
    csrr a0, stval;
    csrr a1, scause;
    jal fault_handle;
    j trap_sret;

