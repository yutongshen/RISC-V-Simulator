# This progrom is runing in boot ROM, it just initialize some configuration, load code into sram and jump to sram
#include "mmap_soc.h"
#include "riscv_def.h"
#define PROG_ELF FLASH_BASE
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
    la sp, STACK_POINTER
    /* TM_PRINT="**** BOOTROM PROG START ****" */

    li a0, MSTATUS_FS;
    csrs mstatus, a0;
    jal main;


    # ELF loader
    li a0, PROG_ELF;
    jal elf_loader;
    mv a2, a0;
    csrr a0, mhartid;
    li a1, DTB_FILE;
    /* TM_PRINT="Load code successfully! sp=0x%08lx, a0=0x%08lx, a1=0x%08lx, a2=0x%08lx", sp, a0, a1, a2 */
    /* TM_PRINT="**** BOOTROM PROG END ****" */
    jr a2;


