#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "combat-art.h"
#include "class-types.h"
#include "constants/skills.h"

STATIC_DECLAR bool CheckBeastNullEffective(struct Unit *unit)
{
	/* Check skill */
#if (defined(SID_BeastShield) && (COMMON_SKILL_VALID(SID_BeastShield)))
	if (SkillListTester(unit, SID_BeastShield))
		return true;
#endif

	return false;
}

STATIC_DECLAR bool CheckFlierNullEffective(struct Unit *unit)
{
	int i;

	/* Check skill */
#if (defined(SID_WingedShield) && (COMMON_SKILL_VALID(SID_WingedShield)))
	if (SkillListTester(unit, SID_WingedShield))
		return true;
#endif

	/* Check item */
	for (i = 0; i < UNIT_ITEM_COUNT; ++i)
		if (GetItemAttributes(unit->items[i]) & IA_NEGATE_FLYING)
			return true;

	return false;
}

STATIC_DECLAR bool CheckUnitNullEffective(struct Unit *unit)
{
#if (defined(SID_Nullify) && (COMMON_SKILL_VALID(SID_Nullify)))
	/* Check unit */
	if (SkillListTester(unit, SID_Nullify))
		return true;
#endif

	return false;
}

LYN_REPLACE_CHECK(IsItemEffectiveAgainst);
bool IsItemEffectiveAgainst(u16 item, struct Unit *unit)
{
	int i, jid;
	const u8 *list;

	if (!unit->pClassData)
		return false;

	jid = UNIT_CLASS_ID(unit);
	list = GetItemEffectiveness(item);

	if (!list)
		return false;

	for (i = 0; list[i]; i++)
		if (list[i] == jid)
			goto check_null_effective;

	return false;

check_null_effective:
	if (CheckUnitNullEffective(unit))
		return false;

	return true;
}

STATIC_DECLAR bool IsBattleUnitEffectiveAgainst(struct BattleUnit *actor, struct BattleUnit *target)
{
	int jid_target = UNIT_CLASS_ID(&target->unit);

	/* Check combat-art */
	if (actor == &gBattleActor) {
		int cid = GetCombatArtInForce(&actor->unit);

		if (COMBART_VALID(cid)) {
			switch (GetCombatArtInfo(cid)->effectiveness) {
			case COMBART_EFF_ALL:
				return true;

			case COMBART_EFF_ARMOR:
				if (CheckClassArmor(jid_target))
					return true;

				break;

			case COMBART_EFF_CAVALRY:
				if (CheckClassCavalry(jid_target))
					return true;

				break;

			case COMBART_EFF_FLIER:
				if (CheckClassFlier(jid_target) && !CheckFlierNullEffective(&target->unit))
					return true;

				break;

			case COMBART_EFF_DRAGON:
				if (CheckClassDragon(jid_target))
					return true;

				break;

			case COMBART_EFF_MONSTER:
				if (CheckClassBeast(jid_target) && !CheckBeastNullEffective(&target->unit))
					return true;

				break;

			case COMBART_EFF_NONE:
			default:
				break;
			}
		}
	}

#if (defined(SID_DoOrDie) && (COMMON_SKILL_VALID(SID_DoOrDie)))
	if (BattleFastSkillTester(actor, SID_DoOrDie)) {
		if ((target->battleAttack - actor->battleDefense) >= actor->hpInitial)
			return true;
	}
#endif

	return false;
}

LYN_REPLACE_CHECK(IsUnitEffectiveAgainst);
bool IsUnitEffectiveAgainst(struct Unit *actor, struct Unit *target)
{
	FORCE_DECLARE int jid_target = UNIT_CLASS_ID(target);

	if (IS_BATTLE_UNIT(actor) && IS_BATTLE_UNIT(target)) {
		if (IsBattleUnitEffectiveAgainst((struct BattleUnit *)actor, (struct BattleUnit *)target))
			goto check_null_effective;
	}

	/* Check skills */
#if (defined(SID_Slayer) && (COMMON_SKILL_VALID(SID_Slayer)))
	if (SkillListTester(actor, SID_Slayer)) {
		if (CheckClassBeast(jid_target) && !CheckBeastNullEffective(target))
			goto check_null_effective;
	}
#endif

#if (defined(SID_Skybreaker) && (COMMON_SKILL_VALID(SID_Skybreaker)))
	if (SkillListTester(actor, SID_Skybreaker)) {
		if (CheckClassFlier(jid_target) && !CheckFlierNullEffective(target))
			goto check_null_effective;
	}
#endif

	return false;

check_null_effective:
	if (CheckUnitNullEffective(target))
		return false;

	return true;
}
