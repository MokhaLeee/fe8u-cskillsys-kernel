#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

FORCE_DECLARE static void callback_anim(ProcPtr proc)
{
	EndMu(GetUnitMu(gActiveUnit));
	StartStatusHealEffect(gActiveUnit, proc);
}

FORCE_DECLARE static void callback_refrain(ProcPtr proc)
{
	struct MuProc *mu;

	HideUnitSprite(gActiveUnit);

	mu = GetUnitMu(gActiveUnit);
	if (!mu)
		mu = StartMu(gActiveUnit);

	SetMuDefaultFacing(mu);
	StartTemporaryLock(proc, 15);
}

bool PostActionAlertStance(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;

	if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
		return false;

	if (gActionData.unitActionType == UNIT_ACTION_WAIT) {
#if defined(SID_AlertStancePlus) && (COMMON_SKILL_VALID(SID_AlertStancePlus))
		if (SkillTester(unit, SID_AlertStancePlus)) {
			NewMuSkillAnimOnActiveUnit(SID_AlertStancePlus, callback_anim, callback_refrain);
			SetUnitStatus(unit, NEW_UNIT_STATUS_AVOID_PLUS);
			return true;
		}
#endif

#if defined(SID_AlertStance) && (COMMON_SKILL_VALID(SID_AlertStance))
		if (SkillTester(unit, SID_AlertStance)) {
			NewMuSkillAnimOnActiveUnit(SID_AlertStancePlus, callback_anim, callback_refrain);
			SetUnitStatus(unit, NEW_UNIT_STATUS_AVOID);
			return true;
		}
#endif
	}
	return false;
}
