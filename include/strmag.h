#pragma once

#include "common-chax.h"
#include "status-getter.h"

#define UNIT_MAG(unit) ((unit)->_u47)
#define BU_CHG_MAG(bu) (*((s8 *)(bu) + 0x7F))
#define ITEM_MAG_BONUS(bonuses) (*((const s8 *)bonuses + 9))

struct UnitMagicInfo {
	s8 base;
	s8 growth;
	s8 cap;
	s8 bonus;
};

extern const struct UnitMagicInfo gMagicPInfos[0x100];
extern const struct UnitMagicInfo gMagicJInfos[0x100];

extern struct UnitMagicInfo const *const gpMagicPInfos;
extern struct UnitMagicInfo const *const gpMagicJInfos;

bool IsMagicAttack(struct BattleUnit *bu);
int GetUnitMaxMagic(struct Unit *unit);
int GetUnitBaseMagic(struct Unit *unit);
int GetUnitBasicMagGrowth(struct Unit *unit);
int GetUnitJobBasedBasicMagGrowth(struct Unit *unit);
int GetClassChgMagicBonus(u8 jid);
int GetUnitMagic(struct Unit *unit);

extern const struct ItemStatBonuses ItemBonus_MagBooster;
