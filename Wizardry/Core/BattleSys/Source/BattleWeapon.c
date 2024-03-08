#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "bmitem.h"
#include "bmarena.h"
#include "bmarch.h"

#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "debuff.h"
#include "weapon-range.h"
#include "constants/items.h"

STATIC_DECLAR void SetBattleUnitWeaponVanilla(struct BattleUnit * bu, int itemSlot)
{
    if (itemSlot == BU_ISLOT_AUTO)
        itemSlot = GetUnitEquippedWeaponSlot(&bu->unit);

    if (bu->unit.state & US_IN_BALLISTA)
        itemSlot = BU_ISLOT_BALLISTA;

    bu->canCounter = TRUE;

    switch (itemSlot) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        bu->weaponSlotIndex = itemSlot;
        bu->weapon = bu->unit.items[bu->weaponSlotIndex];
        break;

    case BU_ISLOT_5:
        bu->weaponSlotIndex = 0xFF;
        bu->weapon = gBmSt.um_tmp_item;
        break;

    case BU_ISLOT_ARENA_PLAYER:
        bu->weaponSlotIndex = 0;
        bu->weapon = gArenaState.playerWeapon;
        bu->canCounter = FALSE;
        break;

    case BU_ISLOT_ARENA_OPPONENT:
        bu->weaponSlotIndex = 0;
        bu->weapon = gArenaState.opponentWeapon;
        bu->canCounter = FALSE;
        break;

    case BU_ISLOT_BALLISTA:
        bu->weaponSlotIndex = 0xFF;
        bu->weapon = GetBallistaItemAt(bu->unit.xPos, bu->unit.yPos);
        bu->canCounter = FALSE;
        break;

    default:
        bu->weaponSlotIndex = 0xFF;
        bu->weapon = 0;
        bu->canCounter = FALSE;
        break;

    }

    bu->weaponBefore = bu->weapon;
    bu->weaponAttributes = GetItemAttributes(bu->weapon);
    bu->weaponType = GetItemType(bu->weapon);

    if (!(gBattleStats.config & BATTLE_CONFIG_BIT2))
    {
        if (bu->weaponAttributes & IA_MAGICDAMAGE)
        {
            switch (GetItemIndex(bu->weapon)) {

            case ITEM_SWORD_WINDSWORD:
                // if (gBattleStats.range == 2)
                if (gBattleStats.range >= 2)
                    bu->weaponType = ITYPE_ANIMA;
                else
                    bu->weaponAttributes = bu->weaponAttributes &~ IA_MAGICDAMAGE;
                break;

            case ITEM_SWORD_LIGHTBRAND:
                // if (gBattleStats.range == 2)
                if (gBattleStats.range >= 2)
                    bu->weaponType = ITYPE_LIGHT;
                else
                    bu->weaponAttributes = bu->weaponAttributes &~ IA_MAGICDAMAGE;
                break;

            case ITEM_SWORD_RUNESWORD:
                bu->weaponType = ITYPE_DARK;
                break;
            }
        }

#ifndef CHAX
        if (!IsItemCoveringRange(bu->weapon, gBattleStats.range) || bu->weaponSlotIndex == 0xFF)
        {
            bu->weapon = 0;
            bu->canCounter = FALSE;
        }

        switch (bu->unit.statusIndex) {
        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            bu->weapon = 0;
            bu->canCounter = FALSE;

            break;

        }
#endif
    }
}

STATIC_DECLAR void PostSetBattleUnitWeaponVanillaHook(struct BattleUnit * bu, int slot)
{
    if (!(gBattleStats.config & BATTLE_CONFIG_BIT2))
    {
        if (!IsItemCoveringRangeRework(bu->weapon, gBattleStats.range, &bu->unit))
        {
            bu->weapon = 0;
            bu->canCounter = FALSE;
        }

        if (bu->weaponSlotIndex == 0xFF)
        {
            bu->weapon = 0;
            bu->canCounter = FALSE;
        }

        switch (GetUnitStatusIndex(&bu->unit)) {
        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            bu->weapon = 0;
            bu->canCounter = FALSE;
            break;
        };
    }
}

/* LynJump */
void SetBattleUnitWeapon(struct BattleUnit * bu, int slot)
{
    int cid;

    SetBattleUnitWeaponVanilla(bu, slot);
    PostSetBattleUnitWeaponVanillaHook(bu, slot);

    /* Check combat-art */
    cid = GetCombatArtInForce(&bu->unit);
    if (COMBART_VALID(cid))
    {
        const struct CombatArtInfo * info = &gpCombatArtInfos[cid];

        if (info->magic_attack)
            bu->weaponAttributes |= IA_MAGICDAMAGE | IA_MAGIC;
    }
}
