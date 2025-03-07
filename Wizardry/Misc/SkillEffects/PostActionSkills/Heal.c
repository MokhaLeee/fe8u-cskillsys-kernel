#include "common-chax.h"
#include "debuff.h"
#include "map-anims.h"
#include "kernel-lib.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

bool PostAction_BattleActorHeal(ProcPtr parent)
{
	int heal = 0;

	int hp_cur = GetUnitCurrentHp(gActiveUnit);
	int hp_max = GetUnitMaxHp(gActiveUnit);

	if (!UnitAvaliable(gActiveUnit) || UNIT_STONED(gActiveUnit))
		return false;

#if defined(SID_Lifetaker) && (COMMON_SKILL_VALID(SID_Lifetaker))
	if (SkillListTester(gActiveUnit, SID_Lifetaker) && gBattleActorGlobalFlag.enimy_defeated)
		heal += hp_max * SKILL_EFF0(SID_Lifetaker) / 100;
#endif

#if defined(SID_MysticBoost) && (COMMON_SKILL_VALID(SID_MysticBoost))
	if (SkillListTester(gActiveUnit, SID_MysticBoost))
		heal += SKILL_EFF0(SID_MysticBoost);
#endif

	if (heal == 0)
		return false;

	if ((heal >= (hp_max - hp_cur)))
		heal = hp_max - hp_cur;

	/**
	 * Try skip anim
	 */
	if (CheckKernelHookSkippingFlag()) {
		AddUnitHp(gActiveUnit, heal);
		return false;
	}

	CallMapAnim_Heal(parent, gActiveUnit, heal);
	return true;
}
