#include "common-chax.h"
#include "status-getter.h"
#include "battle-system.h"
#include "action-expa.h"
#include "skill-system.h"
#include "constants/skills.h"

STATIC_DECLAR bool CheckCanto(void)
{
	bool canto, cantop;

	if (gActiveUnit->state & (US_DEAD | US_HAS_MOVED | US_BIT16))
		return false;

#if defined(SID_Canto) && (COMMON_SKILL_VALID(SID_Canto))
	canto = SkillListTester(gActiveUnit, SID_Canto);
#else
	canto = false;
#endif

#if defined(SID_CantoPlus) && (COMMON_SKILL_VALID(SID_CantoPlus))
	cantop = SkillListTester(gActiveUnit, SID_CantoPlus);
#else
	cantop = false;
#endif

	if (!canto && !cantop)
		return false;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_WAIT:
		return false;

	case UNIT_ACTION_COMBAT:
	case UNIT_ACTION_STAFF:
	case CONFIG_UNIT_ACTION_EXPA_ExecSkill:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff:
		if (!cantop)
			return false;

		break;
	}

	if (MovGetter(gActiveUnit) <= gActionData.moveCount)
		return false;

	if (!CanUnitMove())
		return false;

	return true;
}

LYN_REPLACE_CHECK(TryMakeCantoUnit);
bool TryMakeCantoUnit(ProcPtr proc)
{
	if (!CheckCanto())
		return false;

	BmMapFill(gBmMapRange, 0);

	UnitBeginCantoAction(gActiveUnit);

	gActiveUnit->state |= US_HAS_MOVED;
	gActiveUnit->state &= ~US_UNSELECTABLE;

	EndAllMus();
	StartMu(gActiveUnit);
	SetAutoMuDefaultFacing();

	if (gPlaySt.chapterVisionRange != 0)
		Proc_Goto(proc, 4);
	else
		Proc_Goto(proc, 1);

	return true;
}

LYN_REPLACE_CHECK(PlayerPhase_FinishAction);
void PlayerPhase_FinishAction(ProcPtr proc)
{
	if (gPlaySt.chapterVisionRange != 0) {
		RenderBmMapOnBg2();
		MoveActiveUnit(gActionData.xMove, gActionData.yMove);
		RefreshEntityBmMaps();
		RenderBmMap();
		NewBMXFADE(0);
		RefreshUnitSprites();
	} else {
		MoveActiveUnit(gActionData.xMove, gActionData.yMove);
		RefreshEntityBmMaps();
		RenderBmMap();
	}

	SetCursorMapPosition(gActiveUnit->xPos, gActiveUnit->yPos);

	gPlaySt.xCursor = gBmSt.playerCursor.x;
	gPlaySt.yCursor = gBmSt.playerCursor.y;

#if CHAX
	if (gActionDataExpa.refrain_action)
		gActiveUnit->state &= ~(US_UNSELECTABLE | US_CANTOING);
	else if (TryMakeCantoUnit(proc)) {
		HideUnitSprite(gActiveUnit);
		return;
	}
#else
	if (TryMakeCantoUnit(proc)) {
		HideUnitSprite(gActiveUnit);
		return;
	}
#endif

	if (ShouldCallEndEvent()) {
		EndAllMus();
		RefreshEntityBmMaps();
		RenderBmMap();
		RefreshUnitSprites();
		MaybeCallEndEvent_();
		Proc_Goto(proc, 8);
		return;
	}
	EndAllMus();
}
