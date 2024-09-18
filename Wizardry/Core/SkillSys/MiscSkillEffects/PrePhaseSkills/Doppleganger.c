#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "class-pairs.h"

bool PrePhase_TickDopplegangerSkillStatus(ProcPtr proc)
{
    int i;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit *unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if (defined(SID_Doppleganger) && COMMON_SKILL_VALID(SID_Doppleganger))
        if (SkillTester(unit, SID_Doppleganger))
        {
            for (int i = 0; i < dopplegangerListSize; i++)
            {
                if (gActiveUnit->pCharacterData->number == dopplegangerPairs[i][0])
                {
                    unit->pClassData = GetClassData(dopplegangerPairs[i][1]);
                    break;
                }
            }
        }
#endif
    }
    return false;
}
