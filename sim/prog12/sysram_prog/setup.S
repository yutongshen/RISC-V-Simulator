# This progrom is runing in boot ROM, it just initialize some configuration, load code into sram and jump to sram
#include "mmap_soc.h"
#include "riscv_def.h"
#define VM_ENABLE
#define KERNEL_PROG_ELF (FLASH_BASE + 0x10000)

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
    la sp, STACK_POINTER

    # set fs
    li t0, MSTATUS_FS;
    csrs mstatus, t0;

    jal main;

    # ELF loader
    li a0, KERNEL_PROG_ELF;
    jal elf_loader;

#ifdef VM_ENABLE
    j vm_boot;
#else
    li a0, DISK_1_BASE;
    jr a0;
#endif
