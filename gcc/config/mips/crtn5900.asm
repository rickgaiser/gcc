/* 4 slots for argument spill area.  1 for cpreturn, 1 for stack.
   Return spill offset of 80.  */

	.section .init,"ax",@progbits
	lq      $31,80($sp)
	addu	$sp,$sp,96
	j	$31

	.section .fini,"ax",@progbits
	lq	$31,80($sp)
	addu	$sp,$sp,96
	j	$31
