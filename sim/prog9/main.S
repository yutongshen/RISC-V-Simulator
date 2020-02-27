# 1 "rv64mi/csr.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "rv64mi/csr.S"
# See LICENSE for license details.

# 1 "/home/disk4T/yutongshen/riscv-tests/isa/../env/p/riscv_test.h" 1





# 1 "/home/disk4T/yutongshen/riscv-tests/isa/../env/p/../encoding.h" 1
# 7 "/home/disk4T/yutongshen/riscv-tests/isa/../env/p/riscv_test.h" 2
# 4 "rv64mi/csr.S" 2




# 1 "rv64mi/../rv64si/csr.S" 1
# See LICENSE for license details.

#*****************************************************************************
# csr.S
#-----------------------------------------------------------------------------

# Test CSRRx and CSRRxI instructions.



# 1 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h" 1






#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------
# 20 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
# We use a macro hack to simpify code generation for various numbers
# of bubble cycles.
# 36 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# RV64UI MACROS
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Tests for instructions with immediate operand
#-----------------------------------------------------------------------
# 92 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# Tests for an instruction with register operands
#-----------------------------------------------------------------------
# 120 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# Tests for an instruction with register-register operands
#-----------------------------------------------------------------------
# 214 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# Test memory instructions
#-----------------------------------------------------------------------
# 340 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# Test jump instructions
#-----------------------------------------------------------------------
# 369 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# RV64UF MACROS
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Tests floating-point instructions
#-----------------------------------------------------------------------
# 631 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# Pass and fail code (assumes test num is in gp)
#-----------------------------------------------------------------------
# 643 "/home/disk4T/yutongshen/riscv-tests/isa/macros/scalar/test_macros.h"
#-----------------------------------------------------------------------
# Test data section
#-----------------------------------------------------------------------
# 12 "rv64mi/../rv64si/csr.S" 2

.macro init; li a0, 0x00001800; csrs mstatus, a0;; .endm
.section .text.init; .align 6; .weak stvec_handler; .weak mtvec_handler; .globl _start; _start: j reset_vector; .align 2; trap_vector: csrr t5, mcause; li t6, 0x8; beq t5, t6, write_tohost; li t6, 0x9; beq t5, t6, write_tohost; li t6, 0xb; beq t5, t6, write_tohost; la t5, mtvec_handler; beqz t5, 1f; jr t5; 1: csrr t5, mcause; bgez t5, handle_exception; j other_exception; handle_exception: other_exception: 1: ori gp, gp, 1337; write_tohost: sw gp, tohost, t5; j write_tohost; reset_vector: csrr a0, mhartid; 1: bnez a0, 1b; la t0, 1f; csrw mtvec, t0; csrwi sptbr, 0; .align 2; 1:; la t0, 1f; csrw mtvec, t0; li t0, (1 << (31 + (64 / 64) * (53 - 31))) - 1; csrw pmpaddr0, t0; li t0, 0x18 | 0x01 | 0x02 | 0x04; csrw pmpcfg0, t0; .align 2; 1:; la t0, 1f; csrw mtvec, t0; csrwi medeleg, 0; csrwi mideleg, 0; csrwi mie, 0; .align 2; 1:; li gp, 0; la t0, trap_vector; csrw mtvec, t0; li a0, 1; slli a0, a0, 31; bgez a0, 1f; fence; li gp, 1; li a7, 93; li a0, 0; ecall; 1:; la t0, stvec_handler; beqz t0, 1f; csrw stvec, t0; li t0, (1 << 0xd) | (1 << 0xf) | (1 << 0xc) | (1 << 0x0) | (1 << 0x8) | (1 << 0x3); csrw medeleg, t0; csrr t1, medeleg; bne t0, t1, other_exception; 1: csrwi mstatus, 0; init; ; ; la t0, 1f; csrw mepc, t0; csrr a0, mhartid; mret; 1:
# 27 "rv64mi/../rv64si/csr.S"
  # For RV64, make sure UXL encodes RV64. (UXL does not exist for RV32.)

  # If running in M mode, use mstatus.MPP to check existence of U mode.
  # Otherwise, if in S mode, then U mode must exist and we don't need to check.

  li t0, 0x00001800
  csrc mstatus, t0
  csrr t1, mstatus
  and t0, t0, t1
  bnez t0, 1f

  # If U mode is present, UXL should be 2 (XLEN = 64-bit)
  test_13: csrr a0, mstatus; li a1, 0x0000000300000000; and a0, a0, a1; li x29, ((0x0000000300000000 & (0x0000000300000000 << 1)) & ((1 << (64 - 1) << 1) - 1)); li gp, 13; bne a0, x29, fail;

  j 2f
1:
  # If U mode is not present, UXL should be 0
  test_14: csrr a0, mstatus; li a1, 0x0000000300000000; and a0, a0, a1; li x29, ((0) & ((1 << (64 - 1) << 1) - 1)); li gp, 14; bne a0, x29, fail;
