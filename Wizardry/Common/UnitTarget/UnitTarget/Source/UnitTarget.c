#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(TryAddUnitToSleepTargetList);
void TryAddUnitToSleepTargetList(struct Unit *unit)
{

#if (defined(SID_Insomnia) && (COMMON_SKILL_VALID(SID_Insomnia)))
    if (SkillTester(unit, SID_Insomnia))
        return;
#endif

#if (defined(SID_GoodAsGold) && (COMMON_SKILL_VALID(SID_GoodAsGold)))
    if (SkillTester(unit, SID_GoodAsGold))
        return;
#endif

    if (AreUnitsAllied(gSubjectUnit->index, unit->index))
    {
        return;
    }

    if (unit->statusIndex != UNIT_STATUS_NONE && unit->statusIndex != UNIT_STATUS_SLEEP)
    {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToSilenceTargetList);
void TryAddUnitToSilenceTargetList(struct Unit *unit)
{

#if (defined(SID_GoodAsGold) && (COMMON_SKILL_VALID(SID_GoodAsGold)))
    if (SkillTester(unit, SID_GoodAsGold))
        return;
#endif

    if (AreUnitsAllied(gSubjectUnit->index, unit->index))
    {
        return;
    }

    if (unit->statusIndex != UNIT_STATUS_NONE && unit->statusIndex != UNIT_STATUS_SILENCED)
    {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToBerserkTargetList);
void TryAddUnitToBerserkTargetList(struct Unit *unit)
{

#if (defined(SID_GoodAsGold) && (COMMON_SKILL_VALID(SID_GoodAsGold)))
    if (SkillTester(unit, SID_GoodAsGold))
        return;
#endif

    if (AreUnitsAllied(gSubjectUnit->index, unit->index))
    {
        return;
    }

    if (unit->statusIndex != UNIT_STATUS_NONE && unit->statusIndex != UNIT_STATUS_BERSERK)
    {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}