#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

bool PostAction_BattleActorHeal(ProcPtr parent)
{
    int heal = 0;
    struct Unit * unit = gActiveUnit;
    FORCE_DECLARE int hp_cur = GetUnitCurrentHp(unit);
    FORCE_DECLARE int hp_max = GetUnitMaxHp(unit);

    if (!UNIT_IS_VALID(unit) || (unit->state & (US_DEAD | US_BIT16)))
        return false;


#if defined(SID_Lifetaker) && (COMMON_SKILL_VALID(SID_Lifetaker))
    if (SkillTester(unit, SID_Lifetaker) && gBattleActorGlobalFlag.enimy_defeated)
        heal += hp_max / 4;
#endif

    if (heal == 0)
        return false;

    if ((heal >= (hp_max - hp_cur)))
        heal = hp_max - hp_cur;

    AddUnitHp(unit, heal);
    HideUnitSprite(unit);
    BeginUnitHealAnim(unit, heal);
    return true;
}