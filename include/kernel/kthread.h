#pragma once

#include "gbafe.h"

typedef void (*thread_task_func)(void);

enum {
	/* ThreadInfo::sub_thread_state */
	SUBTHREAD_NONE = 0,
	SUBTHREAD_PENDING = 1,
	SUBTHREAD_ACTIVE = 2,

	/* sub_thread_state::sub_thread_running */
	DEFAULT_SUBTHREAD_RUNNING_MODE = 0,
	INVALID_SUBTHREAD_RUNNING_MODE = 0xFF,

	/* sub_thread_state::sub_thread_break_reason */
	SUBTHRED_BREAK_FROM_IRQ = 0,
	SUBTHRED_BREAK_FROM_NORMAL = 1,
};

struct ThreadInfo {
	u8 sub_thread_running;
	u8 sub_thread_state;
	u8 sub_thread_break_reason;

	u8 _pad_;

	void *main_thread_sp;
	void *sub_thread_sp;

	thread_task_func func;
};

extern struct ThreadInfo gThreadInfo;

void CreateSubThread(thread_task_func func);
void RemoveSubThread(thread_task_func func);
void YieldSubThread(void);

void RegisterKthreadSchedulerTask(thread_task_func func);
void RemoveKthreadSchedulerTask(thread_task_func func);
void GameInit_SetupKthreadScheduler(void);
