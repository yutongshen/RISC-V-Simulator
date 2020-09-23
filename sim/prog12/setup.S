#include "riscv_def.h"
// #define MTIMER_TEST
#define PLIC_TEST

.section .text.init
.align 2
.globl _entry
_entry:
    j handle_reset;

# M-mode Trap Handle

.align 2
.globl trap_entry_m
trap_entry_m:
    # supervisor trap
    # get trapframe pointer (save a0 into mcratch)
    csrrw a0, mscratch, a0;

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

    # load stack point and save trapframe point into mcratch
    csrrw t0, mscratch, a0;
    sd t0, 9*8(a0);

    # push status, epc, tval, cause
    csrr t0, mstatus;
    sd t0, 31*8(a0);
    csrr t0, mepc;
    sd t0, 32*8(a0);
    csrr t0, mtval;
    sd t0, 33*8(a0);
    csrr t0, mcause;
    sd t0, 34*8(a0);

    csrr sp, mscratch;
    j trap_handler_m;

.align 2
.globl pop_trapframe_m
pop_trapframe_m:
    # a0 need to save trapframe pointer
    # pop epc and reg x1~x31 from trapframe
    ld t0, 32*8(a0);
    csrw mepc, t0;
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
    mret;

trap_mret:
    csrr a0, mscratch;
    j pop_trapframe_m;
    
.align 2
trap_handler_m:
    csrr sp, mscratch;
    csrr t0, mcause;
    bltz t0, interrupt_handle_m;
    li t1, CAUSE_SUPERVISOR_ECALL;
    beq t0, t1, scall;
    j _abort;

interrupt_handle_m:
#ifdef MTIMER_TEST
    # Set timer interval
    li a0, 1000;
    jal set_timer_interval;

    # Set stime
    li t0, MIP_SSIP;
    csrs mip, t0;
    j trap_mret;
#endif

scall:
    j _abort;

# S-mode Trap Handle

.align 2
.globl trap_entry_s
trap_entry_s:
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
    j trap_handler_s;

.align 2
.globl pop_trapframe_s
pop_trapframe_s:
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

trap_handler_s:
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

trap_sret:
    csrr a0, sscratch;
    j pop_trapframe_s;
    

handle_reset:
    # initialize register
    li x1, 0;
    li x2, 0;
    li x3, 0;
    li x4, 0;
    li x5, 0;
    li x6, 0;
    li x7, 0;
    li x8, 0;
    li x9, 0;
    li x10, 0;
    li x11, 0;
    li x12, 0;
    li x13, 0;
    li x14, 0;
    li x15, 0;
    li x16, 0;
    li x17, 0;
    li x18, 0;
    li x19, 0;
    li x20, 0;
    li x21, 0;
    li x22, 0;
    li x23, 0;
    li x24, 0;
    li x25, 0;
    li x26, 0;
    li x27, 0;
    li x28, 0;
    li x29, 0;
    li x30, 0;
    li x31, 0;

    # set fs
    li t0, MSTATUS_FS;
    csrs mstatus, t0;

    # set stack point
    la sp, (STACK_TOP - TRAPFRAM_SIZE);
    csrw mscratch, sp;

    # set machine trap entry
    la t0, trap_entry_m;
    csrw mtvec, t0;
    csrr t0, mtvec;

#ifdef MTIMER_TEST
    # Set timer
    li a0, 5000;
    jal set_timer_interval;

    # Enable interrupt
    li a0, MSTATUS_MPIE;
    csrs mstatus, a0;

    # Enable mtimer
    li a0, MIP_MTIP;
    csrs mie, a0;
rtime:
    # j rtime;

#endif

#ifdef PLIC_TEST
    # Set Enable
    li a1, (PLIC_BASE + RG_ENABLE);
    li a0, -1U;
    sw a0, 0(a1);

    # Set Priority
    li a1, (PLIC_BASE + RG_PRIOR);
    li a0, 0x100;
    sw a0, 0x14(a1);
    sw a0, 0x00(a1);
    sw a0, 0x04(a1);
    sw a0, 0x08(a1);
    sw a0, 0x0c(a1);
    sw a0, 0x10(a1);
    sw a0, 0x18(a1);
    sw a0, 0x1c(a1);
    sw a0, 0x20(a1);
    sw a0, 0x24(a1);
    sw a0, 0x28(a1);
    sw a0, 0x2c(a1);
    sw a0, 0x30(a1);
    sw a0, 0x34(a1);
    sw a0, 0x38(a1);
    sw a0, 0x3c(a1);
    sw a0, 0x40(a1);
    sw a0, 0x44(a1);
    sw a0, 0x48(a1);
    sw a0, 0x4c(a1);
    sw a0, 0x50(a1);
    sw a0, 0x54(a1);
    sw a0, 0x58(a1);
    sw a0, 0x5c(a1);
    sw a0, 0x60(a1);
    sw a0, 0x64(a1);
    sw a0, 0x68(a1);
    sw a0, 0x6c(a1);
    sw a0, 0x70(a1);
    sw a0, 0x74(a1);
    sw a0, 0x78(a1);
    sw a0, 0x7c(a1);

    # Enable interrupt
    li a0, MSTATUS_MPIE;
    csrs mstatus, a0;

    # Enable EINT
    li a0, MIP_MEIP;
    csrs mie, a0;

    # Set Priority
    li a1, (PLIC_BASE + RG_INTID);
    lw a0, 0(a1);

    nop;
    nop;
    nop;
    sw a0, 0(a1);
#endif

    # Initialize virtual memory
    j vm_boot;

set_timer_interval:
    li a1, (TIMER_BASE + RG_TIME);
    lw a1, 0(a1);
    add a0, a0, a1;
    li a1, (TIMER_BASE + RG_TIMECMP);
    sw a0, 0(a1);
    ret;

get_time:
    li a0, (TIMER_BASE + RG_TIME);
    lw a0, 0(a0);
    ret;

_terminate:
    li a0, EXIT_CODE_PASS;
    j _write_tohost;

_abort:
    li a0, EXIT_CODE_ERROR;
    j _write_tohost;

_write_tohost:
    la t0, tohost;
1:  sd a0, 0(t0);
    j 1b;

.align 2
.globl user_space
user_space:
    # set stack point
    la sp, (STACK_TOP - TRAPFRAM_SIZE);
    jal main;
    li a0, 1;
    ecall;


.section .tohost
.align 6
.globl tohost
tohost:
    .word 0
.align 6
.globl fromhost
fromhost:
    .word 0
