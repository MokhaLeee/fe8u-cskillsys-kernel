#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

STATIC_DECLAR void AddTargetForRally(struct Unit * unit)
{
    if (UNIT_ALIVE(unit) && AreUnitsAllied(gSubjectUnit->index, unit->index))
        AddTarget(unit->xPos, unit->yPos, unit->index, 1);
}

STATIC_DECLAR void MakeTargetListForRally(struct Unit * unit)
{
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;
    BmMapFill(gBmMapRange, 0);
    MapAddInBoundedRange(x, y, 1, 2);

    InitTargets(x, y);
    ForEachUnitInRange(AddTargetForRally);
}

static void _SetRallyChaosStatDebuff(struct Unit * unit, struct Unit * unit_ally, int rally_id)
{
    switch (rally_id) {
    case 0:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_POW);
        return;
    case 1:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_MAG);
        return;
    case 2:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_SKL);
        return;
    case 3:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_SPD);
        return;
    case 4:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_LCK);
        return;
    case 5:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_DEF);
        return;
    case 6:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_RES);
        return;
    case 7:
        SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_RALLY_MOV);
        return;
    }
}

bool PrePhase_TickRallyChaosSkillStatus(ProcPtr proc)
{
#if (defined(SID_RallyChaos) && COMMON_SKILL_VALID(SID_RallyChaos))
    int i, j;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit * unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (SkillTester(unit, SID_RallyChaos))
        {
            MakeTargetListForRally(unit);
            
            // With this we ensure each unit with this skill randomly generates their own rally
            int rally_id = NextRN_100() % 8;

            for (j = 0; j < GetSelectTargetCount(); j++)
            {
                struct Unit * unit_ally = GetUnit(GetTarget(j)->uid);
                if (!UNIT_ALIVE(unit_ally))
                    continue;

                StartLightRuneAnim(proc, unit->xPos, unit->yPos);
                _SetRallyChaosStatDebuff(unit, unit_ally, rally_id);
            }
        }
    }

    return false;
#endif
}
