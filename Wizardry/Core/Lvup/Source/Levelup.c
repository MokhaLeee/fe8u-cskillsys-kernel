#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "rn.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "skill-system.h"

STATIC_DECLAR int GetStatIncreaseRandC(int growth)
{
	int result = 0;

	while (growth > 100) {
		result++;
		growth -= 100;
	}

	if (Roll1RandC(growth))
		result++;

	return result;
}

STATIC_DECLAR int GetStatIncreaseFixed(int growth, int ref)
{
	return k_udiv(growth + k_umod(growth * ref, 100), 100);
}

STATIC_DECLAR void UnitLvup_Vanilla(struct BattleUnit *bu, int bonus)
{
	struct Unit *unit = GetUnit(bu->unit.index);

	bu->changeHP  = GetStatIncrease(GetUnitHpGrowth(unit)  + bonus);
	bu->changePow = GetStatIncrease(GetUnitPowGrowth(unit) + bonus);
	bu->changeSkl = GetStatIncrease(GetUnitSklGrowth(unit) + bonus);
	bu->changeSpd = GetStatIncrease(GetUnitSpdGrowth(unit) + bonus);
	bu->changeLck = GetStatIncrease(GetUnitLckGrowth(unit) + bonus);
	bu->changeDef = GetStatIncrease(GetUnitDefGrowth(unit) + bonus);
	bu->changeRes = GetStatIncrease(GetUnitResGrowth(unit) + bonus);

	BU_CHG_MAG(bu) = GetStatIncrease(GetUnitMagGrowth(unit) + bonus);
}

STATIC_DECLAR void UnitLvup_RandC(struct BattleUnit *bu, int bonus)
{
	struct Unit *unit = GetUnit(bu->unit.index);

	bu->changeHP  = GetStatIncreaseRandC(GetUnitHpGrowth(unit)  + bonus);
	bu->changePow = GetStatIncreaseRandC(GetUnitPowGrowth(unit) + bonus);
	bu->changeSkl = GetStatIncreaseRandC(GetUnitSklGrowth(unit) + bonus);
	bu->changeSpd = GetStatIncreaseRandC(GetUnitSpdGrowth(unit) + bonus);
	bu->changeLck = GetStatIncreaseRandC(GetUnitLckGrowth(unit) + bonus);
	bu->changeDef = GetStatIncreaseRandC(GetUnitDefGrowth(unit) + bonus);
	bu->changeRes = GetStatIncreaseRandC(GetUnitResGrowth(unit) + bonus);

	BU_CHG_MAG(bu) = GetStatIncreaseRandC(GetUnitMagGrowth(unit) + bonus);
}

STATIC_DECLAR void UnitLvup_Fixed(struct BattleUnit *bu, int bonus)
{
	struct Unit *unit = GetUnit(bu->unit.index);

	int ref = unit->level - 1;

	if (CA_PROMOTED & UNIT_CATTRIBUTES(unit))
		ref = ref + 19;

	bu->changeHP  = GetStatIncreaseFixed(GetUnitHpGrowth(unit)  + bonus, ref += 5);
	bu->changePow = GetStatIncreaseFixed(GetUnitPowGrowth(unit) + bonus, ref += 5);
	bu->changeSkl = GetStatIncreaseFixed(GetUnitSklGrowth(unit) + bonus, ref += 5);
	bu->changeSpd = GetStatIncreaseFixed(GetUnitSpdGrowth(unit) + bonus, ref += 5);
	bu->changeLck = GetStatIncreaseFixed(GetUnitLckGrowth(unit) + bonus, ref += 5);
	bu->changeDef = GetStatIncreaseFixed(GetUnitDefGrowth(unit) + bonus, ref += 5);
	bu->changeRes = GetStatIncreaseFixed(GetUnitResGrowth(unit) + bonus, ref += 5);

	BU_CHG_MAG(bu) = GetStatIncreaseFixed(GetUnitMagGrowth(unit) + bonus, ref += 5);
}

STATIC_DECLAR void UnitLvup_100(struct BattleUnit *bu, int bonus)
{
	bu->changeHP  = 1;
	bu->changePow = 1;
	bu->changeSkl = 1;
	bu->changeSpd = 1;
	bu->changeLck = 1;
	bu->changeDef = 1;
	bu->changeRes = 1;
	BU_CHG_MAG(bu) = 1;
}

STATIC_DECLAR void UnitLvup_0(struct BattleUnit *bu, int bonus) {}

STATIC_DECLAR void UnitLvupCore(struct BattleUnit *bu, int bonus)
{
	static void (*const funcs[])(struct BattleUnit *bu, int bonus) = {
		[0] = UnitLvup_Vanilla,
		[1] = UnitLvup_RandC,
		[2] = UnitLvup_Fixed,
		[3] = UnitLvup_100,
		[4] = UnitLvup_0
	};

	int mode, total_lvup, i, retry;

	if (TUTORIAL_MODE())
		mode = gpKernelDesigerConfig->lvup_mode_tutorial;
	if (gPlaySt.config.controller || (gPlaySt.chapterStateBits & PLAY_FLAG_HARD))
		mode = gpKernelDesigerConfig->lvup_mode_hard;
	else
		mode = gpKernelDesigerConfig->lvup_mode_normal;

	if (mode > 4)
		mode = 0;

	funcs[mode](bu, bonus);

	/**
	 * Retry
	 */
	retry = gpKernelDesigerConfig->guaranteed_lvup ? 10 : 0;

	for (i = 0; i < retry; i++) {
		total_lvup = bu->changeHP + bu->changePow + bu->changeSkl + bu->changeSpd +
						bu->changeLck + bu->changeDef + bu->changeRes + BU_CHG_MAG(bu);

		if (total_lvup != 0)
			break;

		funcs[mode](bu, bonus + 10);
	}
}

STATIC_DECLAR int get_metis_tome_growth_bonus(void)
{
	return MetisTomeGrowthBonus;
}

LYN_REPLACE_CHECK(CheckBattleUnitLevelUp);
void CheckBattleUnitLevelUp(struct BattleUnit *bu)
{
	if (CanBattleUnitGainLevels(bu) && bu->unit.exp >= 100) {
		int bonus = (bu->unit.state & US_GROWTH_BOOST) ? get_metis_tome_growth_bonus() : 0;

		bu->unit.exp -= 100;
		bu->unit.level++;

		if (UNIT_CATTRIBUTES(&bu->unit) & CA_MAXLEVEL10) {
			if (bu->unit.level == 10) {
				bu->expGain -= bu->unit.exp;
				bu->unit.exp = UNIT_EXP_DISABLED;
			}
		} else if (bu->unit.level == UNIT_LEVEL_MAX_RE ||
				 UNIT_RECORDED_LEVEL_MAX == (bu->unit.level + GetUnitHiddenLevel(&bu->unit))) {
			bu->expGain -= bu->unit.exp;
			bu->unit.exp = UNIT_EXP_DISABLED;
		}

		TryAddSkillLvup(GetUnitFromCharIdAndFaction(UNIT_CHAR_ID(&bu->unit), FACTION_BLUE), bu->unit.level);
		UnitLvupCore(bu, bonus);

		CheckBattleUnitStatCaps(GetUnit(bu->unit.index), bu);
	}
}
