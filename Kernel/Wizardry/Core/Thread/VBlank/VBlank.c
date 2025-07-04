#include "common-chax.h"
#include "thread.h"

extern void break_task(void);
extern void jump_to_thread_tasks(void);
extern void _VBlankIntrWait(void);

extern InterruptHandler gCommonVblankIrqHandler;

void kernel_vblank_isr(void)
{
	if (gCommonVblankIrqHandler != NULL)
		gCommonVblankIrqHandler();

	break_task();
}

void GameInit_InitVBlankIrq(void)
{
	gCommonVblankIrqHandler = NULL;
	SetIRQHandler(0, kernel_vblank_isr);

	REG_IE |= INTR_FLAG_VBLANK;
	gLCDControlBuffer.dispstat.vblankIrqEnable = 1;
}

LYN_REPLACE_CHECK(SetInterrupt_LCDVBlank);
void SetInterrupt_LCDVBlank(InterruptHandler handler)
{
	gCommonVblankIrqHandler = handler;
}

void VBlankIntrWaitRework(void)
{
	jump_to_thread_tasks();
	_VBlankIntrWait();
}
