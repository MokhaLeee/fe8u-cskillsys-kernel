#include <common-chax.h>
#include <kernel-lib.h>
#include <list-verify.h>
#include <skill-system.h>
#include <stat-screen.h>

#include <shield.h>

#define LOCAL_TRACE 0

extern struct {
	struct UnitListHeader header;
	const struct ShieldInfo *sinfo;
} sShileldInfoCache[4];

extern u8 sShileldInfoNext;

STATIC_DECLAR bool check_config_en(void)
{
	return gpKernelDesigerConfig->shield_en;
}

NOINLINE STATIC_DECLAR bool get_unit_item_slot(struct Unit *unit, u16 item)
{
	int i;

	for (i = 0; i < UNIT_ITEM_COUNT; i++)
		if (ITEM_INDEX(item) == ITEM_INDEX(unit->items[i]))
			return i;

	return -1;
}

NOINLINE STATIC_DECLAR void consume_unit_item(struct Unit *unit, u16 item)
{
	int slot = get_unit_item_slot(unit, item);

	if (slot >= 0)
		unit->items[slot] = GetItemAfterUse(unit->items[slot]);
}

STATIC_DECLAR void WriteShileInfoCache(struct Unit *unit, const struct ShieldInfo *sinfo)
{
	WriteUnitList(unit, &sShileldInfoCache[sShileldInfoNext].header);
	sShileldInfoCache[sShileldInfoNext].sinfo = sinfo;

	sShileldInfoNext = (sShileldInfoNext + 1) & 3;
}

STATIC_DECLAR const struct ShieldInfo *GetShieldInfoFromCache(struct Unit *unit)
{
	int i;

	for (i = 0; i < 4; i++)
		if (JudgeUnitList(unit, &sShileldInfoCache[i].header))
			return sShileldInfoCache[i].sinfo;

	return NULL;
}

STATIC_DECLAR void ResetShieldInfo(void)
{
	memset(&sShileldInfoCache, 0, sizeof(sShileldInfoCache));

	sShileldInfoNext = 0;
	gpActorShileInfo = gpTargetShileInfo = NULL;
}

NOINLINE STATIC_DECLAR const struct ShieldInfo *GetExtUnitShield(struct Unit *unit)
{
	int slot;
	u8 pid = UNIT_CHAR_ID(unit);
	u8 jid = UNIT_CLASS_ID(unit);
	const struct ShieldExtEquipableConfig *it;

	for (it = gpShieldExtEquipableConfig; it->iid != ITEM_NONE; it++) {
		if (it->pid != 0 && it->pid != pid)
			continue;

		if (it->jid != 0 && it->jid != jid)
			continue;

		if (it->evflag != 0 && !CheckFlag(it->evflag))
			continue;

		if (COMMON_SKILL_VALID(it->skill) && !SkillListTester(unit, it->skill))
			continue;

		slot = get_unit_item_slot(unit, it->iid);
		if (slot >= 0)
			return &it->info;
	}

	return NULL;
}

const struct ShieldInfo *GetUnitShield(struct Unit *unit)
{
	int i;
	const struct ShieldInfo *sinfo;

	if (!check_config_en())
		return NULL;

	sinfo = GetShieldInfoFromCache(unit);
	if (sinfo)
		return sinfo;

	if (gpKernelDesigerConfig->shield_ext_equip_config_en) {
		sinfo = GetExtUnitShield(unit);

		if (sinfo != NULL) {
			WriteShileInfoCache(unit, sinfo);
			return sinfo;
		}
	}

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		int item = unit->items[i];
		const struct ShieldItem *sitem = &gpShieldItemTable[ITEM_INDEX(item)];

		if (!sitem->en)
			continue;

		sinfo = &sitem->info;

		// This is a bug on iinfo from designers
		if (sinfo->iid != ITEM_INDEX(item)) {
			Errorf("Shield item info wrong: item=%02X", ITEM_INDEX(item));
			continue;
		}

		WriteShileInfoCache(unit, sinfo);
		return sinfo;
	}

	return NULL;
}

const struct ShieldInfo *GetBattleUnitShield(struct BattleUnit *bu)
{
	Assert(bu == &gBattleActor || bu == &gBattleTarget);

	if (bu == &gBattleActor)
		return gpActorShileInfo;
	else if (bu == &gBattleTarget)
		return gpTargetShileInfo;

	return NULL;
}

/**
 * Battle hit
 */
