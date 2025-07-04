#include "common-chax.h"
#include "kernel-lib.h"
#include "thread.h"

extern bool task_in_use;
extern void (*thread_task_handler)(void);

extern void exec_task(void (*task)(void));
extern void resume_task();

void jump_to_thread_tasks(void)
{
	if (task_in_use) {
		resume_task();
	} else if (thread_task_handler) {
		task_in_use = true;
		exec_task(thread_task_handler);
		task_in_use = false;
	}
}
