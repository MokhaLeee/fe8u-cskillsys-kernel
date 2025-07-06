#include "gbafe.h"
#include "kthread.h"

extern void _VBlankIntrWait(void);
extern void start_sub_thread(void);
extern void resume_sub_thread(void);

void VBlankIntrWaitRework(void)
{
	u32 vcount = REG_VCOUNT;

	if (vcount >= 140) {
		_VBlankIntrWait();
		return;
	}

	if (gThreadInfo.sub_thread_state == SUBTHREAD_NONE) {
		_VBlankIntrWait();
		return;
	}

	if (gThreadInfo.sub_thread_state == SUBTHREAD_ACTIVE)
		resume_sub_thread();
	else {
		gThreadInfo.sub_thread_state = SUBTHREAD_ACTIVE;
		start_sub_thread();
	}

	gThreadInfo.sub_thread_state = SUBTHREAD_NONE;
	_VBlankIntrWait();
}
