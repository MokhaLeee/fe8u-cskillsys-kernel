#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"

STATIC_DECLAR void PostActionReturnSkipMenuIfNotAlly(struct EventEngineProc *proc)
{
	ShowUnitSprite(gActiveUnit);

	gEventSlots[EVT_SLOT_C] = false;
	if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
		gEventSlots[EVT_SLOT_C] = true;
}

STATIC_DECLAR void PrepareReturnPosition(void)
{
	gEventSlots[EVT_SLOT_2] = gActiveUnit->index;

	EndAllMus();
	RefreshUnitSprites();
}

STATIC_DECLAR void PostActionReturnToOrigin(void)
{
	gEventSlots[EVT_SLOT_2] = gActiveUnit->index;

	gActionData.xMove = gActiveUnitMoveOrigin.x;
	gActionData.yMove = gActiveUnitMoveOrigin.y;

	gActiveUnit->xPos = gActiveUnitMoveOrigin.x;
	gActiveUnit->yPos = gActiveUnitMoveOrigin.y;
}

static void check_skipping(void)
{
	int ret = CheckKernelHookSkippingFlag();

	gEventSlots[EVT_SLOT_C] = ret;

	/**
	 * Try skip anim
	 */
	if (ret)
		PostActionReturnToOrigin();
}

STATIC_DECLAR const EventScr EventScr_PostActionPositionReturn[] = {
	EVBIT_MODIFY(0x4)
	// ASMC(MapAnim_CommonInit)
	ASMC(PostActionReturnSkipMenuIfNotAlly)
	BEQ(0, EVT_SLOT_C, EVT_SLOT_0)

	TUTORIALTEXTBOXSTART
	SVAL(EVT_SLOT_B, 0xffffffff)
	TEXTSHOW(MSG_SkillEffect_PosReturn_Confirm)
	TEXTEND
	REMA
	SVAL(EVT_SLOT_7, 0x1)
	BNE(99, EVT_SLOT_C, EVT_SLOT_7)

	ASMC(check_skipping)
	BNE(99, EVT_SLOT_C, EVT_SLOT_0)

LABEL(0)
#if defined(SID_PosReturn) && (COMMON_SKILL_VALID(SID_PosReturn))
	SVAL(EVT_SLOT_B, SID_PosReturn)
	CALL(EventScr_MuSkillAnim)
#endif
	STAL(20)
	ASMC(PrepareReturnPosition)
	CALL(EventScr_UidWarpOUT)
	STAL(20)
	ASMC(PostActionReturnToOrigin)
	CALL(EventScr_UidFlushingIN)
	STAL(20)

LABEL(99)
	ASMC(MapAnim_CommonEnd)
	NoFade
	ENDA
};

bool PostActionPositionReturn(ProcPtr proc)
{
	if (!UNIT_IS_VALID(gActiveUnit))
		return false;

#if defined(SID_PosReturn) && (COMMON_SKILL_VALID(SID_PosReturn))
	if (!SkillListTester(gActiveUnit, SID_PosReturn))
#else
	if (1)
#endif
		return false;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
		break;

	default:
		return false;
	}

	if (!UnitAvaliable(gActiveUnit) || UNIT_STONED(gActiveUnit))
		return false;

	if (gBattleActorGlobalFlag.enimy_defeated == false)
		return false;

	if (gActiveUnitMoveOrigin.x == gActiveUnit->xPos && gActiveUnitMoveOrigin.y == gActiveUnit->yPos)
		return false;

	KernelCallEvent(EventScr_PostActionPositionReturn, EV_EXEC_CUTSCENE, proc);
	return true;
}
