#include "common-chax.h"
#include "kernel-lib.h"
#include "battle-system.h"
#include "skill-system.h"
#include "post-action.h"
#include "constants/skills.h"

static bool Local_SkillTester(void)
{
#if defined(SID_Shakedown) && (COMMON_SKILL_VALID(SID_Shakedown))
	if (CheckActiveUnitSkillActivate(SID_Shakedown, GetUnitLuck(gActiveUnit)))
		return true;
#endif

	return false;
}

FORCE_DECLARE static void callback2(ProcPtr proc)
{
	NewPopup_GoldGot(proc, gActiveUnit, gPostActionCommonBuffer.total_damage_actor);
}

bool PostActionSkillShakedown(ProcPtr parent)
{
	if (!UNIT_IS_VALID(gActiveUnit))
		return false;

	if (!Local_SkillTester())
		return false;

	if (gPostActionCommonBuffer.total_damage_actor <= 0)
		return false;

#if defined(SID_Shakedown) && (COMMON_SKILL_VALID(SID_Shakedown))
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, SID_Shakedown, NULL, callback2);
#endif

	return true;
}
