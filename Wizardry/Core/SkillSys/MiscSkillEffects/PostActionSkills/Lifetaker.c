#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionLifetaker(ProcPtr parent)
{
#if defined(SID_Lifetaker) && (COMMON_SKILL_VALID(SID_Lifetaker))
    struct Unit * unit_act = gActiveUnit;
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
        if (SkillTester(unit_act, SID_Lifetaker))
            if (unit_tar->curHP == 0)
            {
                int restoreAmount = unit_act->maxHP / 4;

                if(unit_act->curHP + restoreAmount > unit_act->maxHP)
                    unit_act->curHP = unit_act->maxHP;
                else
                    unit_act->curHP += restoreAmount;

                MU_EndAll();
                StartStatusHealEffect(unit_act, restoreAmount);
                return true;
            }
        break;
    }
#endif /* defined(SID_Lifetaker) */
    return false;
}