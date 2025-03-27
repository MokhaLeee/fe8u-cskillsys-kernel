#include <common-chax.h>

LYN_REPLACE_CHECK(PlayerPhase_Suspend);
void PlayerPhase_Suspend(void) {}

void Hook_BmMain_SuspendBeforePhase(void)
{
	gActionData.suspendPointType = SUSPEND_POINT_PHASECHANGE;
	WriteSuspendSave(SAVE_ID_SUSPEND);
}
