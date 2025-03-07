#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void anim_init(ProcPtr proc)
{
	struct MuProc *mu;

	HideUnitSprite(gActiveUnit);
	mu = StartMu(gActiveUnit);

	FreezeSpriteAnim(mu->sprite_anim);
	SetMuDefaultFacing(mu);
	SetDefaultColorEffects();
	EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void anim_act(ProcPtr proc)
{
	StartMuActionAnim(GetUnitMu(gActiveUnit));
}

static void anim_core(ProcPtr proc)
{
	StartLightRuneAnim(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void exec(ProcPtr proc)
{
	RemoveUnitNegativeStatus(gActiveUnit);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionClearance[] = {
	PROC_CALL(LockGame),
	PROC_CALL(MapAnim_CommonInit),
	PROC_CALL_2(EnsureCameraOntoActiveUnitPosition),
	PROC_YIELD,
	PROC_CALL(anim_init),
	PROC_YIELD,
	PROC_CALL(anim_act),
	PROC_SLEEP(30),
	PROC_CALL(anim_core),
	PROC_YIELD,
	PROC_CALL(exec),
	PROC_CALL(UnlockGame),
	PROC_CALL(MapAnim_CommonEnd),
	PROC_END
};

bool PostAction_Clearance(ProcPtr parent)
{
	if (!UnitAvaliable(gActiveUnit))
		return false;

#if defined(SID_Clearance) && (COMMON_SKILL_VALID(SID_Clearance))
	if (!SkillListTester(gActiveUnit, SID_Clearance))
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

	if (UnitHasNegativeStatus(gActiveUnit)) {
		if (CheckKernelHookSkippingFlag()) {
			RemoveUnitNegativeStatus(gActiveUnit);
			return false;
		}

		Proc_Start(ProcScr_PostActionClearance, PROC_TREE_3);
		return true;
	}
	return false;
}
