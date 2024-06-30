#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionReMove(ProcPtr parent)
{
#if defined(SID_ReMove) && (COMMON_SKILL_VALID(SID_ReMove))
    struct Unit * unit = gActiveUnit;
    if (SkillTester(unit, SID_ReMove))
    {
        // MU_EndAll();
        // unit->state |= (US_UNSELECTABLE | US_CANTOING);
        unit->state |= US_HIDDEN;
        // StartStatusHealEffect(unit, parent);
        return true;
    }
#endif /* defined(SID_ReMove) */
    return false;
}
