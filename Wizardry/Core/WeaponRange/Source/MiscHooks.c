#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "bmmap.h"
#include "bmitemuse.h"
#include "bmidoten.h"
#include "playerphase.h"
#include "bmarch.h"
#include "bmphase.h"
#include "cp_common.h"
#include "bmmind.h"

#include "common-chax.h"
#include "battle-system.h"
#include "status-getter.h"
#include "weapon-range.h"

/* LynJump */
bool AiReachesByBirdsEyeDistance(struct Unit * unit, struct Unit * other, u16 item)
{
    int distance = RECT_DISTANCE(unit->xPos, unit->yPos, other->xPos, other->yPos);
    if (distance <= MovGetter(unit) + GetItemMaxRangeRework(item, unit))
        return true;

    return false;
}

/* LynJump */
bool AiCouldReachByBirdsEyeDistance(struct Unit * unit, struct Unit * other, u16 item)
{

    int distance = RECT_DISTANCE(unit->xPos, unit->yPos, other->xPos, other->yPos);
    if (distance <= MovGetter(unit) + MovGetter(other) + GetItemMaxRangeRework(item, unit))
        return true;

    return false;
}

/* LynJump */
int GetUnitWeaponReachBits(struct Unit * unit, int slot)
{
    int i, item, result = 0;

    switch (slot) {
    case -1:
        for (i = 0; (i < UNIT_ITEM_COUNT) && (item = unit->items[i]); ++i)
            if (CanUnitUseWeapon(unit, item))
                result |= GetItemReachBitsRework(item, unit);
        break;

    default:
        item = GetItemFormSlot(unit, slot);
        result = GetItemReachBitsRework(item, unit);
        break;
    }

    return result;
}

/* LynJump */
int GetUnitItemUseReachBits(struct Unit * unit, int slot)
{
    int i;
    u16 item;
    u32 mask = 0;

    switch (slot) {
    case -1:
        for (i = 0; i < UNIT_ITEM_COUNT; i++)
        {
            item = unit->items[i];

            if (ITEM_INDEX(item) > 0 && CanUnitUseItem(unit, item))
                mask |= GetItemReachBitsRework(item, unit);
        }
        break;

    default:
        item = GetItemFormSlot(unit, slot);
        mask = REACH_NONE;
        if (CanUnitUseItem(unit, item))
            mask = GetItemReachBitsRework(item, unit);

        break;
    }
    return mask;
}

/* LynJump */
int GetUnitStaffReachBits(struct Unit * unit)
{
    int i;
    u16 item;
    u32 mask = 0;

    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        item = unit->items[i];

        if (ITEM_INDEX(item) != 0 && CanUnitUseStaff(unit, item))
            mask |= GetItemReachBitsRework(item, unit);
    }
    return mask;
}

/* LynJump */
void AiFillReversedAttackRangeMap(struct Unit * unit, u16 item)
{
    BmMapFill(gBmMapRange, 0);

    // <!> not unit! here is target! we should judge for AI!
    AddMap(unit->xPos, unit->yPos, GetItemReachBitsRework(item, gActiveUnit), 1, 0);
}

/* LynJump */
void AiFloodMovementAndRange(struct Unit * unit, u16 move, u16 item) {
    int ix, iy;
    u32 mask;

    SetWorkingMoveCosts(GetUnitMovementCost(unit));
    SetWorkingBmMap(gBmMapMovement);
    GenerateMovementMap(unit->xPos, unit->yPos, move, unit->index);
    BmMapFill(gBmMapRange, 0);

    mask = GetItemReachBitsRework(item, unit);

    for (iy = gBmMapSize.y - 1; iy >= 0; iy--)
    {
        for (ix = gBmMapSize.x - 1; ix >= 0; ix--)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            AddMap(unit->xPos, unit->yPos, mask, 1, 0);
        }
    }
}

/* LynJump */
int AiGetInRangeCombatPositionScoreComponent(int x, int y, struct Unit * unit)
{
    int dist = RECT_DISTANCE(unit->xPos, unit->yPos, x, y);
    u16 item = GetUnitEquippedWeapon(unit);

    if (ITEM_INDEX(item) == 0)
        return 0;

    if (dist > GetItemMaxRangeRework(item, unit))
        if (dist < GetItemMinRangeRework(item, unit))
            return 50;

    return 0;
}

/* LynJump */
void DisplayUnitEffectRange(struct Unit * unit)
{
    u32 movelimitv_flag = MOVLIMITV_MMAP_BLUE;

    GenerateUnitMovementMapExt(gActiveUnit, MovGetter(gActiveUnit) - gActionData.moveCount);

    if (!(gActiveUnit->state & US_CANTOING))
    {
        BmMapFill(gBmMapOther, 0);

        if (UnitHasMagicRank(unit))
            GenerateMagicSealMap(1);

        BmMapFill(gBmMapRange, 0);

        switch (GetUnitWeaponUsabilityBits(gActiveUnit)) {
        case (UNIT_USEBIT_STAFF | UNIT_USEBIT_WEAPON):
            if (gBmSt.swapActionRangeCount & 1)
            {
                GenerateUnitCompleteStaffRange(gActiveUnit);
                movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN;
            }
            else
            {
                GenerateUnitCompleteAttackRange(gActiveUnit);
                movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_RED;
            }
            break;

        case UNIT_USEBIT_STAFF:
            GenerateUnitCompleteStaffRange(gActiveUnit);
            movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN;
            break;
        case UNIT_USEBIT_WEAPON:
            GenerateUnitCompleteAttackRange(gActiveUnit);
            movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_RED;
            break;
        } /* switch */
    }
    DisplayMoveRangeGraphics(movelimitv_flag);
}

