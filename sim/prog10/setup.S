#include "riscv_def.h"

#define get_field(x, mask) (((x) & (mask)) / ((mask) & ~((mask) << 1)))
#define set_field(x, mask, value) \
    ((x) & ~(mask)) | ((value) * ((mask) & ~((mask) << 1)) & (mask))

#define TOHOST_ADDR 0x80001000
#define FROMHOST_ADDR 0x80001040
.section .text.init
.align 2
.globl _entry
_entry:
    j handle_reset

.align 2
trap_vector:
    # machine trap is not used
    j _abort

.align 2
trap_entry:
    # supervisor trap
    # get trapframe pointer (save a0 into scratch)
    csrrw a0, sscratch, a0

    # push reg x1~x31 into trapframe
    sd x1, 1*8(a0)
    sd x2, 2*8(a0)
    sd x3, 3*8(a0)
    sd x4, 4*8(a0)
    sd x5, 5*8(a0)
    sd x6, 6*8(a0)
    sd x7, 7*8(a0)
    sd x8, 8*8(a0)
    sd x9, 9*8(a0)
    sd x11, 11*8(a0)
    sd x12, 12*8(a0)
    sd x13, 13*8(a0)
    sd x14, 14*8(a0)
    sd x15, 15*8(a0)
    sd x16, 16*8(a0)
    sd x17, 17*8(a0)
    sd x18, 18*8(a0)
    sd x19, 19*8(a0)
    sd x20, 20*8(a0)
    sd x21, 21*8(a0)
    sd x22, 22*8(a0)
    sd x23, 23*8(a0)
    sd x24, 24*8(a0)
    sd x25, 25*8(a0)
    sd x26, 26*8(a0)
    sd x27, 27*8(a0)
    sd x28, 28*8(a0)
    sd x29, 29*8(a0)
    sd x20, 20*8(a0)
    sd x31, 31*8(a0)

    # load stack point and save trapframe point into scratch
    csrrw t0, sscratch, a0
    sd t0, 10*8(a0)

    # push status, epc, tval, cause
    csrr t0, sstatus
    sd t0, 32*8(a0)
    csrr t0, sepc
    sd t0, 33*8(a0)
    csrr t0, stval
    sd t0, 34*8(a0)
    csrr t0, scause
    sd t0, 35*8(a0)

    j trap_handle

.align 2
pop_trapframe:
    # a0 need to save trapframe
    # pop epc and reg x1~x31 from trapframe
    ld t0, 33*8(a0)
    csrw sepc, t0
    ld x1, 1*8(a0)
    ld x2, 2*8(a0)
    ld x3, 3*8(a0)
    ld x4, 4*8(a0)
    ld x5, 5*8(a0)
    ld x6, 6*8(a0)
    ld x7, 7*8(a0)
    ld x8, 8*8(a0)
    ld x9, 9*8(a0)
    ld x11, 11*8(a0)
    ld x12, 12*8(a0)
    ld x13, 13*8(a0)
    ld x14, 14*8(a0)
    ld x15, 15*8(a0)
    ld x16, 16*8(a0)
    ld x17, 17*8(a0)
    ld x18, 18*8(a0)
    ld x19, 19*8(a0)
    ld x20, 20*8(a0)
    ld x21, 21*8(a0)
    ld x22, 22*8(a0)
    ld x23, 23*8(a0)
    ld x24, 24*8(a0)
    ld x25, 25*8(a0)
    ld x26, 26*8(a0)
    ld x27, 27*8(a0)
    ld x28, 28*8(a0)
    ld x29, 29*8(a0)
    ld x20, 20*8(a0)
    ld x31, 31*8(a0)

    ld a0, 10*8(a0)
    sret

