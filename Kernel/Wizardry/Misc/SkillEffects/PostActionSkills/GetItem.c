#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionGetItem(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;

	if (!UNIT_IS_VALID(unit))
		return false;

	if (!UNIT_IS_VALID(unit) || UNIT_STONED(unit))
		return false;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
		if (gBattleActorGlobalFlag.enimy_defeated) {
#if defined(SID_Despoil) && (COMMON_SKILL_VALID(SID_Despoil))
			if (SkillListTester(unit, SID_Despoil)) {
				NewPopup_ItemGot(parent, unit, ITEM_REDGEM);
				return true;
			}
#endif

#if defined(SID_GoldDigger) && (COMMON_SKILL_VALID(SID_GoldDigger))
			if (SkillListTester(unit, SID_GoldDigger) && gBattleActorGlobalFlag.enimy_defeated) {
				NewPopup_GoldGot(parent, unit, SKILL_EFF0(SID_GoldDigger));
				return true;
			}
#endif
		}
		break;

	default:
		break;
	}
	return false;
}
