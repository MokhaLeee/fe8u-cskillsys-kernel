#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "hardware.h"
#include "bmbattle.h"
#include "uimenu.h"
#include "bmmenu.h"
#include "playerphase.h"
#include "bksel.h"

#include "common-chax.h"
#include "combat-art.h"
#include "constants/gfx.h"
#include "constants/combat-arts.h"

bool CanUnitPlayCombatArt(struct Unit * unit, u16 item)
{
    int i, wtype = GetItemType(item);
    struct CombatArtList * list = GetCombatArtList(unit, wtype);

    for (i = 0; i < list->amt; i++)
        if (gpCombatArtInfos[list->cid[i]].wtype == wtype)
            return true;

    return false;
}

/* Support for menu */
u8 GetBestRangeBonusCid(struct Unit * unit, u16 item)
{
    u8 ret = 0;
    int i, wtype = GetItemType(item);
    struct CombatArtList * list = GetCombatArtList(unit, wtype);

    for (i = 0; i < list->amt; i++)
    {
        if (gpCombatArtInfos[list->cid[i]].wtype != wtype)
            continue;

        if (gpCombatArtInfos[list->cid[i]].range_bonus > gpCombatArtInfos[ret].range_bonus)
            ret = list->cid[i];
    }
    return ret;
}

/* Icon Getter */
const u8 * GetCombatArtIcon(const u8 cid)
{
    u8 const * const default_icons[] = {
        [ITYPE_SWORD] = GFX_CombatArtIcon_SwordArtAtk,
        [ITYPE_LANCE] = GFX_CombatArtIcon_LanceArtAtk,
        [ITYPE_AXE]   = GFX_CombatArtIcon_AxeArtAtk,
        [ITYPE_BOW]   = GFX_CombatArtIcon_BowArtAtk,
    };

    if (COMBART_VALID(cid))
    {
        const struct CombatArtInfo * info = &gpCombatArtInfos[cid];
        if (info->icon)
            return info->icon;

        switch (info->wtype) {
        case ITYPE_SWORD:
        case ITYPE_LANCE:
        case ITYPE_AXE:
        case ITYPE_BOW:
            return default_icons[info->wtype];
        }
    }
    return NULL;
}

/* Weapon range getter */
int WeaponRangeGetterCombatArt(int range, struct Unit * unit, u16 item)
{
    u8 cid = GetCombatArtInForce(unit);

    if (COMBART_VALID(cid))
    {
        const struct CombatArtInfo * info = &gpCombatArtInfos[cid];
        if (GetItemType(item) == info->wtype)
            range += info->range_bonus;
    }
    return range;
}

/* Pre-battle calc */
void PreBattleCalcCombatArt(struct BattleUnit * bu, struct BattleUnit * defender)
{
    u8 cid = GetCombatArtInForce(&bu->unit);
    const struct CombatArtInfo * info;
    struct Unit * unit;

    if (!COMBART_VALID(cid))
        return;

    info = &gpCombatArtInfos[cid];
    unit = GetUnit(bu->unit.index);

    bu->battleAttack       += info->battle_status.atk;
    bu->battleDefense      += info->battle_status.def;
    bu->battleHitRate      += info->battle_status.hit;
    bu->battleAvoidRate    += info->battle_status.avo;
    bu->battleSilencerRate += info->battle_status.silencer;
    bu->battleDodgeRate    += info->battle_status.dodge;

    if (info->external_calc)
    {
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
        };
    }
}

/* LynJump */
void sub_8022E54(void)
{
    /* During reload from B pressed in select-target */
    ResetCombatArtStatus();
    UnitActionMenu_Attack(NULL, NULL);
}
