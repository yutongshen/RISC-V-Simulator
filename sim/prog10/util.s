	.file	"util.c"
	.option nopic
	.text
	.align	1
	.globl	_putchar
	.hidden	_putchar
	.type	_putchar, @function
_putchar:
	addi	sp,sp,-32
	sd	s0,24(sp)
	addi	s0,sp,32
	mv	a5,a0
	sw	a5,-20(s0)
	nop
	mv	a0,a5
	ld	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	_putchar, .-_putchar
	.ident	"GCC: (GNU) 8.3.0"
