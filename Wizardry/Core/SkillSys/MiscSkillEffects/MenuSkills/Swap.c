#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"


#if defined(SID_Swap) && (COMMON_SKILL_VALID(SID_Swap))
u8 Swap_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForWarp))
        return MENU_DISABLED;

    if (CheckBitUES(gActiveUnit, UES_BIT_SWAP_SKILL_USED))
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

STATIC_DECLAR void PrepareMenuPositionSwap(void)
{
    EndAllMus();
    RefreshUnitSprites();
}

static void set_target_unit(void)
{
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);
    gEventSlots[EVT_SLOT_2] = unit_tar->index;
}

static void set_actor_unit(void)
{
    gEventSlots[EVT_SLOT_2] = gActiveUnit->index;
}

static void set_position(void) 
{
    struct Unit * unita = gActiveUnit;
    struct Unit * unitb = GetUnit(gActionData.targetIndex);

    int x = unita->xPos;
    int y = unita->yPos;

    unita->xPos = unitb->xPos;
    unita->yPos = unitb->yPos;

    gActionData.xMove = unitb->xPos;
    gActionData.yMove = unitb->yPos;

    unitb->xPos = x;
    unitb->yPos = y;
}

static u8 Swap_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_Swap;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 Swap_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_Swap_FRtext);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForWarp(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_Rescue, Swap_OnSelectTarget),
        GetStringFromIndex(MSG_MenuSkill_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

STATIC_DECLAR const EventScr EventScr_MenuPositionSwap[] = {

LABEL(0)
    SVAL(EVT_SLOT_B, SID_Swap)
    CALL(EventScr_MuSkillAnim)
    ASMC(PrepareMenuPositionSwap)
    ASMC(set_actor_unit)
    CALL(EventScr_UidWarpOUT)
    STAL(20)
    ASMC(set_target_unit)
    CALL(EventScr_UidFlushingOUT)
    STAL(60)
    ASMC(set_position)
    ASMC(set_target_unit)
    CALL(EventScr_UidFlushingIN)
    STAL(20)
    ASMC(set_actor_unit)
    CALL(EventScr_UidWarpIN)
    STAL(20)

LABEL(99)
    NoFade
    ENDA
};

bool Action_Swap(ProcPtr parent)
{
    SetBitUES(gActiveUnit, UES_BIT_SWAP_SKILL_USED);
    KernelCallEvent(EventScr_MenuPositionSwap, EV_EXEC_CUTSCENE, parent);
    
#if defined(SID_GridMaster) && (COMMON_SKILL_VALID(SID_GridMaster))
    if (SkillTester(gActiveUnit, SID_GridMaster))
    {
        gActionDataExpa.refrain_action = true;
        EndAllMus();
    }
#endif

    return true;
}
#endif