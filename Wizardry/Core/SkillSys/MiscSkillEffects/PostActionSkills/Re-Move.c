#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

extern u8 gPostActionReMoveFlag;

bool PostActionReMove(ProcPtr parent)
{
#if defined(SID_ReMove) && (COMMON_SKILL_VALID(SID_ReMove))

    struct Unit * unit = GetUnit(gActionData.subjectIndex);

    gPostActionReMoveFlag = false;

    if (SkillTester(gActiveUnit, SID_ReMove) && Roll1RN(unit->skl))
    {
        gPostActionReMoveFlag = true;
        MU_EndAll();
        StartStatusHealEffect(gActiveUnit, parent);
        return true;
    }
#endif /* defined(SID_ReMove) */
    return false;
}
