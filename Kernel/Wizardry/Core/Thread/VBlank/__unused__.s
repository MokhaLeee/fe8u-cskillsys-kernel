	.INCLUDE "macros.inc"
	.INCLUDE "gba.inc"
	.SYNTAX UNIFIED

	.section .text


THUMB_FUNC_START jump_to_in_process_task
	bx pc
	nop
	.arm
_jump_to_routine:
	ldr r0, [r0]
	mov sp, r0
_jump_to_routine_ext:
	pop {r0}
	mov r1, #1
	tst r0, r1
	bne 1f
	pop {r0-r12, lr}
	pop {pc}

1:
	pop {r0-r12, lr}
	push {r0}
	mov r0, pc
	add r0, #1
	bx r0

	.thumb
	pop {r0}
	pop {pc}

.arm
# .global switch_to_routine
.type switch_to_routine, %function
switch_to_routine:
	str sp, [r0]
	mov r0, pc
	sub r0, #8
	b _jump_to_routine

ARM_FUNC_START resume_main_routine
resume_main_routine:
	bx lr

THUMB_FUNC_START kernel_vblank_isr
kernel_vblank_isr:
	/**
	* gba_bis::irq_vector --> vanilla::IrqMain --> kernel_vblank_isr
	* here we need to modify on gba_bios::irq_vector to recover the context
	*/
	push {lr}
	bl common_vblank_isr
	bx pc
	nop
	.ARM
	bl resume_main_routine
	pop {r0}
	bx r0

THUMB_FUNC_START jump_to_in_process_task
jump_to_in_process_task:

push_context_stack:
	bx pc
	nop
	.arm
	stmia sp!, {pc}
	stmia sp!, {r0-r12, lr}
	bx lr

THUMB_FUNC_START exec_thread_tasks
exec_thread_tasks:
	ldr r1, =thread_stack
	mov sp, r1
	bl push_context_stack
	bx r1

THUMB_FUNC_START resume_task
resume_task:
	bx pc
	nop
	.arm
	ldr r1, =thread_stack
	mov sp, r1
	b _jump_to_routine_ext
