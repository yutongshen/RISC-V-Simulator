.section .text.boot
.align 2
.globl _start
_start:
    la t0, entry
    ld t0, 0(t0)
    jr t0

.align 4
entry:
    .dword 0x80000000