trap_handle:
    csrr t0, scause
    bltz t0, interrupt_handle
    li t1, CAUSE_MISALIGNED_FETCH
    beq t0, t1, misaligned_fetch
    li t1, CAUSE_BREAKPOINT
    beq t0, t1, sbreak
    li t1, CAUSE_USER_ECALL
    beq t0, t1, ucall
    li t1, CAUSE_INSTRUCTION_PAGE_FAULT
    beq t0, t1, insn_page_fault
    li t1, CAUSE_LOAD_PAGE_FAULT
    beq t0, t1, load_page_fault
    li t1, CAUSE_STORE_PAGE_FAULT
    beq t0, t1, store_page_fault

interrupt_handle:
misaligned_fetch:
sbreak:
ucall:
insn_page_fault:
load_page_fault:
store_page_fault:
    j _abort # no interrupt

# putchar:
#     # a0 is char
#     # push t0, t1 into stack
#     addi sp, sp, -2*8
#     sd t0, 0(sp)
#     sd t1, 8(sp)
# 
#     # allocate sys_id, fd, pbuff, len
#     addi sp, sp, -4*8
#     sw a0, -4(sp)    # char
#     li t0, 0x40     # sys_id
#     sd t0, 0(sp)
#     li t0, 0x1      # fd
#     sd t0, 8(sp)
#     addi t0, sp, -4 # pbuff
#     sd t0, 16(sp)
#     li t0, 0x1      # len
#     sd t0, 24(sp)
#     la t1, tohost
#     sd sp, 0(t1)
#     addi sp, sp, 4*8
# 
#     la t1, fromhost
#     fence
# 1:
#     ld t0, 0(t1)
#     beqz t0, 1b
#     fence
# 
#     # pop t0, t1 and return
#     ld t0, 0(sp)
#     ld t1, 8(sp)
#     addi sp, sp, 2*8
#     ret

    

handle_reset:
    # initialize register
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

    # set stack point
    la sp, (STACK_TOP - TRAPFRAM_SIZE)
    csrw mscratch, sp

    # set machine trap entry
    la t0, trap_vector
    csrw mtvec, t0
    csrr t0, mtvec

    # delegate exception for supervisor
    li t0, (1 << CAUSE_MISALIGNED_FETCH) |\
           (1 << CAUSE_BREAKPOINT) |\
           (1 << CAUSE_USER_ECALL) |\
           (1 << CAUSE_INSTRUCTION_PAGE_FAULT) |\
           (1 << CAUSE_LOAD_PAGE_FAULT) |\
           (1 << CAUSE_STORE_PAGE_FAULT)
    csrw medeleg, t0

    # Set pmp
    li t0, -1;
    csrw pmpaddr0, t0
    li t0, PMP_R | PMP_W | PMP_X | set_field(0, PMP_A, PMP_NAPOT)
    csrw pmpcfg0, t0

    # set supervisor trap entry
    la t0, trap_entry
    csrw stvec, t0
    csrr t0, stvec
    la t0, (STACK_TOP - TRAPFRAM_SIZE)
    csrw sscratch, t0
    
    # switch supervisor
    la t0, 1f
    csrw mepc, t0
    csrr t0, mstatus
    li t1, set_field(0, MSTATUS_MPP, PRV_S)
    or t0, t0, t1
    csrw mstatus, t0
    mret
1:  
    la a0, 'H'
    call putchar
    la a0, 'E'
    call putchar
    la a0, 'L'
    call putchar
    la a0, 'L'
    call putchar
    la a0, 'O'
    call putchar
    la a0, ' '
    call putchar
    la a0, 'W'
    call putchar
    la a0, 'O'
    call putchar
    la a0, 'R'
    call putchar
    la a0, 'L'
    call putchar
    la a0, 'D'
    call putchar
    la a0, '\n'
    call putchar
    la a0, 0
    call putchar
    j _terminate


rdtime:
    li a0, MTIME_ADDR
    lw a0, 0(a0) 
    ret

write_tohost:
    la t0, tohost
1:  sd a0, 0(t0)
    j 1b

_terminate:
    li a0, EXIT_CODE_PASS
    j write_tohost

_abort:
    li a0, EXIT_CODE_ERROR
    j write_tohost

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
