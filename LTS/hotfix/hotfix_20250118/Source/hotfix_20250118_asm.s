.thumb

.include "macros.inc"

THUMB_FUNC_START UnitAutolevelCore_Fix
UnitAutolevelCore_Fix: @ 0x8017FC4
	push {r4, r5, lr}
	mov r4, r0
	mov r5, r2
	cmp r5, #0
	bne 1f
	pop {r4, r5, pc}

1:
	@ mag
	mov r0, r4
	mov r2, r5
	bl UnitAutolevelCore_Mag

	@ vanilla
	ldr r0, [r4, #4]
	ldrb r0, [r0, #0x1B]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	ldr r1, =0x08017FD7
	bx r1

THUMB_FUNC_START CheckVantageOrder_Fix
CheckVantageOrder_Fix: @ 0x8BAB600
	@ r6  is the actor
	@ r10 is the target
	mov r0, #0x48
	mov r1, r10
	ldrh r0, [r1, r0]
	cmp r0, #0
	bne 1f
	ldr r0, =0x8BAB821
	bx r0

1:
	mov r1, #0x73   @ 0x8BAB5FE
	mov r0, r10     @ 0x8BAB600
	ldr r3, [r4]    @ 0x8BAB602
	ldr r2, =0x8BAB609
	mov lr, r2
	bx r3
