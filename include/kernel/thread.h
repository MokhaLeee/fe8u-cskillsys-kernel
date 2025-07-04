#pragma once

#include "common-chax.h"

enum {
	KTHREAD_STACK_SIZE = 0x100,
	MAX_KTHREADS = 8,
};

struct context_stack {
	u32 pc_ref;
	u32 reg_0_12_lr[14];
};

void jump_to_routine(struct context_stack **sp);
void switch_to_routine(struct context_stack **old_sp, struct context_stack **new_sp);

struct kthread {
	u8 stack[KTHREAD_STACK_SIZE];
	void (*task)(void*);
	void *arg;
	void *sp;
	u8 in_use;
};

extern struct kthread kthreads[MAX_KTHREADS];
extern u8 current_kthread;

void kernel_context_switch(struct kthread *old, struct kthread *new);

void timer_enable(void (*call_back)(void), int cnt);
void timer_disable(void);
void systick_isr(void);

void thread_scheduler(void);
