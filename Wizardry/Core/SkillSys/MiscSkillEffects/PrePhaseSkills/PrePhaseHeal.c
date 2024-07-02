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
        ret += Div(GetUnitMaxHp(unit) * 3, 10);
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
            ret += Div(GetUnitMaxHp(unit) * 2, 10);
    }
#endif

#if defined(SID_RainDish) && (COMMON_SKILL_VALID(SID_RainDish))
    if (SkillTester(unit, SID_RainDish) && gPlaySt.chapterWeatherId == WEATHER_RAIN)
        ret += Div(GetUnitMaxHp(unit) * 3, 20);
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
