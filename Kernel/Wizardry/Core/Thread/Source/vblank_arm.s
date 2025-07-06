	.INCLUDE "macros.inc"
	.INCLUDE "gba.inc"
	.SYNTAX UNIFIED

	.section .text

THUMB_FUNC_START _VBlankIntrWait
_VBlankIntrWait:
	movs r2, #0
	swi #5
	bx lr
