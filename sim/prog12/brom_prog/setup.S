# This progrom is runing in boot ROM, it just initialize some configuration, load code into sram and jump to sram
#include "mmap_soc.h"
#include "riscv_def.h"
#define POSTBL_PROG_ELF FLASH_BASE
#define DTB_FILE (BROM_BASE + 0x1000)

.section .text
.globl _start
.align 2
_start:
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
    
    # initialize sp
    csrr t0, mhartid;
    la sp, STACK_POINTER
    slli t0, t0, 8;
    add  sp, sp, t0;

    jal main;

    csrr a0, mhartid;
    bnez a0, 1f;

    # CPU 1~7 power on
    li t0, CLST_0_RG_CPU1_PC;
    sw zero, 0(t0);
    li t0, CLST_0_RG_CPU2_PC;
    sw zero, 0(t0);
    li t0, CLST_0_RG_CPU3_PC;
    sw zero, 0(t0);
    li t0, CLST_0_RG_CPU4_PC;
    sw zero, 0(t0);
    li t0, CLST_0_RG_CPU5_PC;
    sw zero, 0(t0);
    li t0, CLST_0_RG_CPU6_PC;
    sw zero, 0(t0);
    li t0, CLST_0_RG_CPU7_PC;
    sw zero, 0(t0);

    li t1, 0xfe;
    li t0, CLST_0_RG_PWR_REQ_SET;
    sw t1, 0(t0);

    # ELF loader
    li a0, POSTBL_PROG_ELF;
    jal elf_loader;

    # send ipi
    li t0, 1;
    li t1, CLINT_RG_MSIP;

    sw t0, 4(t1);
    sw t0, 8(t1);
    sw t0, 12(t1);
    sw t0, 16(t1);
    sw t0, 20(t1);
    sw t0, 24(t1);
    sw t0, 28(t1);

	mv a2, a0;
    j 2f;
1:
    li t0, MIP_MSIP;
    csrs mie, t0;
    /* TM_PRINT="CPU%d: Enter to WFI", a0 */
    fence;
    fence.i;
    wfi;
    csrc mip, t0;
    csrc mie, t0;
    li a2, 0x80000000;
2:
    csrr a0, mhartid;
    li a1, DTB_FILE;
    jr a2;
