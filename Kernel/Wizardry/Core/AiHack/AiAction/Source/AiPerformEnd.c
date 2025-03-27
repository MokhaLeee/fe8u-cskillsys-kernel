#include "common-chax.h"

LYN_REPLACE_CHECK(CpPerform_Cleanup);
void CpPerform_Cleanup(struct CpPerformProc *proc)
{
	UpdateAllPhaseHealingAIStatus();

#if !CHAX
	AiRefreshMap();
#else
	gActiveUnit = GetUnit(gActionData.subjectIndex);

	SetCursorMapPosition(gAiDecision.xMove, gAiDecision.yMove);
	RenderBmMapOnBg2();

	MoveActiveUnit(gAiDecision.xMove, gAiDecision.yMove);

#if CHAX
	if (gActiveUnit->curHP != 0) {
		switch (gAiDecision.actionId) {
		case CONFIG_AI_ACTION_EXPA_Teleportation:
			gActiveUnit->state &= ~US_UNSELECTABLE;
			break;
		}
	}
#endif

	RefreshEntityBmMaps();
	RenderBmMap();

	NewBMXFADE(1);

	EndAllMus();
	RefreshEntityBmMaps();

	ShowUnitSprite(gActiveUnit);
	RefreshUnitSprites();
#endif

	if (!(gActiveUnit->pCharacterData) || (gActiveUnit->state & (US_HIDDEN | US_DEAD | US_BIT16)))
		Proc_Goto(proc, 1);
}
