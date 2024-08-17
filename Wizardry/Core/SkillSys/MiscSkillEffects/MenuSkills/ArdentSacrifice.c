#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

u8 ArdentSacrifice_Usability(const struct MenuItemDef * def, int number)
{
    int i;

    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (GetUnitCurrentHp(gActiveUnit) < SKILL_EFF0(SID_ArdentSacrifice))
        return MENU_NOTSHOWN;

    MakeTargetListForAdjacentHeal(gActiveUnit);

    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        struct Unit * unit = GetUnit(GetTarget(i)->uid);
        if (GetUnitCurrentHp(unit) < GetUnitMaxHp(unit))
             return MENU_ENABLED;
    }

    return MENU_NOTSHOWN;
}

static u8 ArdentSacrifice_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

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
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForAdjacentHeal(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_Heal, ArdentSacrifice_OnSelectTarget),
        GetStringFromIndex(0x874));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void ActionArdentSacrifice_CallBack1(ProcPtr proc)
{
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);

    struct MuProc * mu;

    mu = GetUnitMu(gActiveUnit);
    if (!mu)
        mu = StartMu(gActiveUnit);

    FreezeSpriteAnim(mu->sprite_anim);
    SetMuDefaultFacing(mu);
    SetDefaultColorEffects();

    NewSkillMapAnimMini(unit_tar->xPos, unit_tar->yPos, SID_ArdentSacrifice, proc);
}

bool Action_ArdentSacrifice(ProcPtr parent)
{
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);

    CallMapAnim_HealExt(
        parent,
        unit_tar,
        SKILL_EFF1(SID_ArdentSacrifice),
        ActionArdentSacrifice_CallBack1,
        NULL);

    CallMapAnim_Hurt(parent, gActiveUnit, SKILL_EFF0(SID_ArdentSacrifice));
    return true;
}
