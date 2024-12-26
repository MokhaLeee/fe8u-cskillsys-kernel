#include "common-chax.h"
#include "combat-art.h"
#include "skill-system.h"
#include "kernel-tutorial.h"
#include "constants/gfx.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

bool CanUnitPlayCombatArt(struct Unit *unit, u16 item)
{
	int i, wtype = GetItemType(item);
	struct CombatArtList *list = GetCombatArtList(unit, item);

	for (i = 0; i < list->amt; i++) {
		const struct CombatArtInfo *info = GetCombatArtInfo(list->cid[i]);

		if (info->wtype == CA_WTYPE_ANY || info->wtype == wtype)
			return true;
	}
	return false;
}

/* Support for menu */
u8 GetBestRangeBonusCid(struct Unit *unit, u16 item)
{
	u8 ret = 0;
	int i, wtype = GetItemType(item);
	struct CombatArtList *list = GetCombatArtList(unit, item);

	for (i = 0; i < list->amt; i++) {
		const struct CombatArtInfo *info = GetCombatArtInfo(list->cid[i]);

		if (info->wtype != CA_WTYPE_ANY && info->wtype != wtype)
			continue;

		if (info->range_bonus > GetCombatArtInfo(ret)->range_bonus)
			ret = list->cid[i];
	}
	return ret;
}

/* Icon Getter */
const u8 *GetCombatArtIcon(const u8 cid)
{
	u8 const *const default_icons[] = {
		[ITYPE_SWORD] = GFX_CombatArtIcon_SwordArtAtk,
		[ITYPE_LANCE] = GFX_CombatArtIcon_LanceArtAtk,
		[ITYPE_AXE]   = GFX_CombatArtIcon_AxeArtAtk,
		[ITYPE_BOW]   = GFX_CombatArtIcon_BowArtAtk,
	};

	if (COMBART_VALID(cid)) {
		const struct CombatArtInfo *info = GetCombatArtInfo(cid);

		if (info->icon)
			return info->icon;

		switch (info->wtype) {
		case ITYPE_SWORD:
		case ITYPE_LANCE:
		case ITYPE_AXE:
		case ITYPE_BOW:
			return default_icons[info->wtype];

		case CA_WTYPE_ANY:
			return default_icons[ITYPE_SWORD];
		}
	}
	return NULL;
}

/* Weapon range getter */
int WeaponRangeGetterCombatArt(int range, struct Unit *unit, u16 item)
{
	u8 cid = GetCombatArtInForce(unit);

	if (COMBART_VALID(cid)) {
		const struct CombatArtInfo *info = GetCombatArtInfo(cid);

		if (info->wtype == CA_WTYPE_ANY || info->wtype == GetItemType(item))
			range += info->range_bonus;
	}
	return range;
}

/* Pre-battle calc */
void PreBattleCalcCombatArt(struct BattleUnit *bu, struct BattleUnit *defender)
{
	FORCE_DECLARE int tmp1, tmp2;
	u8 cid = GetCombatArtInForce(&bu->unit);
	const struct CombatArtInfo *info;
	struct Unit *unit;

	if (!COMBART_VALID(cid))
		return;

	info = GetCombatArtInfo(cid);
	unit = GetUnit(bu->unit.index);

	bu->battleAttack	   += info->battle_status.atk;
	bu->battleDefense	  += info->battle_status.def;
	bu->battleHitRate	  += info->battle_status.hit;
	bu->battleAvoidRate	+= info->battle_status.avo;
	bu->battleCritRate	 += info->battle_status.crit;
	bu->battleSilencerRate += info->battle_status.silencer;
	bu->battleDodgeRate	+= info->battle_status.dodge;

	switch (cid) {
	case CID_Soulblade:
		bu->battleAttack += GetUnitResistance(unit);
		break;

	case CID_FinesseBlade:
		bu->battleAttack += GetUnitSkill(unit);
		break;

	case CID_FrozenLance:
		bu->battleAttack += GetUnitSkill(unit);
		break;

	case CID_LanceJab:
		bu->battleAttack += GetUnitSpeed(unit);
		break;

	case CID_Vengeance:
		bu->battleAttack += GetUnitMaxHp(unit) - GetUnitCurrentHp(unit);
		break;

	case CID_ArmoredStrike:
		bu->battleAttack += GetUnitDefense(unit);
		break;

	case CID_LightningAxe:
		bu->battleAttack += GetUnitResistance(unit);
		break;

	case CID_Detonate:
		if (!(GetItemAttributes(bu->weapon) & IA_UNBREAKABLE))
			bu->battleAttack += ITEM_USES(bu->weapon);

		break;

	case CID_BloodTribute:
#if (defined(SID_COMBAT_CrimsonStrike) && (COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike)))
		tmp1 = perc_of(bu->hpInitial, SKILL_EFF0(SID_COMBAT_BloodTribute));
		tmp2 = perc_of(tmp1, SKILL_EFF1(SID_COMBAT_BloodTribute));

		/**
		 * This is just for UI display.
		 * In real battle, the hp-cost should bind to current hp in each round,
		 * thus at which time we need to calculated the damage bonus at battle-hit.
		 */
		if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
			bu->battleAttack += tmp2;
#endif
		break;

	case CID_CrimsonStrike:
#if (defined(SID_COMBAT_CrimsonStrike) && (COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike)))
		tmp1 = perc_of(bu->hpInitial, SKILL_EFF0(SID_COMBAT_CrimsonStrike));
		tmp2 = perc_of(tmp1, SKILL_EFF1(SID_COMBAT_CrimsonStrike));

		/**
		 * Same as BloodTribute
		 */
		if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
			bu->battleAttack += tmp2;
#endif
		break;

	case CID_VitalReckoning:
#if (defined(SID_COMBAT_VitalReckoning) && (COMMON_SKILL_VALID(SID_COMBAT_VitalReckoning)))
		tmp1 = perc_of(bu->hpInitial, SKILL_EFF0(SID_COMBAT_VitalReckoning));
		tmp2 = perc_of(tmp1, SKILL_EFF1(SID_COMBAT_VitalReckoning));

		/**
		 * Same as BloodTribute
		 */
		if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
			bu->battleAttack += tmp2;
#endif
		break;
	};
}

/* Pre-battle generate */
void PreBattleGenerate_TrigCombartKTut(void)
{
	if (gBattleStats.config & BATTLE_CONFIG_REAL) {
		if (COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
			TriggerKtutorial(KTUTORIAL_COMBATART_MENU);
	}
}

LYN_REPLACE_CHECK(sub_8022E54);
void sub_8022E54(void)
{
	/* During reload from B pressed in select-target */
	ResetCombatArtStatus();
	UnitActionMenu_Attack(NULL, NULL);
}