void BattleHit_ConsumeShield(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	const struct ShieldInfo *sinfo;

	if (!check_config_en())
		return;

	/**
	 * Attacker
	 */
	sinfo = GetBattleUnitShield(attacker);
	if (sinfo && sinfo->iid != ITEM_NONE && sinfo->consume_on_attack)
		if (!sinfo->consume_on_hitted || !(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
			consume_unit_item(&attacker->unit, sinfo->iid);

	/**
	 * Defender
	 */
	sinfo = GetBattleUnitShield(defender);
	if (sinfo && sinfo->iid != ITEM_NONE && sinfo->consume_on_defense)
		if (!sinfo->consume_on_hitted || !(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
			consume_unit_item(&attacker->unit, sinfo->iid);
}

/**
 * U2B
 */
void UnitToBattle_SetShieldInfo(struct Unit *unit, struct BattleUnit *bu)
{
	/**
	 * We hold the same judgement as: UnitToBattle_SetupSkillList
	 */
	if (bu == &gBattleTarget) {
		gpActorShileInfo  = GetUnitShield(GetUnit(gBattleActor.unit.index));
		gpTargetShileInfo = GetUnitShield(GetUnit(gBattleTarget.unit.index));
	}
}

void BattleToUnit_ResetShieldInfo(struct BattleUnit *bu, struct Unit *unit)
{
	ResetShieldInfo();
}

void GameInit_ResetShieldInfo(void)
{
	ResetShieldInfo();
}

/**
 * Pre-battle calc hooks
 */
void PreBattleCalcShieldBonus(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	const struct ShieldInfo *sinfo = GetBattleUnitShield(attacker);

	if (!check_config_en())
		return;

	if (sinfo) {
		LTRACEF("item=0x%02X, atk_bonus=%d", sinfo->iid, sinfo->battle_status_bonus[BATTLE_STATUS_ATK]);

		attacker->battleAttack       += sinfo->battle_status_bonus[BATTLE_STATUS_ATK];
		attacker->battleDefense      += sinfo->battle_status_bonus[BATTLE_STATUS_DEF];
		attacker->battleSpeed        += sinfo->battle_status_bonus[BATTLE_STATUS_AS];
		attacker->battleHitRate      += sinfo->battle_status_bonus[BATTLE_STATUS_HIT];
		attacker->battleAvoidRate    += sinfo->battle_status_bonus[BATTLE_STATUS_AVO];
		attacker->battleCritRate     += sinfo->battle_status_bonus[BATTLE_STATUS_CRIT];
		attacker->battleDodgeRate    += sinfo->battle_status_bonus[BATTLE_STATUS_DODGE];
		attacker->battleSilencerRate += sinfo->battle_status_bonus[BATTLE_STATUS_SILENCER];
	}
}

/**
 * Statscreen
 */
void DrawItemPage_ShieldEquipLine(void)
{
	int line;
	const struct ShieldInfo *sinfo = GetUnitShield(gStatScreen.unit);

	if (!sinfo)
		return;

	line = get_unit_item_slot(gStatScreen.unit, sinfo->iid);

	if (line >= 0 && line < CHAX_ITEM_PAGE_AMT) {
		PutSpecialChar(
			gUiTmScratchA + TILEMAP_INDEX(16, 1 + line * 2),
			TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_35);

		CallARM_FillTileRect(
			gUiTmScratchC + TILEMAP_INDEX(1, 2 + line * 2),
			gpTSA_ItemEquipLine, TILEREF(0x40, STATSCREEN_BGPAL_3));
	}
}

/**
 * Status getter
 */
int PowGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_POW];

	return status;
}

int MagGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_MAG];

	return status;
}

int SklGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_SKL];

	return status;
}

int SpdGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_SPD];

	return status;
}

int LckGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_LCK];

	return status;
}

int DefGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_DEF];

	return status;
}

int ResGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_RES];

	return status;
}

int MovGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_status_bonus[UNIT_STATUS_MOV];

	return status;
}

/**
 * Growth getter
 */
int PowGrowthGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_growth_bonus[UNIT_STATUS_POW];

	return status;
}

int MagGrowthGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_growth_bonus[UNIT_STATUS_MAG];

	return status;
}

int SklGrowthGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_growth_bonus[UNIT_STATUS_SKL];

	return status;
}

int SpdGrowthGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_growth_bonus[UNIT_STATUS_SPD];

	return status;
}

int LckGrowthGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_growth_bonus[UNIT_STATUS_LCK];

	return status;
}

int DefGrowthGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_growth_bonus[UNIT_STATUS_DEF];

	return status;
}

int ResGrowthGetterShield(int status, struct Unit *unit)
{
	const struct ShieldInfo *sinfo = GetUnitShield(unit);

	if (sinfo)
		status += sinfo->unit_growth_bonus[UNIT_STATUS_RES];

	return status;
}
