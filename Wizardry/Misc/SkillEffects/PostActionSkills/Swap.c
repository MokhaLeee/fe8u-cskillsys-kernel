#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/texts.h"
#include "constants/skills.h"
#include "battle-system.h"

STATIC_DECLAR void PostActionSwapSkipMenuIfNotAlly(struct EventEngineProc *proc)
{
	gEventSlots[EVT_SLOT_C] = false;

	if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
		gEventSlots[EVT_SLOT_C] = true;
}

STATIC_DECLAR void PreparePositionSwap(void)
{
	EndAllMus();
	RefreshUnitSprites();
}

static void set_target_unit(void)
{
	struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

	gEventSlots[EVT_SLOT_2] = unit_tar->index;
}

static void set_actor_unit(void)
{
	gEventSlots[EVT_SLOT_2] = gActiveUnit->index;
}

static void set_position(void)
{
	struct Unit *unita = gActiveUnit;
	struct Unit *unitb = GetUnit(gActionData.targetIndex);

	int x = unita->xPos;
	int y = unita->yPos;

	unita->xPos = unitb->xPos;
	unita->yPos = unitb->yPos;

	gActionData.xMove = unitb->xPos;
	gActionData.yMove = unitb->yPos;

	unitb->xPos = x;
	unitb->yPos = y;
}

static void check_skipping(void)
{
	int ret = CheckKernelHookSkippingFlag();

	gEventSlots[EVT_SLOT_C] = ret;

	/**
	 * Try skip anim
	 */
	if (ret)
		set_position();
}

STATIC_DECLAR const EventScr EventScr_PostActionPositionSwap[] = {
	EVBIT_MODIFY(0x4)
	STAL(20)
	ASMC(PostActionSwapSkipMenuIfNotAlly)
	BEQ(0, EVT_SLOT_C, EVT_SLOT_0)

	TUTORIALTEXTBOXSTART
	SVAL(EVT_SLOT_B, 0xffffffff)
	TEXTSHOW(MSG_SkillEffect_Lunge_Confirm)
	TEXTEND
	REMA
	SVAL(EVT_SLOT_7, 0x1)
	BNE(99, EVT_SLOT_C, EVT_SLOT_7)

	ASMC(check_skipping)
	BNE(99, EVT_SLOT_C, EVT_SLOT_0)

LABEL(0)
#if defined(SID_Lunge) && (COMMON_SKILL_VALID(SID_Lunge))
	SVAL(EVT_SLOT_B, SID_Lunge)
	CALL(EventScr_MuSkillAnim)
#endif
	ASMC(PreparePositionSwap)
	ASMC(set_actor_unit)
	CALL(EventScr_UidWarpOUT)
	STAL(20)
	ASMC(set_target_unit)
	CALL(EventScr_UidFlushingOUT)
	STAL(60)
	ASMC(set_position)
	ASMC(set_target_unit)
	CALL(EventScr_UidFlushingIN)
	STAL(20)
	ASMC(set_actor_unit)
	CALL(EventScr_UidWarpIN)
	STAL(20)

LABEL(99)
	NoFade
	ENDA
};

bool PostActionSwap(ProcPtr proc)
{
	struct Unit *unit = gActiveUnit;
	struct Unit *unit_tar;

	if (!UNIT_IS_VALID(unit))
		return false;

#if defined(SID_Lunge) && (COMMON_SKILL_VALID(SID_Lunge))
	if (!SkillListTester(unit, SID_Lunge))
#else
	if (1)
#endif
		return false;

	if (!UnitAvaliable(unit) || UNIT_STONED(unit))
		return false;

	unit_tar = GetUnit(gActionData.targetIndex);

	if (!UnitAvaliable(unit_tar) || UNIT_STONED(unit_tar))
		return false;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
		break;

	default:
		return false;
	}

#if 0
	if (gBattleActorGlobalFlag.hitted == false)
		return false;
#endif

	KernelCallEvent(EventScr_PostActionPositionSwap, EV_EXEC_CUTSCENE, proc);
	return true;
}
