    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

THUMB_FUNC_START DrawItemMenuLine_hook
DrawItemMenuLine_hook:
	push {lr}
	push {r0-r3}
	movs r0, r6
	bl IsDuraItem
	cmp r0, #1
	pop {r0-r3}
	beq 1f
	bl PutNumberOrBlank
1:
	pop {pc}
