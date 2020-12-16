#include "riscv_def.h"
#include "mmap_soc.h"
// #define MTIMER_TEST
// #define PLIC_TEST
#define CPU_NUM 1
#define STACK_TOP _end + PGSIZE

.section .text.init
.align 2
.globl _start
_start:
    j handle_reset;

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
    csrw sscratch, sp;

    # set machine trap entry
    la t0, strap_entry;
    csrw stvec, t0;

# #ifdef MTIMER_TEST
#     # Set timer
#     li a0, 5000;
#     jal set_timer_interval;
# 
#     # Enable interrupt
#     li a0, MSTATUS_MPIE;
#     csrs mstatus, a0;
# 
#     # Enable mtimer
#     li a0, MIP_MTIP;
#     csrs mie, a0;
# rtime:
#     # j rtime;
# 
# #endif
# 
# #ifdef PLIC_TEST
#     # Set Enable
#     li a1, PLIC_RG_ENABLE;
#     li a0, -1U;
#     sw a0, 0(a1);
# 
#     # Set Priority
#     li a1, PLIC_RG_PRIOR;
#     li a0, 0x100;
#     sw a0, 0x14(a1);
#     sw a0, 0x00(a1);
#     sw a0, 0x04(a1);
#     sw a0, 0x08(a1);
#     sw a0, 0x0c(a1);
#     sw a0, 0x10(a1);
#     sw a0, 0x18(a1);
#     sw a0, 0x1c(a1);
#     sw a0, 0x20(a1);
#     sw a0, 0x24(a1);
#     sw a0, 0x28(a1);
#     sw a0, 0x2c(a1);
#     sw a0, 0x30(a1);
#     sw a0, 0x34(a1);
#     sw a0, 0x38(a1);
#     sw a0, 0x3c(a1);
#     sw a0, 0x40(a1);
#     sw a0, 0x44(a1);
#     sw a0, 0x48(a1);
#     sw a0, 0x4c(a1);
#     sw a0, 0x50(a1);
#     sw a0, 0x54(a1);
#     sw a0, 0x58(a1);
#     sw a0, 0x5c(a1);
#     sw a0, 0x60(a1);
#     sw a0, 0x64(a1);
#     sw a0, 0x68(a1);
#     sw a0, 0x6c(a1);
#     sw a0, 0x70(a1);
#     sw a0, 0x74(a1);
#     sw a0, 0x78(a1);
#     sw a0, 0x7c(a1);
# 
#     # Enable interrupt
#     li a0, MSTATUS_MPIE;
#     csrs mstatus, a0;
# 
#     # Enable EINT
#     li a0, MIP_MEIP;
#     csrs mie, a0;
# 
#     # Set Priority
#     li a1, PLIC_RG_INTID;
#     lw a0, 0(a1);
# 
#     sw a0, 0(a1);
# #endif
# 
#if (CPU_NUM > 1)
    li a0, 1;
    jal set_cpu_power_on;
#endif

#if (CPU_NUM > 2)
    li a0, 2;
    jal set_cpu_power_on;
#endif

#if (CPU_NUM > 3)
    li a0, 3;
    jal set_cpu_power_on;
#endif

#if (CPU_NUM > 4)
    li a0, 4;
    jal set_cpu_power_on;
#endif

#if (CPU_NUM > 5)
    li a0, 5;
    jal set_cpu_power_on;
#endif

#if (CPU_NUM > 6)
    li a0, 6;
    jal set_cpu_power_on;
#endif

#if (CPU_NUM > 7)
    li a0, 7;
    jal set_cpu_power_on;
#endif

#     csrr a0, mhartid;
#     bnez a0, _wfi;

	# Set page table
	jal set_pagetbl;
    ecall;


set_timer_interval:
    li a1, CLINT_RG_TIME;
    lw a1, 0(a1);
    add a0, a0, a1;
    li a1, CLINT_RG_TIMECMP;
    sw a0, 0(a1);
    ret;

get_time:
    li a0, CLINT_RG_TIME;
    lw a0, 0(a0);
    ret;

set_cpu_power_on:
    li a1, 1;
    sll a0, a1, a0;
    li a1, CLST_0_RG_PWR_REQ;
    sw a0, 0(a1);
    ret;

.globl _terminate
_terminate:
    li a0, EXIT_CODE_PASS;
    j _write_tohost;

.globl _wfi
_wfi:
    wfi;

.globl _abort
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
    # jal main;
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
