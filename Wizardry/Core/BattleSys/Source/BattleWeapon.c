#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "debuff.h"
#include "weapon-range.h"
#include "weapon-lockex.h"
#include "constants/items.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

STATIC_DECLAR void SetBattleUnitWeaponVanilla(struct BattleUnit *bu, int itemSlot)
{
    if (itemSlot == BU_ISLOT_AUTO)
        itemSlot = GetUnitEquippedWeaponSlot(&bu->unit);

    if (bu->unit.state & US_IN_BALLISTA)
        itemSlot = BU_ISLOT_BALLISTA;

    bu->canCounter = true;

    switch (itemSlot)
    {
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
        bu->canCounter = false;
        break;

    case BU_ISLOT_ARENA_OPPONENT:
        bu->weaponSlotIndex = 0;
        bu->weapon = gArenaState.opponentWeapon;
        bu->canCounter = false;
        break;

    case BU_ISLOT_BALLISTA:
        bu->weaponSlotIndex = 0xFF;
        bu->weapon = GetBallistaItemAt(bu->unit.xPos, bu->unit.yPos);
        bu->canCounter = false;
        break;

    default:
        bu->weaponSlotIndex = 0xFF;
        bu->weapon = 0;
        bu->canCounter = false;
        break;
    }

    bu->weaponBefore = bu->weapon;
    bu->weaponAttributes = GetItemAttributes(bu->weapon);
    bu->weaponType = GetItemType(bu->weapon);

    if (!(gBattleStats.config & BATTLE_CONFIG_BIT2))
    {
        if (bu->weaponAttributes & IA_MAGICDAMAGE)
        {
            switch (GetItemIndex(bu->weapon))
            {

            case ITEM_SWORD_WINDSWORD:
                // if (gBattleStats.range == 2)
                if (gBattleStats.range >= 2)
                    bu->weaponType = ITYPE_ANIMA;
                else
                    bu->weaponAttributes = bu->weaponAttributes & ~IA_MAGICDAMAGE;
                break;

            case ITEM_SWORD_LIGHTBRAND:
                // if (gBattleStats.range == 2)
                if (gBattleStats.range >= 2)
                    bu->weaponType = ITYPE_LIGHT;
                else
                    bu->weaponAttributes = bu->weaponAttributes & ~IA_MAGICDAMAGE;
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
            bu->canCounter = false;
        }

        switch (bu->unit.statusIndex)
        {
        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            bu->weapon = 0;
            bu->canCounter = false;

            break;
        }
#endif
    }
}

STATIC_DECLAR void PostSetBattleUnitWeaponVanillaHook(struct BattleUnit *bu, int slot)
{
    FORCE_DECLARE int gBattleActorWeapon;
    FORCE_DECLARE int gBattleTargetWeapon;

    if (!(gBattleStats.config & BATTLE_CONFIG_BIT2))
    {
        if (!IsItemCoveringRangeRework(bu->weapon, gBattleStats.range, &bu->unit))
        {
            bu->weapon = 0;
            bu->canCounter = false;
            return;
        }

        if (bu->weaponSlotIndex == 0xFF)
        {
            bu->weapon = 0;
            bu->canCounter = false;
            return;
        }

#if (defined(SID_Mimic) && (COMMON_SKILL_VALID(SID_Mimic)))
        if (BattleSkillTester(bu, SID_Mimic))
        {
            gBattleTargetWeapon = GetUnitEquippedWeapon(GetUnit(gBattleTarget.unit.index));
            gBattleActorWeapon = GetUnitEquippedWeapon(GetUnit(gBattleActor.unit.index));

            if (bu == &gBattleActor)
            {
                bu->weapon = gBattleTargetWeapon;
                bu->weaponBefore = gBattleTargetWeapon;
                bu->weaponType = gBattleTargetWeapon;
            }
            else
            {
                bu->weapon = gBattleActorWeapon;
                bu->weaponBefore = gBattleActorWeapon;
                bu->weaponType = gBattleActorWeapon;
            }
        }
#endif

#if (defined(SID_Dazzle) && (COMMON_SKILL_VALID(SID_Dazzle)))
        if (bu == &gBattleTarget && BattleSkillTester(&gBattleActor, SID_Dazzle))
        {
            bu->weapon = 0;
            bu->canCounter = false;
            return;
        }
#endif

#if (defined(SID_Moonlight) && (COMMON_SKILL_VALID(SID_Moonlight)))
        if (bu == &gBattleTarget && BattleSkillTester(&gBattleActor, SID_Moonlight))
        {
            bu->weapon = 0;
            bu->canCounter = false;
            return;
        }
#endif

#if (defined(SID_Comatose) && (COMMON_SKILL_VALID(SID_Comatose)))
        if (BattleSkillTester(bu, SID_Comatose))
            return;
        else
        {
            switch (GetUnitStatusIndex(&bu->unit))
            {
            case UNIT_STATUS_SLEEP:
            case UNIT_STATUS_PETRIFY:
            case UNIT_STATUS_13:
                bu->weapon = 0;
                bu->canCounter = false;
                return;

            default:
                break;
            };
        }
#else
        switch (GetUnitStatusIndex(&bu->unit))
        {
        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            bu->weapon = 0;
            bu->canCounter = false;
            return;

        default:
            break;
        };
#endif
    }
}

LYN_REPLACE_CHECK(SetBattleUnitWeapon);
void SetBattleUnitWeapon(struct BattleUnit *bu, int slot)
{
    int cid;

    SetBattleUnitWeaponVanilla(bu, slot);
    PostSetBattleUnitWeaponVanillaHook(bu, slot);

    /* Check combat-art */
    cid = GetCombatArtInForce(&bu->unit);
    if (COMBART_VALID(cid))
    {
        if (GetCombatArtInfo(cid)->magic_attack)
            bu->weaponAttributes |= IA_MAGICDAMAGE | IA_MAGIC;
    }
}

LYN_REPLACE_CHECK(CanUnitUseWeapon);
s8 CanUnitUseWeapon(struct Unit *unit, int item)
{
    if (item == 0)
        return false;

    if (!(GetItemAttributes(item) & IA_WEAPON))
        return false;

#if (defined(SID_Amische) && (COMMON_SKILL_VALID(SID_Amische)))
    if (SkillTester(unit, SID_Amische))
        switch(ITEM_INDEX(item)) {
        case ITEM_SWORD_IRON:
        case ITEM_LANCE_IRON:
        case ITEM_AXE_IRON:
        case ITEM_BOW_IRON:
            break;
        default:
            return false;
        } 
#endif

    if (GetItemAttributes(item) & IA_LOCK_ANY)
    {
        // Check for item locks

        if ((GetItemAttributes(item) & IA_LOCK_1) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_1))
            return false;

        if ((GetItemAttributes(item) & IA_LOCK_4) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_4))
            return false;

        if ((GetItemAttributes(item) & IA_LOCK_5) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_5))
            return false;

        if ((GetItemAttributes(item) & IA_LOCK_6) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_6))
            return false;

        if ((GetItemAttributes(item) & IA_LOCK_7) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_7))
            return false;

        if ((GetItemAttributes(item) & IA_LOCK_2) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_2))
            return false;

        // Monster lock is special
        if (GetItemAttributes(item) & IA_LOCK_3)
        {
            if (!(UNIT_CATTRIBUTES(unit) & CA_LOCK_3))
                return false;

            return true;
        }

        if (GetItemAttributes(item) & IA_UNUSABLE)
            if (!(IsItemUnsealedForUnit(unit, item)))
                return false;
    }

