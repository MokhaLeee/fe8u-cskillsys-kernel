#pragma once

#include <common-chax.h>
#include <kernel-lib.h>
#include <list-verify.h>

struct ShieldInfo {
	u8 iid;
	u8 consume_on_attack, consume_on_defense;
	u8 consume_on_hitted;

	s8 battle_status_bonus[BATTLE_STATUS_MAX];
	s8 unit_status_bonus[UNIT_STATUS_MAX - 1];
	s8 unit_growth_bonus[UNIT_STATUS_MAX - 1];
	u16 skills[2];
};

struct ShieldItem {
	u8 en;
	u8 _pad_[3];

	struct ShieldInfo info;
};

// extern struct ShieldItem const gShieldItemTable[0x100];
extern struct ShieldItem const *const gpShieldItemTable;

struct ShieldExtEquipableConfig {
	u8 iid, pid, jid, level;
	u16 evflag, skill;

	struct ShieldInfo info;
};

// extern struct ShieldExtEquipableConfig const gShieldExtEquipableConfig[];
extern struct ShieldExtEquipableConfig const *const gpShieldExtEquipableConfig;

extern const struct ShieldInfo *gpActorShileInfo;
extern const struct ShieldInfo *gpTargetShileInfo;

const struct ShieldInfo *GetUnitShield(struct Unit *unit);
const struct ShieldInfo *GetBattleUnitShield(struct BattleUnit *bu);
void BattleHit_ConsumeShield(struct BattleUnit *attacker, struct BattleUnit *defender);
void UnitToBattle_SetShieldInfo(struct Unit *unit, struct BattleUnit *bu);
void BattleToUnit_ResetShieldInfo(struct BattleUnit *bu, struct Unit *unit);
void GameInit_ResetShieldInfo(void);
void PreBattleCalcShieldBonus(struct BattleUnit *attacker, struct BattleUnit *defender);
void DrawItemPage_ShieldEquipLine(void);

int PowGetterShield(int status, struct Unit *unit);
int MagGetterShield(int status, struct Unit *unit);
int SklGetterShield(int status, struct Unit *unit);
int SpdGetterShield(int status, struct Unit *unit);
int LckGetterShield(int status, struct Unit *unit);
int DefGetterShield(int status, struct Unit *unit);
int ResGetterShield(int status, struct Unit *unit);
int MovGetterShield(int status, struct Unit *unit);

int PowGrowthGetterShield(int status, struct Unit *unit);
int MagGrowthGetterShield(int status, struct Unit *unit);
int SklGrowthGetterShield(int status, struct Unit *unit);
int SpdGrowthGetterShield(int status, struct Unit *unit);
int LckGrowthGetterShield(int status, struct Unit *unit);
int DefGrowthGetterShield(int status, struct Unit *unit);
int ResGrowthGetterShield(int status, struct Unit *unit);
int MovGrowthGetterShield(int status, struct Unit *unit);
