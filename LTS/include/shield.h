#pragma once

#include <common-chax.h>
#include <kernel-lib.h>
#include <list-verify.h>

struct ShieldInfo {
	u8 iid;
	u8 consume_on_attack, consume_on_defense;
	u8 consume_on_hitted;

	s8 battle_status_bonus[BATTLE_STATUS_MAX];
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

void BattleHit_ConsumeShield(struct BattleUnit *attacker, struct BattleUnit *defender);
void UnitToBattle_SetShieldInfo(struct Unit *unit, struct BattleUnit *bu);
void BattleToUnit_ResetShieldInfo(struct BattleUnit *bu, struct Unit *unit);
void GameInit_ResetShieldInfo(void);
void PreBattleCalcShieldBonus(struct BattleUnit *attacker, struct BattleUnit *defender);
void DrawItemPage_ShieldEquipLine(void);
