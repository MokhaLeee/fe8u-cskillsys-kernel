	.INCLUDE "macros.inc"
	.INCLUDE "gba.inc"
	.SYNTAX UNIFIED

	.section .text

THUMB_FUNC_START resume_task
resume_task:
	bx lr

THUMB_FUNC_START exec_task
exec_task:
	bx lr

THUMB_FUNC_START break_task
break_task:
	bx lr
