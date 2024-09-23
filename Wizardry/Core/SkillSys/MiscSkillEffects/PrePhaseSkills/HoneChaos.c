#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

#if (defined(SID_HoneChaos) && COMMON_SKILL_VALID(SID_HoneChaos))
STATIC_DECLAR void AddTargetForChaos(struct Unit * unit)
{
    if (UNIT_ALIVE(unit) && AreUnitsAllied(gSubjectUnit->index, unit->index))
        AddTarget(unit->xPos, unit->yPos, unit->index, 1);
}
#endif

#if (defined(SID_HoneChaos) && COMMON_SKILL_VALID(SID_HoneChaos))
STATIC_DECLAR void MakeTargetListForChaos(struct Unit * unit)
{
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;
    BmMapFill(gBmMapRange, 0);
    MapAddInBoundedRange(x, y, 1, 2);

    InitTargets(x, y);
    ForEachUnitInRange(AddTargetForChaos);
}
#endif

#if (defined(SID_HoneChaos) && COMMON_SKILL_VALID(SID_HoneChaos))
static void _SetHoneChaosStatDebuff(struct Unit * unit, struct Unit * unit_ally, int hone_id)
{
    switch (hone_id)
    {
        case 0:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_POW);
            return;
        case 1:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_MAG);
            return;
        case 2:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_SKL);
            return;
        case 3:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_SPD);
            return;
        case 4:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_LCK);
            return;
        case 5:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_DEF);
            return;
        case 6:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_RES);
            return;
        case 7:
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_HONE_MOV);
            return;
    }
}
#endif

bool PrePhase_TickHoneChaosSkillStatus(ProcPtr proc)
{
#if (defined(SID_HoneChaos) && COMMON_SKILL_VALID(SID_HoneChaos))
    int i, j;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit * unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (SkillTester(unit, SID_HoneChaos))
        {
            MakeTargetListForChaos(unit);

            int rally_id = NextRN_N(8);

            for (j = 0; j < GetSelectTargetCount(); j++)
            {
                struct Unit * unit_ally = GetUnit(GetTarget(j)->uid);

                StartLightRuneAnim(proc, unit->xPos, unit->yPos);
                _SetHoneChaosStatDebuff(unit, unit_ally, rally_id);
            }
        }
    }
#endif
    return false;
}