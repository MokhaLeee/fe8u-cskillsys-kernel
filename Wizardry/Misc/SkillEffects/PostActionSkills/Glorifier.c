#include "common-chax.h"
#include "skill-system.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostAction_Glorifier(ProcPtr parent)
{
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_BUFF_GLORIFIER);

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
	case UNIT_ACTION_STAFF:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff:
		break;

	default:
		return false;
	}

#if defined(SID_Glorifier) && (COMMON_SKILL_VALID(SID_Glorifier))
	if (UnitAvaliable(gActiveUnit) && SkillListTester(gActiveUnit, SID_Glorifier)) {
		if (gBattleActorGlobalFlag.enimy_defeated == true) {
			if (!CheckKernelHookSkippingFlag())
				NewMuSkillAnimOnActiveUnitWithDeamon(parent, SID_Glorifier, NULL, NULL);

			SetUnitStatDebuff(gActiveUnit, UNIT_STAT_BUFF_GLORIFIER);
			return true;
		}
	}
#endif
	return false;
}
