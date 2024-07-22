#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

u8 HealingFocus_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (GetUnitCurrentHp(gActiveUnit) == GetUnitMaxHp(gActiveUnit))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

u8 HealingFocus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_HealingFocus_FRtext);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = SID_HealingFocus;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void post_skill(ProcPtr proc)
{
    struct MuProc * mu;
    HideUnitSprite(gActiveUnit);

    mu = GetUnitMu(gActiveUnit);
    if (!mu)
        mu = StartMu(gActiveUnit);

    SetMuDefaultFacing(mu);
}

static void call_heal(ProcPtr proc)
{
    CallMapAnim_Heal(proc, gActiveUnit, Div(GetUnitMaxHp(gActiveUnit) * SKILL_EFF0(SID_HealingFocus), 100));
}

bool Action_HealingFocus(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, post_skill, call_heal);
    return true;
}
