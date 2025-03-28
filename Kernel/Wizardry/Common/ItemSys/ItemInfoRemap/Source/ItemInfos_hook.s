    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

THUMB_FUNC_START DrawItemMenuLine_hook
DrawItemMenuLine_hook:
	@ 0x80168B4
	push {r0-r3}
	movs r0, r6
	bl IsDuraItem
	cmp r0, #1
	pop {r0-r3}
	beq 1f
	bl PutNumberOrBlank
1:
	movs r0, r6
	bl GetItemIconId
	movs r1, r0
	ldr r0, =0x80168CB
	bx r0
