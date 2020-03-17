#include "riscv_def.h"

.section .text.init
.align 2
.globl _entry
_entry:
    j handle_reset;

.align 2
trap_vector:
    csrr t0, mcause;
    li t1, CAUSE_SUPERVISOR_ECALL;
    beq t0, t1, scall;
    li t1, CAUSE_USER_ECALL;
    beq t0, t1, ucall;
    j _abort;
scall:
ucall:
    j _abort;

.align 2
.globl trap_entry
trap_entry:
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
    j trap_handle;

.align 2
.globl pop_trapframe
pop_trapframe:
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

trap_handle:
    csrr t0, scause;
    bltz t0, interrupt_handle;
    li t1, CAUSE_MISALIGNED_FETCH;
    beq t0, t1, misaligned_fetch;
    li t1, CAUSE_BREAKPOINT;
    beq t0, t1, sbreak;
    li t1, CAUSE_INSTRUCTION_PAGE_FAULT;
    beq t0, t1, insn_page_fault;
    li t1, CAUSE_LOAD_PAGE_FAULT;
    beq t0, t1, load_page_fault;
    li t1, CAUSE_STORE_PAGE_FAULT;
    beq t0, t1, store_page_fault;

interrupt_handle:
misaligned_fetch:
sbreak:
    j _abort; # no interrupt
insn_page_fault:
load_page_fault:
store_page_fault:
    csrr a0, stval;
    csrr a1, scause;
    jal fault_handle;
    csrr a0, sscratch;
    j pop_trapframe;
    

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

    # set stack point
    la sp, (STACK_TOP - TRAPFRAM_SIZE);
    csrw mscratch, sp;

    # set machine trap entry
    la t0, trap_vector;
    csrw mtvec, t0;
    csrr t0, mtvec;

    # Initialize virtual memory
    j vm_boot;


rdtime:
    li a0, MTIME_ADDR;
    lw a0, 0(a0);
    ret;

write_tohost:
    la t0, tohost;
1:  sd a0, 0(t0);
    j 1b;

_terminate:
    li a0, EXIT_CODE_PASS;
    j write_tohost;

_abort:
    li a0, EXIT_CODE_ERROR;
    j write_tohost;

.align 2
.globl user_space
user_space:
    # set stack point
    la sp, (STACK_TOP - TRAPFRAM_SIZE);
    jal main;
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

.section .data
.align 4
str:
.dword 0x40
.dword 1
.dword hello
.dword 13
hello:
.string "hello word!\n"
