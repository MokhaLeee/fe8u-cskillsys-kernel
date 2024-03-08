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

void StartStatusHealEffect(struct Unit * unit, ProcPtr proc);

bool PostActionAlertStance(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;

    if (gActionData.unitActionType == UNIT_ACTION_WAIT)
    {
        if (SkillTester(unit, SID_AlertStancePlus))
        {
            SetUnitStatus(unit, NEW_UNIT_STATUS_AVOID_PLUS);
            MU_EndAll();
            StartStatusHealEffect(unit, parent);
            return true;
        }

        if (SkillTester(unit, SID_AlertStance))
        {
            SetUnitStatus(unit, NEW_UNIT_STATUS_AVOID);
            MU_EndAll();
            StartStatusHealEffect(unit, parent);
            return true;
        }
    }
    return false;
}
