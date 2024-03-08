#pragma once

#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "status-getter.h"

#define UNIT_MAG(unit) ((unit)->_u47)
#define BU_CHG_MAG(bu) (*((u8 *)(bu) + 0x7F))
#define ITEM_MAG_BONUS(bonuses) *((const s8 *)bonuses + 9)

struct UnitMagicInfo {
    s8 base;
    s8 growth;
    s8 cap;
    s8 bonus;
};

extern const struct UnitMagicInfo gMagicPInfos[0x100];
extern const struct UnitMagicInfo gMagicJInfos[0x100];

extern struct UnitMagicInfo const * const gpMagicPInfos;
extern struct UnitMagicInfo const * const gpMagicJInfos;

static inline bool IsMagicAttack(struct BattleUnit * bu)
{
    return !!((IA_MAGICDAMAGE | IA_MAGIC) & bu->weaponAttributes);
}

static inline int GetUnitMaxMagic(struct Unit * unit)
{
    int status = gpMagicJInfos[UNIT_CLASS_ID(unit)].cap;

    if (status <= 0)
        return 20;

    return status;
}

static inline int GetUnitBaseMagic(struct Unit * unit)
{
    return gpMagicPInfos[UNIT_CHAR_ID(unit)].base + gpMagicJInfos[UNIT_CLASS_ID(unit)].base;
}

static inline int GetUnitBasicMagGrowth(struct Unit * unit)
{
    return gpMagicPInfos[UNIT_CHAR_ID(unit)].growth;
}

static inline int GetClassChgMagicBonus(u8 jid)
{
    return gpMagicJInfos[jid].bonus;
}

static inline int GetUnitMagic(struct Unit * unit)
{
    return MagGetter(unit);
}
