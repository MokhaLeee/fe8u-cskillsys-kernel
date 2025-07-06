#include "common-chax.h"
#include "kthread.h"

extern void _VBlankIntrWait(void);
extern void start_sub_thread(void);
extern void resume_sub_thread(void);

#define LOCAL_TRACE 1

void VBlankIntrWaitRework(void)
{
	u32 vcount = REG_VCOUNT;

	if (vcount >= 160 || vcount == 0) {
		_VBlankIntrWait();
		return;
	}

	if (gThreadInfo.sub_thread_state == SUBTHREAD_NONE) {
		_VBlankIntrWait();
		return;
	}

	if (gThreadInfo.sub_thread_state == SUBTHREAD_ACTIVE) {
		LTRACEF("resume subthread: 0x%08X", gThreadInfo.func);

#ifdef CONFIG_DEBUG_DUMP_THREAD_STACK
		dump_sub_thread_stack();
#endif

		log_print_en = false;
		start_sub_thread();
		log_print_en = true;
	} else {
		LTRACEF("start subthread: 0x%08X", gThreadInfo.func);

		gThreadInfo.sub_thread_state = SUBTHREAD_ACTIVE;

		log_print_en = false;
		start_sub_thread();
		log_print_en = true;
	}

	// gThreadInfo.sub_thread_state = SUBTHREAD_NONE;
	if (gThreadInfo.sub_thread_break_reason == SUBTHRED_BREAK_FROM_NORMAL)
		LTRACE("return to main thread");

	_VBlankIntrWait();
}
