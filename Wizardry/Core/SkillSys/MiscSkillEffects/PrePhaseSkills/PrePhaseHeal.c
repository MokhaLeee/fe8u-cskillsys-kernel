#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "strmag.h"

STATIC_DECLAR int GetPrePhaseHealAmount(struct Unit * unit)
{
    int ret = 0;

#if defined(SID_Renewal) && (COMMON_SKILL_VALID(SID_Renewal))
    if (SkillTester(unit, SID_Renewal))
        ret += Div(GetUnitMaxHp(unit) * SKILL_EFF0(SID_Renewal), 100);
#endif

#if defined(SID_Imbue) && (COMMON_SKILL_VALID(SID_Imbue))
    if (SkillTester(unit, SID_Imbue))
        ret += GetUnitMagic(unit);
#endif

#if defined(SID_Forager) && (COMMON_SKILL_VALID(SID_Forager))
    if (SkillTester(unit, SID_Forager))
    {
        const unsigned int terrainId = gBmMapTerrain[unit->yPos][unit->xPos];
        if(terrainId == 1/* plain */ || terrainId == 12 /* forest */ || terrainId == 17 /* mountain */)
            ret += Div(GetUnitMaxHp(unit) * SKILL_EFF0(SID_Forager), 100);
    }
#endif

#if defined(SID_RainDish) && (COMMON_SKILL_VALID(SID_RainDish))
    if (SkillTester(unit, SID_RainDish) && gPlaySt.chapterWeatherId == WEATHER_RAIN)
        ret += Div(GetUnitMaxHp(unit) * SKILL_EFF0(SID_Renewal), 100);
#endif

#if defined(SID_Relief) && (COMMON_SKILL_VALID(SID_Relief))
    if (SkillTester(unit, SID_Relief))
    {
        bool ally_in_range = false;
        int i, j;

        for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
        {
            struct Unit * unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            for (j = 0; j < ARRAY_COUNT_RANGE2x2; j++)
            {
                int _x = unit->xPos + gVecs_2x2[j].x;
                int _y = unit->yPos + gVecs_2x2[j].y;

                struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
                if (!UNIT_IS_VALID(unit_ally))
                    continue;

                if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                    continue;

                if (AreUnitsAllied(unit->index, unit_ally->index))
                {
                    ally_in_range = true;
                    break;
                }
            }
        }

        if(!ally_in_range)
            ret += Div(GetUnitMaxHp(unit) * SKILL_EFF0(SID_Relief), 100);
    }
#endif

    return ret;
}

/* LynJump */
void MakeTerrainHealTargetList(int faction)
{
    int i;

    InitTargets(0, 0);

    for (i = faction + 1; i <= (faction + GetFactionUnitAmount(gPlaySt.faction)); i++)
    {
        struct Unit * unit = GetUnit(i);
        int terrainId;
        int amount;

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_DEAD | US_NOT_DEPLOYED | US_RESCUED | US_BIT16))
            continue;

        terrainId = gBmMapTerrain[unit->yPos][unit->xPos];

#if CHAX
        if (GetUnitCurrentHp(unit) != GetUnitMaxHp(unit))
        {
            /* Heal */
            int terrain_heal = Div(GetTerrainHealAmount(terrainId) * GetUnitMaxHp(unit), 100);
            int skill_heal = GetPrePhaseHealAmount(unit);

            amount = terrain_heal + skill_heal;
            if (amount != 0)
                AddTarget(unit->xPos, unit->yPos, unit->index, amount);
        }
#else
        if (GetTerrainHealAmount(terrainId) != 0 && (GetUnitCurrentHp(unit) != GetUnitMaxHp(unit)))
        {
            amount = (GetTerrainHealAmount(terrainId) * GetUnitMaxHp(unit)) / 100;
            AddTarget(unit->xPos, unit->yPos, unit->index, amount);
        }
#endif

        if (GetTerrainHealsStatus(terrainId) == 0)
            continue;

#if CHAX
        if (GetUnitStatusIndex(unit) == UNIT_STATUS_NONE)
            continue;

        if (GetUnitStatusIndex(unit) == UNIT_STATUS_13)
            SetUnitStatus(unit, UNIT_STATUS_PETRIFY);
#else
        if (unit->statusIndex == UNIT_STATUS_NONE)
            continue;

        if (unit->statusIndex == UNIT_STATUS_13)
            unit->statusIndex = UNIT_STATUS_PETRIFY;
#endif

        AddTarget(unit->xPos, unit->yPos, unit->index, -1);
    }
}
