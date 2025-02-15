#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"

s8 CanUnitRefuge(struct Unit* actor, struct Unit* target);
void TryAddUnitToRefugeTargetList(struct Unit* unit);
void MakeRefugeTargetList(struct Unit* unit);
void MakeRefugeTargetListForAdjacentAlly(struct Unit * unit);
extern void ForEachAdjacentUnit(int x, int y, void (*)(struct Unit *));

s8 CanUnitRefuge(struct Unit* actor, struct Unit* target) {
    int actorCon  = UNIT_CON(actor);
    int targetAid = GetUnitAid(target);

    return (actorCon <= targetAid) ? TRUE : FALSE;
}

void TryAddUnitToRefugeTargetList(struct Unit* unit) {

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (gSubjectUnit->pClassData->number == CLASS_PHANTOM || unit->pClassData->number == CLASS_PHANTOM) {
        return;
    }

    if (unit->statusIndex == UNIT_STATUS_BERSERK) {
        return;
    }

    if (unit->state & (US_RESCUING | US_RESCUED)) {
        return;
    }

    if (!CanUnitRefuge(gSubjectUnit, unit)) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeRefugeTargetList(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachAdjacentUnit(x, y, TryAddUnitToRefugeTargetList);

    return;
}

u8 Refuge_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }

    if (gActiveUnit->state & (US_IN_BALLISTA | US_RESCUING)) {
        return MENU_NOTSHOWN;
    }

    MakeRefugeTargetList(gActiveUnit);

    if (GetSelectTargetCount() == 0) {
        return MENU_NOTSHOWN;
    }

    return MENU_ENABLED;
}

static u8 Refuge_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    Action_Refuge(proc);

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A |
        TARGETSELECTION_ACTION_CLEARBGS;
}

void MakeRefugeTargetListForAdjacentAlly(struct Unit * unit)
{
    InitTargets(0, 0);
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachAdjacentUnit(x, y, TryAddUnitToRefugeTargetList);

    return;
}

u8 Refuge_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        //MenuFrozenHelpBox(menu, MSG_MenuSkill_Refuge_FRtext);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeRefugeTargetListForAdjacentAlly(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, Refuge_OnSelectTarget),
        GetStringFromIndex(MSG_MenuSkill_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

bool Action_Refuge(ProcPtr parent)
{
    struct Unit * targetUnit = GetUnit(gActionData.targetIndex);

    /* Set the states of both units */
    targetUnit->state  |= US_RESCUING;
    gActiveUnit->state |= US_RESCUED | US_HIDDEN;

    targetUnit->rescue = gActiveUnit->index;
    gActiveUnit->rescue = targetUnit->index;

    // gActiveUnit->xPos = targetUnit->xPos;
    // gActiveUnit->yPos = targetUnit->yPos;

    gActiveUnit = targetUnit;
    

    /* Make the target unit the active unit */
    gActionData.subjectIndex = targetUnit->index;
    gActionData.targetIndex = gActiveUnit->index;

    /* Give the new active unit their old target unit map coordinates */
    gActionData.xMove = gActionData.xOther;
    gActionData.yMove = gActionData.yOther;

    if (!(targetUnit->state & US_HAS_MOVED))
        gActionDataExpa.refrain_action = true;

    return true;
}