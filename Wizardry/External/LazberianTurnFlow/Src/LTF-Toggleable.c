
#include "LTF.h"

// I'm just borrowing padding between sections
#define gLTFEnableFlag (*((u8*)(0x03000006)))

extern const struct ProcCmd LTFPlayerPhaseProcHook[];

extern const u8 gLTFChapterEnableList[];

void LTFT_UpdateLTFEnable(void)
{
	gLTFEnableFlag = IsLtfEnabled();
}

int LTFT_IsActiveNow(void)
{
	return !!gLTFEnableFlag; //return !!CheckEventId(gLTFEnableFlag);
}

void LTFT_PlayerPhaseEndHook(struct Proc* proc)
{
	if (LTFT_IsActiveNow())
		// Proc_JumpToPointer(proc, LTFPlayerPhaseProcHook);
		Proc_GotoScript(proc, LTFPlayerPhaseProcHook);
	else
		Proc_Goto(proc, 0);
}

void LTFT_CpDecideLoopCheck(struct Proc* proc)
{
	if (!LTFT_IsActiveNow())
		Proc_Goto(proc, 0);
}
