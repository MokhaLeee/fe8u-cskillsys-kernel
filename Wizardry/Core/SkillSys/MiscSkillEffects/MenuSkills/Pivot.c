#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"

#if defined(SID_Pivot) && (COMMON_SKILL_VALID(SID_Pivot))
struct Vec2u GetPivotCoord(int x1, int x2, int y1, int y2);
void TryPivotAllyToTargetList(struct Unit * unit);
void MakePivotTargetListForAdjacentAlly(struct Unit * unit);
extern void ForEachAdjacentUnit(int x, int y, void (*)(struct Unit *));

static inline bool IsPosInvaild(s8 x, s8 y)
{
    return ((x < 0) & (x > gBmMapSize.x) & (y < 0) & (y > gBmMapSize.y));
}

u8 Pivot_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakePivotTargetListForAdjacentAlly))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

static u8 Pivot_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_Pivot;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A |
        TARGETSELECTION_ACTION_CLEARBGS;
}

u8 Pivot_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_Pivot_FRtext);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakePivotTargetListForAdjacentAlly(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, Pivot_OnSelectTarget),
        GetStringFromIndex(MSG_MenuSkill_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void callback_anim(ProcPtr proc)
{
}

static void callback_exec(ProcPtr proc)
{
    gActiveUnit->state |= US_HAS_MOVED | US_CANTOING;
    // gActiveUnit->state &= ~US_CANTOING;
    struct Unit * targetUnit = GetUnit(gActionData.targetIndex);

    int x1 = gActiveUnit->xPos;
    int y1 = gActiveUnit->yPos;

    int x2 = targetUnit->xPos; // target
    int y2 = targetUnit->yPos; // target

    struct Vec2u dest = GetPivotCoord(x1, x2, y1, y2);

    gActionData.xMove = dest.x;
    gActionData.yMove = dest.y;
}

struct Vec2u GetPivotCoord(int x1, int x2, int y1, int y2)
{
    struct Vec2u result;
    result.x = x1;
    result.y = y1;
    // int dir = 0;
    if (x1 != x2)
    {
        if (x1 > x2)
        {
            // dir = MU_COMMAND_MOVE_RIGHT; // actor is on the right side of target, so move the actor to the left side
            result.x = x1 - 2;
        }
        else if (x1 < x2)
        {
            // dir = MU_COMMAND_MOVE_LEFT;
            result.x = x1 + 2;
        }
    }
    else if (y1 != y2)
    {
        if (y1 > y2)
        {
            // dir = MU_COMMAND_MOVE_DOWN;
            result.y = y1 - 2;
        }
        else if (y1 < y2)
        {
            // dir = MU_COMMAND_MOVE_UP;
            result.y = y1 + 2;
        }
    }
    return result;
}

void TryPivotAllyToTargetList(struct Unit * unit)
{

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index))
    {
        return;
    }

    if (unit->state & US_RESCUED)
    {
        return;
    }
    if (unit == gSubjectUnit)
    {
        return;
    }

    int x1 = gSubjectUnit->xPos;
    int x2 = unit->xPos; // target
    int y1 = gSubjectUnit->yPos;
    int y2 = unit->yPos; // target

    struct Vec2u dest = GetPivotCoord(x1, x2, y1, y2);

    if (IsPosInvaild(dest.x, dest.y))
    {
        return;
    }

    // if (gBmMapMovement[dest.y][dest.x] < 0xF) { // can we actually move there
    if (gBmMapUnit[dest.y][dest.x])
    {
        return; // dest sq is occupied
    }
    if (gBmMapHidden[dest.y][dest.x] & 1)
    {
        return; // hidden unit here
    }

// If the target unit has anchor they cannot be moved
#if defined(SID_Anchor) && (COMMON_SKILL_VALID(SID_Anchor))
    if (SkillTester(unit, SID_Anchor))
        return;
#endif

    if (CanUnitCrossTerrain(gSubjectUnit, gBmMapTerrain[dest.y][dest.x]))
    { // can we actually move there
        if (CanUnitCrossTerrain(unit, gBmMapTerrain[y1][x1]))
        { // can target be pulled onto here?
            AddTarget(unit->xPos, unit->yPos, unit->index, 0);
        }
    }

    return;
}

void MakePivotTargetListForAdjacentAlly(struct Unit * unit)
{
    InitTargets(0, 0);
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

/* Boost the range of this unit's movement skill */
#if defined(SID_Domain) && (COMMON_SKILL_VALID(SID_Domain))
    if (SkillTester(unit, SID_Domain))
    {
        MapAddInRange(x, y, 1 + SKILL_EFF0(SID_Domain), 1);
        ForEachUnitInRange(TryPivotAllyToTargetList);
    }
    else
        ForEachAdjacentUnit(x, y, TryPivotAllyToTargetList);

#else
    ForEachAdjacentUnit(x, y, TryPivotAllyToTargetList);
#endif

    return;
}

bool Action_Pivot(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}
#endif