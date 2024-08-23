#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(TryAddUnitToSleepTargetList);
void TryAddUnitToSleepTargetList(struct Unit* unit) {

#if (defined(SID_Insomnia) && (COMMON_SKILL_VALID(SID_Insomnia)))
    if (SkillTester(unit, SID_Insomnia))
        return;
#endif

    if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (unit->statusIndex != UNIT_STATUS_NONE && unit->statusIndex != UNIT_STATUS_SLEEP) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}