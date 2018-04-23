/* 4 slots for argument spill area.  1 for cpreturn, 1 for stack.
   Return spill offset of 80.  */

	.section .init,"ax",@progbits
	.globl	_init
	.type	_init,@function
_init:
	addu    $sp,$sp,-96
	sq      $31,80($sp)

	.section .fini,"ax",@progbits
	.globl	_fini
	.type	_fini,@function
_fini:
	addu    $sp,$sp,-96
	sq      $31,80($sp)
