	.INCLUDE "macros.inc"
	.SYNTAX UNIFIED

	.section .text

ARM_FUNC_START systick_isr
systick_isr:
	bx lr
