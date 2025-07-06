	.INCLUDE "macros.inc"
	.INCLUDE "gba.inc"
	.INCLUDE "kthread.inc"
	.SYNTAX UNIFIED

.set PSR_USR_MODE,  0x00000010
.set PSR_FIQ_MODE,  0x00000011
.set PSR_IRQ_MODE,  0x00000012
.set PSR_SVC_MODE,  0x00000013
.set PSR_ABT_MODE,  0x00000017
.set PSR_UND_MODE,  0x0000001b
.set PSR_SYS_MODE,  0x0000001f
.set PSR_MODE_MASK, 0x0000001f
.set PSR_T_BIT,     0x00000020
.set PSR_F_BIT,     0x00000040
.set PSR_I_BIT,     0x00000080

	.section .text
ARM_FUNC_START IrqMain_CHAX
IrqMain_CHAX:
	@ Reserve IE & spsr
	mov r3, REG_BASE
	add r3, r3, REG_OFFSET_IE
	ldr r2, [r3]
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mrs r0, spsr
	push {r0, r1, r3, lr}

irq_search:
	and r1, r2, r2, lsr #16
	ands r0, r1, INTR_FLAG_GAMEPAK

@ when the cart is removed from the GBA.
panic_gamepak:
	bne panic_gamepak

	mov r2, #0
	ands r0, r1, INTR_FLAG_VBLANK
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_HBLANK
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_VCOUNT
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_TIMER0
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_TIMER1
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_TIMER2
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_TIMER3
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_SERIAL
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_DMA0
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_DMA1
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_DMA2
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_DMA3
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_KEYPAD
	bne handle_normal_irq
	add r2, r2, #4
	ands r0, r1, INTR_FLAG_GAMEPAK

@ when the cart is removed from the GBA.
panic_gamepak2:
	bne panic_gamepak2

handle_normal_irq:
	strh r0, [r3, #2]

	/**
	 * For vblank irq, we need to suspend the sub task
	 */
	ldr r1, =gThreadInfo
	ldrb r3, [r1, #oThreadInfo_sub_thread_running]
	cmp r2, r3
	// r12 now is the top of bios irq handler
	addeq r12, sp, #0x10

	@ Switch to System Mode
	mrs r3, cpsr
	bic r3, r3, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r3, r3, #PSR_SYS_MODE
	msr cpsr_fc, r3

	ldr r3, =gIRQHandlers
	add r3, r3, r2
	ldr r3, [r3]

	/**
	 * Try to suspend the sub thread to main!
	 */
	ldrb r0, [r1, #oThreadInfo_sub_thread_running]
	cmp r2, r0
	bne .L_irq_no_sub_thread

	mov r0, #SUBTHRED_BREAK_FROM_IRQ
	strb r0, [r1, #oThreadInfo_sub_thread_break_reason]

	// load Bios' LR on INTR
	ldr r0, [r12, #0x14]

	// But it has no thumb/arm info
	// Thus we need to get the thumb bit from SPSR
	// switch to irq mode
	mrs r2, cpsr
	bic r2, r2, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r2, r2, #PSR_IRQ_MODE
	msr cpsr_fc, r2

	mrs r2, spsr
	tst r2, #PSR_T_BIT
	orrne r0, r0, #1

	// switch back to system mode
	mrs r2, cpsr
	bic r2, r2, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r2, r2, #PSR_SYS_MODE
	msr cpsr_fc, r2

	/**
	 * r0,  r1,  r2,  r3, r12
	 * r5,  r6,  r7, r8,  r9,  r10, r11,
	 * lr,  r4,  pc
	 */
	push {r0}
	push {r4}
	push {r5-r11, lr}

	// read the sub-thread irq saved regs
	// we're reading what r0-r3,r12 are
	// of where the irq saved regs are stored
	// but we have to use different scratch registers
	// so that we save r0 so we can read the intr function later
	ldmia r12, {r0,r2,r4,r5,r6}

	// save the sub-thread irq saved regs (except lr)
	push {r0,r2,r4,r5,r6}

	// main thread was restored, so mark in sub thread as false
	mov r0, #INVALID_SUBTHREAD_RUNNING_MODE
	strb r0, [r1, #oThreadInfo_sub_thread_running]

	// save the sub-thread stack pointer
	str sp, [r1, #oThreadInfo_sub_thread_sp]

/*
	// For debug
	pop {r0-r2}
	pop {r9}  @ r3
	pop {r10} @ r12
	pop {r5-r8}
	pop {r11} @ r9
	pop {r11} @ r10
	pop {r11}
	pop {lr}
	pop {r4}
	pop {r11} @ pc
	ldr r1, =gThreadInfo
*/

	// and load the main thread stack pointer
	ldr sp, [r1, #oThreadInfo_main_thread_sp]
	// this pops off the main thread registers, because the sp we just loaded
	// had previously pushed the main thread registers before

	// Enable print
	// ldr r1, =log_print_en
	// mov r0, #1
	// strb r0, [r1]

	// restore the main thread irq saved registers (except lr)
	pop {r0,r2,r4,r5,r6,lr}
	// restore the main thread irq lr
	// and store them to where they're saved on the stack
	stmia r12, {r0,r2,r4,r5,r6,lr}

	// restore the main thread general registers
	pop {r4-r11,lr}

.L_irq_no_sub_thread:
	@ jump to irq-handler
	push {lr}
	add lr, pc, #0
	bx r3
	pop {lr}

	@ Reset to IRQ Mode, also disable irq interrupt
	mrs r3, cpsr_fc
	bic r3, r3, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r3, r3, #PSR_I_BIT | PSR_IRQ_MODE
	msr cpsr_fc, r3

	@ Restore original spsr & IE
	pop {r0, r1, r3, lr}
	strh r1, [r3]
	msr spsr_fc, r0
	bx lr
