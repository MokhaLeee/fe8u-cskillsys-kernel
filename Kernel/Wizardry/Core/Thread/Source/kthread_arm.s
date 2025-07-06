	.INCLUDE "macros.inc"
	.INCLUDE "gba.inc"
	.INCLUDE "kthread.inc"
	.SYNTAX UNIFIED

	.section .text

THUMB_FUNC_START start_sub_thread
start_sub_thread:
	bx pc
	.arm
	.align 2, 0
start_sub_thread_arm:
	push {r4-r11, lr}
	add lr, lr, #4
	push {r0-r3, r12, lr}

	ldr r0, =gThreadInfo

	# save stack pointer
	str sp, [r0, #oThreadInfo_main_thread_sp]
	ldr sp, =SUB_THREAD_STACK_BASE

	# Add a mark for IRQ handler
	mov r1, #DEFAULT_SUBTHREAD_RUNNING_MODE
	strb r1, [r0, #oThreadInfo_sub_thread_running]

	# jump to thread
	ldr r0, [r0, oThreadInfo_func]
	adr lr, start_sub_thread_return
	bx r0

start_sub_thread_return:
	ldr r0, =gThreadInfo
	ldr sp, [r0, #oThreadInfo_main_thread_sp]
	pop {r0-r3, r12, lr}
	pop {r4-r11, lr}
	bx lr

THUMB_FUNC_START resume_sub_thread
resume_sub_thread:
	bx pc
	.arm
	.align 2, 0
resume_sub_thread_arm:
	push {r4-r11, lr}
	add lr, lr, #4
	push {r0-r3, r12, lr}

	ldr r0, =gThreadInfo
	str sp, [r0, #oThreadInfo_main_thread_sp]
	ldr sp, [r0, #oThreadInfo_sub_thread_sp]

	mov r1, #DEFAULT_SUBTHREAD_RUNNING_MODE
	strb r1, [r0, #oThreadInfo_sub_thread_running]

	ldrb r4, [r0, #oThreadInfo_sub_thread_break_reason]
	cmp r4, #SUBTHRED_BREAK_FROM_IRQ

.L_thumb_judge_from_normal:
	popne {r0-r3, r12}
	popne {r5-r11, lr}
	movne r4, lr

.L_thumb_judge_from_IRQ:
	popeq {r0-r3, r12}

	# 0x04: kernel_vblank_isr
	# 0x10: IrqMain
	# 0x14: bios irq_vector
	ldreq lr, [sp, #0x24]
	subeq lr, lr, #4
	streq lr, [sp, #0x24]
	# shift thumb bit into carry flag
	moveq r4, lr

	# restore sub-thread general registers, EXCEPT r4
	popeq {r5-r11,lr}

1:
	# carry clear = arm mode = can just return away
	lsr r4, r4, #1
	popcc {r4}
	popcc {pc}

	// need to switch to thumb mode
	adr r4, resume_sub_thread_thumb+1
	bx r4

	.thumb
resume_sub_thread_thumb:
	pop {r4}
	pop {pc}

THUMB_FUNC_START yield_sub_thread
yield_sub_thread:
	bx pc
	.arm
	.align 2, 0
yield_sub_thread_arm:
	mov r3, REG_BASE
	add r3, REG_OFFSET_IME
	mov r0, #0
	strh r0, [r3]

	ldr r1, =gThreadInfo
	ldrb r0, [r1, #oThreadInfo_sub_thread_running]
	cmp r0, #INVALID_SUBTHREAD_RUNNING_MODE
	bne 1f

	# error return
	mov r0, #1
	strh r0, [r3]
	bx lr

1:
	# save sub thread
	adr r0, .L_thumb_bxlr + 1
	push {r0}
	push {r4}
	push {r5-r11, lr}
	push {r0-r3, r12}
	str sp, [r1, #oThreadInfo_sub_thread_sp]

	# load main
	ldr r0, =gThreadInfo
	ldr sp, [r0, #oThreadInfo_main_thread_sp]
	pop {r0-r3, r12, lr}
	pop {r4-r11, lr}

	ldr r0, =gThreadInfo
	mov r1, #INVALID_SUBTHREAD_RUNNING_MODE
	strb r1, [r0, #oThreadInfo_sub_thread_running]
	mov r1, #SUBTHRED_BREAK_FROM_NORMAL
	strb r1, [r0, #oThreadInfo_sub_thread_break_reason]

	mov r3, REG_BASE
	add r3, REG_OFFSET_IME
	mov r0, #1
	strh r0, [r3]

	bx lr

.L_thumb_bxlr:
	.thumb
	bx lr
