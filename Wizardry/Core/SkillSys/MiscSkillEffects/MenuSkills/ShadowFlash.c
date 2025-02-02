#include "common-chax.h"
#include "map-anims.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"
#include "event-rework.h"
#include "bmtarget.h"
#include "weapon-range.h"

#if defined(SID_ShadowFlash) && (COMMON_SKILL_VALID(SID_ShadowFlash))

static void TryAddToShadowFlashTargetList(int x, int y) {
    if (gBmMapUnit[y][x] != 0) {
        return;
    }

    if (TerrainTable_MovCost_FlyNormal[gBmMapTerrain[y][x]] <= 0) {
        return;
    }

    AddTarget(x, y, 0, 0);

    return;
}

void ForEachEmptySpaceInRange(int range, void(*func)(int x, int y)) {
    int x = gSubjectUnit->xPos;
    int y = gSubjectUnit->yPos;

    InitTargets(x, y);

    for (int ix = x - range; ix <= x + range; ix++) {
        for (int iy = y - range; iy <= y + range; iy++) {
            if (ix < 0 || iy < 0 || ix >= gBmMapSize.x || iy >= gBmMapSize.y) {
                continue;
            }

            /* Calculate Manhattan distance */
            if (abs(ix - x) + abs(iy - y) <= range) {
                /* Exclude the starting position of the unit */
                if (ix == x && iy == y) {
                    continue;
                }
                if (gBmMapUnit[iy][ix] == 0) {
                    func(ix, iy);
                }
            }
        }
    }
}

static void MakeTargetListForShadowFlash(struct Unit* unit) {
    BmMapFill(gBmMapRange, 0);

    ForEachEmptySpaceInRange(SKILL_EFF1(SID_ShadowFlash), TryAddToShadowFlashTargetList);

    return;
}

u8 ShadowFlash_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (CheckBitUES(gActiveUnit, UES_BIT_SHADOWFLASH_SKILL_USED))
        return MENU_NOTSHOWN;

    if (GetUnitCurrentHp(gActiveUnit) <= SKILL_EFF0(SID_ShadowFlash))
        return MENU_DISABLED;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForShadowFlash))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

int ShadowFlash_Hover(struct MenuProc * menu, struct MenuItemProc * item)
{
    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);
    int maxRange = SKILL_EFF1(SID_ShadowFlash);
    MapAddInRange(gActiveUnit->xPos, gActiveUnit->yPos, maxRange,     +1);
    DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
    return 0;
}

int ShadowFlash_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    HideMoveRangeGraphics();
    return 0;
}


STATIC_DECLAR void PrepareMenuShadowFlash(void)
{
    EndAllMus();
    RefreshUnitSprites();
}

static void set_actor_unit(void)
{
    gEventSlots[EVT_SLOT_2] = gActiveUnit->index;
}

static void set_position(void) 
{
    /* This ensures the unit is in the right position when FlushIn is used */
    gActiveUnit->xPos = gActionData.xOther;
    gActiveUnit->yPos = gActionData.yOther;

    /* This ensures the unit's final position is not reset after FlushIn is used */
    gActionData.xMove = gActionData.xOther;
    gActionData.yMove = gActionData.yOther;

    AddUnitHp(gActiveUnit, -SKILL_EFF0(SID_ShadowFlash));
}

static void move_again(void)
{
    gActionDataExpa.refrain_action = true;
    EndAllMus();
}


static u8 ShadowFlash_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_ShadowFlash;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 ShadowFlash_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
        return MENU_ACT_SND6B;
    }

    /* Removes the menu while targeting */
    ClearBg0Bg1();

    MakeTargetListForShadowFlash(gActiveUnit);

    // BmMapFill(gBmMapMovement, -1);
    // BmMapFill(gBmMapRange, 0);
    // int maxRange = SKILL_EFF1(SID_ShadowFlash);
    // MapAddInRange(gActiveUnit->xPos, gActiveUnit->yPos, maxRange,     +1);
    // DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, ShadowFlash_OnSelectTarget),
        GetStringFromIndex(0x87E));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

STATIC_DECLAR const EventScr EventScr_MenuShadowFlash[] = {

LABEL(0)
    CALL(EventScr_MuSkillAnim)
    ASMC(PrepareMenuShadowFlash)
    ASMC(set_actor_unit)
    CALL(EventScr_UidFlushingOUT)
    STAL(20)
    ASMC(set_position)
    CALL(EventScr_UidFlushingIN)
    STAL(20)
    ASMC(move_again)

LABEL(99)
    NOFADE
    ENDA
};


bool Action_ShadowFlash(ProcPtr parent)
{
    SetBitUES(gActiveUnit, UES_BIT_SHADOWFLASH_SKILL_USED);
    KernelCallEvent(EventScr_MenuShadowFlash, EV_EXEC_CUTSCENE, parent);
    return true;
}
#endif