2:



  test_15: csrrwi a0, mscratch, 0; csrrwi a0, mscratch, 0xF; li x29, ((0) & ((1 << (64 - 1) << 1) - 1)); li gp, 15; bne a0, x29, fail;;
  test_16: csrw mscratch, zero; csrr a0, mscratch; li x29, ((0) & ((1 << (64 - 1) << 1) - 1)); li gp, 16; bne a0, x29, fail;;

  csrwi mscratch, 3
  test_2: csrr a0, mscratch; li x29, ((3) & ((1 << (64 - 1) << 1) - 1)); li gp, 2; bne a0, x29, fail;;
  test_3: csrrci a1, mscratch, 1; li x29, ((3) & ((1 << (64 - 1) << 1) - 1)); li gp, 3; bne a1, x29, fail;;
  test_4: csrrsi a2, mscratch, 4; li x29, ((2) & ((1 << (64 - 1) << 1) - 1)); li gp, 4; bne a2, x29, fail;;
  test_5: csrrwi a3, mscratch, 2; li x29, ((6) & ((1 << (64 - 1) << 1) - 1)); li gp, 5; bne a3, x29, fail;;
  test_6: li a0, 0xbad1dea; csrrw a1, mscratch, a0; li x29, ((2) & ((1 << (64 - 1) << 1) - 1)); li gp, 6; bne a1, x29, fail;;
  test_7: li a0, 0x0001dea; csrrc a0, mscratch, a0; li x29, ((0xbad1dea) & ((1 << (64 - 1) << 1) - 1)); li gp, 7; bne a0, x29, fail;;
  test_8: li a0, 0x000beef; csrrs a0, mscratch, a0; li x29, ((0xbad0000) & ((1 << (64 - 1) << 1) - 1)); li gp, 8; bne a0, x29, fail;;
  test_9: csrr a0, mscratch; li x29, ((0xbadbeef) & ((1 << (64 - 1) << 1) - 1)); li gp, 9; bne a0, x29, fail;;


  # Is F extension present?
  csrr a0, misa
  andi a0, a0, (1 << ('F' - 'A'))
  beqz a0, 1f
  # If so, make sure FP stores have no effect when mstatus.FS is off.
  li a1, 0x00006000
  csrs mstatus, a1

  fmv.s.x f0, x0
  csrc mstatus, a1
  la a1, fsw_data
  test_10: fsw f0, (a1); lw a0, (a1); li x29, ((1) & ((1 << (64 - 1) << 1) - 1)); li gp, 10; bne a0, x29, fail;;




1:

  # Figure out if 'U' is set in misa
  csrr a0, misa # a0 = csr(misa)
  srli a0, a0, 20 # a0 = a0 >> 20
  andi a0, a0, 1 # a0 = a0 & 1
  beqz a0, finish # if no user mode, skip the rest of these checks


  # jump to user land
  li t0, 0x00001800
  csrc mstatus, t0
  la t0, 1f
  csrw mepc, t0
  mret
  1:

  # Make sure writing the cycle counter causes an exception.
  # Don't run in supervisor, as we don't delegate illegal instruction traps.

  test_11: li a0, 255; csrrw a0, cycle, x0; li x29, ((255) & ((1 << (64 - 1) << 1) - 1)); li gp, 11; bne a0, x29, fail;;


  # Make sure reading status in user mode causes an exception.
  # Don't run in supervisor, as we don't delegate illegal instruction traps.

  test_12: li a0, 255; csrr a0, mstatus; li x29, ((255) & ((1 << (64 - 1) << 1) - 1)); li gp, 12; bne a0, x29, fail;




finish:
  fence; li gp, 1; li a7, 93; li a0, 0; ecall

  # We should only fall through to this if scall failed.
  bne x0, gp, pass; fail: fence; 1: beqz gp, 1b; sll gp, gp, 1; or gp, gp, 1; li a7, 93; addi a0, gp, 0; ecall; pass: fence; li gp, 1; li a7, 93; li a0, 0; ecall

  .align 2
  .global mtvec_handler
mtvec_handler:
  # Trapping on tests 10-12 is good news.
  # Note that since the test didn't complete, TESTNUM is smaller by 1.
  li t0, 9
  bltu gp, t0, 1f
  li t0, 11
  bleu gp, t0, privileged
1:

  # catch fence; li gp, 1; li a7, 93; li a0, 0; ecall and kick it up to M-mode
  csrr t0, mcause
  li t1, 0x8
  bne t0, t1, fail
  fence; li gp, 1; li a7, 93; li a0, 0; ecall

privileged:
  # Make sure mcause indicates a lack of privilege.
  csrr t0, mcause
  li t1, 0x2
  bne t0, t1, fail
  # Return to user mode, but skip the trapping instruction.
  csrr t0, mepc
  addi t0, t0, 4
  csrw mepc, t0
  mret

unimp

  .data
 .pushsection .tohost,"aw",@progbits; .align 6; .global tohost; tohost: .dword 0; .align 6; .global fromhost; fromhost: .dword 0; .popsection; .align 4; .global begin_signature; begin_signature:

fsw_data: .word 1

.align 4; .global end_signature; end_signature:
# 8 "rv64mi/csr.S" 2