#if CHAX
    if ((GetUnitStatusIndex(unit) == UNIT_STATUS_SILENCED) && (GetItemAttributes(item) & IA_MAGIC))
#else
    if ((unit->statusIndex == UNIT_STATUS_SILENCED) && (GetItemAttributes(item) & IA_MAGIC))
#endif
        return false;

#if CHAX
    switch (CheckWeaponLockEx(unit, item))
    {
    case 1:
        return true;

    case -1:
        return false;
    }
#endif

#if (defined(SID_ShadowgiftPlus) && (COMMON_SKILL_VALID(SID_ShadowgiftPlus)))
    if (SkillTester(unit, SID_ShadowgiftPlus))
        if (GetItemType(item) == ITYPE_DARK)
            if (unit->ranks[ITYPE_DARK] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Shadowgift) && (COMMON_SKILL_VALID(SID_Shadowgift)))
    if (SkillTester(unit, SID_Shadowgift))
        if (GetItemType(item) == ITYPE_DARK)
            if (unit->ranks[ITYPE_DARK] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

#if (defined(SID_LuminaPlus) && (COMMON_SKILL_VALID(SID_LuminaPlus)))
    if (SkillTester(unit, SID_LuminaPlus))
        if (GetItemType(item) == ITYPE_LIGHT)
            if (unit->ranks[ITYPE_LIGHT] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Lumina) && (COMMON_SKILL_VALID(SID_Lumina)))
    if (SkillTester(unit, SID_Lumina))
        if (GetItemType(item) == ITYPE_LIGHT)
            if (unit->ranks[ITYPE_LIGHT] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

    return (unit->ranks[GetItemType(item)] >= GetItemRequiredExp(item)) ? true : false;
}

LYN_REPLACE_CHECK(CanUnitUseWeaponNow);
s8 CanUnitUseWeaponNow(struct Unit* unit, int item) {

    if (item == 0)
        return FALSE;

    if (!(GetItemAttributes(item) & IA_WEAPON))
        return FALSE;

    if ((GetItemAttributes(item) & IA_MAGIC) && IsUnitMagicSealed(unit))
        return FALSE;

    return CanUnitUseWeapon(unit, item);
}

int GetWeaponCost(struct BattleUnit *bu, u16 item)
{
    int cost;

    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return 0;

    cost = 1;

    if (bu == &gBattleActor)
    {
        int cid = GetCombatArtInForce(&bu->unit);
        if (COMBART_VALID(cid))
        {
            int _cost = GetCombatArtInfo(cid)->cost;
            if (_cost > 1)
                cost = _cost;
        }
    }

#if (defined(SID_FaerghusAncestry) && (COMMON_SKILL_VALID(SID_FaerghusAncestry)))
    if (BattleSkillTester(bu, SID_FaerghusAncestry))
        cost = cost * 2;
#endif

    LIMIT_AREA(cost, 0, 255);
    return cost;
}
