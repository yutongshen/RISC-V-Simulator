#include "mmap_soc.h"

# Define constants
.section .text
.align 2
.globl _start
_start:
    li x1, 0
    li x2, 0
    li x3, 0
    li x4, 0
    li x5, 0
    li x6, 0
    li x7, 0
    li x8, 0
    li x9, 0
    li x10, 0
    li x11, 0
    li x12, 0
    li x13, 0
    li x14, 0
    li x15, 0
    li x16, 0
    li x17, 0
    li x18, 0
    li x19, 0
    li x20, 0
    li x21, 0
    li x22, 0
    li x23, 0
    li x24, 0
    li x25, 0
    li x26, 0
    li x27, 0
    li x28, 0
    li x29, 0
    li x30, 0
    li x31, 0

    /* initialize global pointer */
    la gp, _gp

init_stack:
    /* set stack pointer */
    csrr a0, mhartid
    slli a1, a0, 12
    la sp, _stack
    add sp, sp, a1

    /* Core 1 */
    li a1, 1
    beq a0, a1, sensor_handle

    /* Core 2~7 */
    bnez a0, _wfi

    /* Core 1 wakeup */
    # li a0, CLINT_RG_MSIP
    # li a1, 1
    # sw a1, 4(a0)
    li a0, CLST_0_RG_CPU1_PC
    li a1, 0x80000000
    sd a1, 0(a0)
    li a0, CLST_0_RG_PWR_REQ_SET
    li a1, 0x2
    sd a1, 0(a0)

    /* set mtvec */
    la a0, trap_entry
    csrw mtvec, a0

init_bss:
    /* init bss section */
    la a0, __bss_start
    la a1, __bss_end-4 /* section end is actually the start of the next section */
    li a2, 0x0
    jal fill_block

init_sbss:
    /* init bss section */
    la a0, __sbss_start
    la a1, __sbss_end-4 /* section end is actually the start of the next section */
    li a2, 0x0
    jal fill_block

write_stack_pattern:
    /* init stack section */
    la a0, _stack_end  /* note the stack grows from top to bottom */
    la a1, __stack-4   /* section end is actually the start of the next section */
    li a2, 0x0
    jal fill_block

write_test_pattern:
    la a0, _test_start
    la a1, _test_end-4
    li a2, 0x0
    jal fill_block

SystemInit:
    jal main
    jal ans_cmp;

SystemExit:
    /* End simulation */
    slli a0, a0, 16;
    beqz a0, 1f;
    li a1, 0x3333;
    or a0, a0, a1;
    j 2f;
1:
    li a1, 0x5555;
    or a0, a0, a1;
2:
    li a1, FINISHER_RG_FINISH;
    sw a0, 0(a1);
dead_loop:
    j dead_loop

/* Fills memory blocks */
fill_block:
    bgtu a0, a1, fb_end;
    sw a2, 0(a0);
    addi a0, a0, 4;
    j fill_block;
fb_end:
    ret;

_wfi:
    fence
    wfi
