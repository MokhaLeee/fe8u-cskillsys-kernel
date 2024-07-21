#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

u8 LightRune_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (GetUnitCurrentHp(gActiveUnit) <= SKILL_EFF0(SID_LightRune))
        return MENU_DISABLED;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForLightRune))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

static u8 LightRune_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_LightRune;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 LightRune_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForLightRune(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, LightRune_OnSelectTarget),
        GetStringFromIndex(0x87E));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

bool Action_LightRune(ProcPtr parent)
{
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex), gActionData.itemSlotIndex);
    gBattleActor.canCounter = false;
    AddLightRune(gActionData.xOther, gActionData.yOther);
    BattleApplyItemEffect(parent);
    StartLightRuneAnim(parent, gActionData.xOther, gActionData.yOther);
    gBattleTarget.statusOut = -1;
    gBattleActor.hasItemEffectTarget = 0; // seems unused?
    AddUnitHp(&gBattleActor.unit, -SKILL_EFF0(SID_LightRune));
    return true;
}
