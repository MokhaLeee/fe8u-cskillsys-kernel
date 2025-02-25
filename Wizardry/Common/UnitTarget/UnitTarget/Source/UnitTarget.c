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

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
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

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
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

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
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

LYN_REPLACE_CHECK(TryAddUnitToRescueTargetList);
void TryAddUnitToRescueTargetList(struct Unit* unit) {

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
        return;
#endif

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (gSubjectUnit->pClassData->number == CLASS_PHANTOM || unit->pClassData->number == CLASS_PHANTOM) {
        return;
    }

    if (unit->statusIndex == UNIT_STATUS_BERSERK) {
        return;
    }

    if (unit->state & (US_RESCUING | US_RESCUED)) {
        return;
    }

    if (!CanUnitRescue(gSubjectUnit, unit)) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToBarrierTargetList);
void TryAddUnitToBarrierTargetList(struct Unit* unit) {

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
        return;
#endif

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    if (unit->barrierDuration >= 7) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToWarpTargetList);
void TryAddUnitToWarpTargetList(struct Unit* unit) {

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
        return;
#endif

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToHammerneTargetList);
void TryAddUnitToHammerneTargetList(struct Unit* unit) {

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
        return;
#endif

    if (!IsSameAllegiance(gSubjectUnit->index, unit->index)) {
        return;
    }

    int i; // JESTER - moved the 'i' because otherwuse it risks becoming an unused variable during compilation

    for (i = 0; i < UNIT_ITEM_COUNT; i++) {
        if (IsItemHammernable(unit->items[i])) {
            AddTarget(unit->xPos, unit->yPos, unit->index, 0);
            break;
        }
    }

    return;
}
