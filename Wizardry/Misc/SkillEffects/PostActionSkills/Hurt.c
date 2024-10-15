#include "common-chax.h"
#include "skill-system.h"
#include "debuff.h"
#include "map-anims.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionBattleActorHurt(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;
	FORCE_DECLARE struct Unit *target = GetUnit(gActionData.targetIndex);
	int damage = 0;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
#if defined(SID_GrislyWound) && (COMMON_SKILL_VALID(SID_GrislyWound))
		if (SkillTester(target, SID_GrislyWound)) {
			int GrislyWoundDamage = unit->maxHP / 5;

			if (GrislyWoundDamage > unit->curHP)
				GrislyWoundDamage = unit->curHP - 1;

			damage += GrislyWoundDamage;
		}
#endif

#if defined(SID_Scendscale) && (COMMON_SKILL_VALID(SID_Scendscale))
		if (SkillTester(unit, SID_Scendscale) && GetBattleGlobalFlags(&gBattleActor)->hitted) {
			damage += SKILL_EFF1(SID_Scendscale);
		}
#endif

	/* fall through */

	case UNIT_ACTION_STAFF:
#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
		if (SkillTester(unit, SID_Fury))
			damage += 6;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
		if (SkillTester(unit, SID_FuryPlus))
			damage += 8;
#endif

#if defined(SID_DoubleLion) && (COMMON_SKILL_VALID(SID_DoubleLion))
		if (SkillTester(unit, SID_DoubleLion) && (gBattleActorGlobalFlag.skill_activated_double_lion))
			damage += 1;
#endif

		break;
	};

	if (damage == 0)
		return false;

	if (damage >= GetUnitCurrentHp(unit))
		damage = GetUnitCurrentHp(unit) - 1;

	CallMapAnim_Hurt(parent, unit, damage);
	return true;
}

bool PostActionBattleTargetHurt(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = GetUnit(gActionData.targetIndex);
	FORCE_DECLARE struct Unit *actor = GetUnit(gActionData.subjectIndex);

	int damage = 0;

	if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
		return false;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
#if defined(SID_PoisonStrike) && (COMMON_SKILL_VALID(SID_PoisonStrike))
		if (SkillTester(actor, SID_PoisonStrike)) {
			int poisonStrikeDamage = unit->maxHP / 5;

			if (poisonStrikeDamage > unit->curHP)
				poisonStrikeDamage = unit->curHP - 1;

			damage += poisonStrikeDamage;
		}
#endif

#if defined(SID_GrislyWound) && (COMMON_SKILL_VALID(SID_GrislyWound))
		if (SkillTester(actor, SID_GrislyWound)) {
			int GrislyWoundDamage = unit->maxHP / 5;

			if (GrislyWoundDamage > unit->curHP)
				GrislyWoundDamage = unit->curHP - 1;

			damage += GrislyWoundDamage;
		}
#endif

#if defined(SID_Scendscale) && (COMMON_SKILL_VALID(SID_Scendscale))
		if (SkillTester(unit, SID_Scendscale) && GetBattleGlobalFlags(&gBattleTarget)->hitted)
			damage += SKILL_EFF1(SID_Scendscale);
#endif

	/* fall through */

	case UNIT_ACTION_STAFF:
#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
		if (SkillTester(unit, SID_Fury))
			damage += 6;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
		if (SkillTester(unit, SID_FuryPlus))
			damage += 8;
#endif

		break;
	};

	if (damage == 0)
		return false;

	if (damage >= GetUnitCurrentHp(unit))
		damage = GetUnitCurrentHp(unit) - 1;

	CallMapAnim_Hurt(parent, unit, damage);
	return true;
}
