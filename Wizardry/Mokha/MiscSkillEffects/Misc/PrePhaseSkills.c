#include "global.h"
#include "proc.h"
#include "uiselecttarget.h"
#include "bmunit.h"

#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

STATIC_DECLAR int GetPrePhaseHealAmount(struct Unit * unit)
{
    int ret = 0;

    if (SkillTester(unit, SID_Renewal))
        ret += Div(GetUnitMaxHp(unit) * 3, 10);

    return ret;
}

/* LynJump */
void MakeTerrainHealTargetList(int faction)
{
    int i;

    InitTargets(0, 0);

    for (i = faction + 1; i < faction + 0x40; i++)
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

    return;
}
