#include "common-chax.h"
#include "kernel-lib.h"
#include "battle-system.h"
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
        MenuFrozenHelpBox(menu, MSG_SkillEffect_HealingFocus_Fail);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = SID_HealingFocus;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

struct ProcActionSkillHealingFocus {
    PROC_HEADER;
    int heal;
};

STATIC_DECLAR void ActionSkillHealingFocus_Init(struct ProcActionSkillHealingFocus * proc)
{
    MapAnim_CommonInit();
}

STATIC_DECLAR void ActionSkillHealingFocus_MoveCamera(ProcPtr proc)
{
    EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

STATIC_DECLAR void ActionSkillHealingFocus_ExecAnim(struct ProcActionSkillHealingFocus * proc)
{
    HideUnitSprite(gActiveUnit);
    BeginUnitHealAnim(gActiveUnit, proc->heal);
}

STATIC_DECLAR void ActionSkillHealingFocus_ExecBmHeal(struct ProcActionSkillHealingFocus * proc)
{
    AddUnitHp(gActiveUnit, proc->heal);
}

STATIC_DECLAR void ActionSkillHealingFocus_End(struct ProcActionSkillHealingFocus * proc)
{
    MapAnim_CommonEnd();
    RestoreBattleRoundInfo();
}

STATIC_DECLAR const struct ProcCmd ProcScr_ActionSkillHealingFocus[] = {
    PROC_CALL(ActionSkillHealingFocus_Init),
    PROC_CALL(ActionSkillHealingFocus_MoveCamera),
    PROC_YIELD,
    PROC_CALL(ActionSkillHealingFocus_ExecAnim),
    PROC_YIELD,
    PROC_CALL(ActionSkillHealingFocus_ExecBmHeal),
    PROC_CALL(ActionSkillHealingFocus_End),
    PROC_END
};

bool HealingFocus_Action(ProcPtr parent)
{
    struct ProcActionSkillHealingFocus * proc;
    proc = Proc_StartBlocking(ProcScr_ActionSkillHealingFocus, parent);
    proc->heal = Div(GetUnitMaxHp(gActiveUnit) * SKILL_EFF0(SID_HealingFocus), 100);
    return true;
}
