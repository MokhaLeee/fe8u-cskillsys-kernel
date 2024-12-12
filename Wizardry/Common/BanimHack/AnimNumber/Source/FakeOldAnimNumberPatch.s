.INCLUDE "macros.inc"

.section .text

/**
 * This is just a jump-back from 0x552D0 (in side function: StartBattleAnimHitEffects)
 * As we need a fack on AnimNumbers patch.
 */
THUMB_FUNC_START FakeOldAnimNumberPatchJumpBack1
FakeOldAnimNumberPatchJumpBack1:
	movs r8, r1
	cmp r4, #0
	beq 1f

	cmp r4, #1
	bl 0x80552D8

1:
	bl 0x80552E4

/**
 * This is just a jump-back from 0x52304 (in side function: NewEfxHpBar)
 * As we need a fack on AnimNumbers patch.
 */
THUMB_FUNC_START FakeOldAnimNumberPatchJumpBack2
FakeOldAnimNumberPatchJumpBack2:
	push {r4 - r6, lr}
	mov r4, r0
	bl GetAnimPosition
	bl 0x805230C
