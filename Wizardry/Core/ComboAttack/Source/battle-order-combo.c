#include "common-chax.h"
#include "battle-system.h"
#include "combo-attack.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

STATIC_DECLAR bool BattleComboGenerateHit(void)
{
	FORCE_DECLARE struct Unit *unit;
	int ret;

	unit = GetUnit(gComboAtkList[GetBattleHitRound(gBattleHitIterator)].uid);

	gBattleStats.hitRate = 80;
	gBattleStats.damage = 0;

	/* step1 BattleGenerateHitAttributes */
	ret = BattleRoll2RN(gBattleStats.hitRate, TRUE);
	if (!ret) {
		// Miss
		gBattleStats.damage = 0;
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
	} else {
		// Hitted
		gBattleStats.damage = 5;

#if (defined(SID_Assist) && COMMON_SKILL_VALID(SID_Assist))
		if (SkillTester(unit, SID_Assist))
			gBattleStats.damage += SKILL_EFF0(SID_Assist);
#endif

#if (defined(SID_Synergism) && COMMON_SKILL_VALID(SID_Synergism))
		if (BattleFastSkillTester(&gBattleActor, SID_Synergism))
			gBattleStats.damage += SKILL_EFF0(SID_Synergism);
#endif
	}

	/* step2 BattleGenerateHitEffects */
	gBattleTarget.unit.curHP -= gBattleStats.damage;

	if (gBattleTarget.unit.curHP < 0)
		gBattleTarget.unit.curHP = 0;

	gBattleHitIterator->hpChange = gBattleStats.damage;

	Printf("Combot hit %d for uid %#x",
		GetBattleHitRound(gBattleHitIterator), gComboAtkList[GetBattleHitRound(gBattleHitIterator)].uid);

	/* step3 BattleGenerateHit */
	if (gBattleTarget.unit.curHP == 0) {
		gBattleActor.wexpMultiplier++;
		gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
		gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;
		gBattleHitIterator++;
		return true;
	}

	gBattleHitIterator++;
	return false;
}

bool BattleComboGenerateHits(void)
{
	int i;
	u32 attrs;

	ResetComboAtkList();

	if (!gpKernelDesigerConfig->combo_attack_en)
		return false;

	/* Not considering on simulation */
	if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
		return false;

	/* Not considering on enemy */
	if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE)
		return false;

	if (AreUnitsAllied(gBattleActor.unit.index, gBattleTarget.unit.index))
		return false;

	BattleGenerateComboAtkList();

	attrs = gBattleHitIterator->attributes;

	for (i = 0; i < COMBO_ATK_MAX; i++) {
		if (gComboAtkList[i].uid == COMBO_ATK_UID_INVALID)
			break;

		gBattleHitIterator->attributes |= attrs;
		if (BattleComboGenerateHit())
			return true;
	}
	return false;
}
