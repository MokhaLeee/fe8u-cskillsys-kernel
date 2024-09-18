#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

u8 ArdentSacrifice_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForAdjacentHeal))
		return MENU_DISABLED;

    if (GetUnitCurrentHp(gActiveUnit) <= SKILL_EFF0(SID_ArdentSacrifice))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

static u8 ArdentSacrifice_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_ArdentSacrifice;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 ArdentSacrifice_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
 if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_ArdentSacrifice_FRtext);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForAdjacentHeal(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, ArdentSacrifice_OnSelectTarget),
        GetStringFromIndex(MSG_MenuSkill_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void ActionArdentSacrifice_CallBack1(ProcPtr proc)
{
    struct MuProc * mu;
    struct Unit * target = GetUnit(gActionData.targetIndex);

    mu = GetUnitMu(target);
    if (!mu)
        mu = StartMu(target);

    FreezeSpriteAnim(mu->sprite_anim);
    SetMuDefaultFacing(mu);
    SetDefaultColorEffects();

    NewSkillMapAnimMini(gActiveUnit->xPos, gActiveUnit->yPos, SID_ArdentSacrifice, proc);
}

bool Action_ArdentSacrifice(ProcPtr parent)
{
    CallMapAnim_HealExt(
        parent,
        GetUnit(gActionData.targetIndex),
        SKILL_EFF0(SID_ArdentSacrifice),
        ActionArdentSacrifice_CallBack1,
        NULL);

    CallMapAnim_HurtExt(parent, gActiveUnit, SKILL_EFF0(SID_ArdentSacrifice), NULL, NULL);
    return true;
}
