#define EXIT_CODE 0x01234567
#define MTIMECMP_ADDR 0xc0000800
#define MTIME_ADDR 0xc0000ff8
#define TOHOST 0x80001000
#define FROMHOST 0x80001040

#define PGSIZE 4096
#define TRAPFRAM_SIZE (36 * 8)
#define STACK_TOP (_end + PGSIZE)

.section .text.init
.align 2
.globl _entry
_entry:
    j handle_reset

trap_vector:
    # machine trap is not used
    j terminate

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

    j terminate


rdtime:
    li a0, MTIME_ADDR
    lw a0, 0(a0) 
    ret

tohost:
    li t0, TOHOST
    sd a0, 0(t0)
    j tohost

terminate:
    li a0, EXIT_CODE
    j tohost