/* LynJump */
void GenerateUnitMovementMap(struct Unit * unit)
{
    SetWorkingMoveCosts(GetUnitMovementCost(unit));
    SetWorkingBmMap(gBmMapMovement);

    GenerateMovementMap(unit->xPos, unit->yPos, MovGetter(unit), unit->index);
}

/* LynJump */
void GenerateUnitCompleteAttackRange(struct Unit * unit)
{
    int ix, iy;
    u32 mask = GetUnitWeaponReachBits(unit, -1);

    BmMapFill(gBmMapRange, 0);

    if (UNIT_CATTRIBUTES(unit) & CA_BALLISTAE)
    {
        u16 item = GetBallistaItemAt(unit->xPos, unit->yPos);
        if (0 != item)
            mask |= GetItemReachBitsRework(item, unit);
    }

    for (iy = 0; iy < gBmMapSize.y; iy++)
    {
        for (ix = 0; ix < gBmMapSize.x; ix++)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            if (gBmMapUnit[iy][ix])
                continue;

            if (gBmMapOther[iy][ix])
                continue;

            AddMap(ix, iy, mask, 1, 0);
        }
    }
    SetWorkingBmMap(gBmMapMovement);
}

/* LynJump */
void GenerateUnitStandingReachRange(struct Unit * unit, int mask)
{
    BmMapFill(gBmMapRange, 0);
    AddMap(unit->xPos, unit->yPos, mask, 1, 0);
}

/* LynJump */
void GenerateUnitCompleteStaffRange(struct Unit * unit)
{
    int ix, iy;
    u32 mask = GetUnitStaffReachBits(unit);

    BmMapFill(gBmMapRange, 0);

    if (UNIT_CATTRIBUTES(unit) & CA_BALLISTAE)
    {
        u16 item = GetBallistaItemAt(unit->xPos, unit->yPos);
        if (0 != item)
            mask |= GetItemReachBitsRework(item, unit);
    }

    for (iy = 0; iy < gBmMapSize.y; iy++)
    {
        for (ix = 0; ix < gBmMapSize.x; ix++)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            if (gBmMapUnit[iy][ix])
                continue;

            if (gBmMapOther[iy][ix])
                continue;

            AddMap(ix, iy, mask, 1, 0);
        }
    }
    SetWorkingBmMap(gBmMapMovement);
}

/* LynJump */
void GenerateDangerZoneRange(bool boolDisplayStaffRange)
{
    int i, enemyFaction;
    int hasMagicRank, prevHasMagicRank;
    u8 savedUnitId;

    prevHasMagicRank = -1;

    BmMapFill(gBmMapRange, 0);

    enemyFaction = GetNonActiveFaction();

    for (i = enemyFaction + 1; i < enemyFaction + 0x80; ++i)
    {
        struct Unit * unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue; // not a unit

        if (boolDisplayStaffRange && !UnitHasMagicRank(unit))
            continue; // no magic in magic range mode

        if (gPlaySt.chapterVisionRange && (gBmMapFog[unit->yPos][unit->xPos] == 0))
            continue; // in the fog

        if (unit->state & US_UNDER_A_ROOF)
            continue; // under a roof

        // Fill movement map for unit
        GenerateUnitMovementMapExt(unit, MovGetter(unit));

        savedUnitId = gBmMapUnit[unit->yPos][unit->xPos];
        gBmMapUnit[unit->yPos][unit->xPos] = 0;

        hasMagicRank = UnitHasMagicRank(unit);

        if (prevHasMagicRank != hasMagicRank)
        {
            BmMapFill(gBmMapOther, 0);

            if (hasMagicRank)
                GenerateMagicSealMap(1);

            prevHasMagicRank = hasMagicRank;
        }

        SetWorkingBmMap(gBmMapRange);

        // Apply unit's range to range map

        if (boolDisplayStaffRange)
            GenerateUnitCompleteStaffRange(unit);
        else
            GenerateUnitCompleteAttackRange(unit);

        gBmMapUnit[unit->yPos][unit->xPos] = savedUnitId;
    }
}

/* LynJump */
void FillMovementAndRangeMapForItem(struct Unit * unit, u16 item)
{
    int ix, iy;

    GenerateUnitMovementMap(unit);
    BmMapFill(gBmMapRange, 0);

    for (iy = 0; iy < gBmMapSize.y; iy++)
    {
        for (ix = 0; ix < gBmMapSize.x; ix++)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            AddMap(ix, iy, GetItemReachBitsRework(item, unit), 1, 0);
        }
    }
}

/* LynJump */
void sub_803B678(struct Unit * unit, u16 item)
{
    int ix, iy;

    GenerateUnitMovementMap(unit);
    BmMapFill(gBmMapRange, 0);

    for (iy = gBmMapSize.y - 1; iy >= 0; iy--)
    {
        for (ix = gBmMapSize.x - 1; ix >= 0; ix--)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            AddMap(ix, iy, GetItemReachBitsRework(item, unit), 1, 0);
        }
    }
}

/* LynJump */
void SetupUnitHealStaffAIFlags(struct Unit * unit, u16 item)
{
    int flags = 0;

    if ((GetItemAttributes(item) & IA_WEAPON) && (GetItemMaxRangeRework(item, unit) > 1))
        flags = AI_UNIT_FLAG_6;

    switch (GetItemUseEffect(item)) {
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x21:
    case 0x22:
        flags |= AI_UNIT_FLAG_2;
        break;
    }

    unit->aiFlags |= flags;
}
