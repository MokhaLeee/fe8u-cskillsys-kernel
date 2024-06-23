#include "global.h"
#include "bmunit.h"
#include "mu.h"
#include "proc.h"
#include "bmmind.h"
#include "bmmind.h"

#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

bool PostActionAlertStance(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;

    if (gActionData.unitActionType == UNIT_ACTION_WAIT)
    {
#if defined(SID_AlertStancePlus) && (COMMON_SKILL_VALID(SID_AlertStancePlus))
        if (SkillTester(unit, SID_AlertStancePlus))
        {
            SetUnitStatus(unit, NEW_UNIT_STATUS_AVOID_PLUS);
            MU_EndAll();
            StartStatusHealEffect(unit, parent);
            return true;
        }
#endif

#if defined(SID_AlertStance) && (COMMON_SKILL_VALID(SID_AlertStance))
        if (SkillTester(unit, SID_AlertStance))
        {
            SetUnitStatus(unit, NEW_UNIT_STATUS_AVOID);
            MU_EndAll();
            StartStatusHealEffect(unit, parent);
            return true;
        }
#endif
    }
    return false;
}
