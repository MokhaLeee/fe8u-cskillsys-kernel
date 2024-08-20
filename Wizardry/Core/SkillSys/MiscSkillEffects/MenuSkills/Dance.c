#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "debuff.h"
#include "constants/skills.h"


LYN_REPLACE_CHECK(DanceCommandUsability);
u8 DanceCommandUsability(const struct MenuItemDef * def, int number)
{
#if !CHAX
    if (!(UNIT_CATTRIBUTES(gActiveUnit) & CA_DANCE))
        return MENU_NOTSHOWN;
#endif

    gBmSt.um_tmp_item = ITEM_DANCE;
    return sub_80230F0(def);
}


LYN_REPLACE_CHECK(RefreshMapSelect_Select);
u8 RefreshMapSelect_Select(ProcPtr proc, struct SelectTarget* target) {

    gActionData.unitActionType = UNIT_ACTION_DANCE;
    gActionData.targetIndex = target->uid;

#if defined(SID_VigorDance) && (COMMON_SKILL_VALID(SID_VigorDance))
    if (SkillTester(gActiveUnit, SID_VigorDance) & !gBattleActorGlobalFlag.skill_activated_vigor_dance)
    {
        gBattleActorGlobalFlag.skill_activated_vigor_dance = true;
        SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_BUFF_VIGORDANCE);
    }
#endif

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}