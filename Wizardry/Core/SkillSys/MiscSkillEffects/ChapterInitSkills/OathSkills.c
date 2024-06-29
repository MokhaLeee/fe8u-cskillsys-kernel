#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "kernel-lib.h"

static void _SetOathStatDebuf(struct Unit * unit)
{
#if (defined(SID_OathStrength) && COMMON_SKILL_VALID(SID_OathStrength))
    if (SkillTester(unit, SID_OathStrength))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_POW);
#endif

#if (defined(SID_OathMagic) && COMMON_SKILL_VALID(SID_OathMagic))
    if (SkillTester(unit, SID_OathMagic))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_MAG);
#endif

#if (defined(SID_OathSkill) && COMMON_SKILL_VALID(SID_OathSkill))
    if (SkillTester(unit, SID_OathSkill))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SKL);
#endif

#if (defined(SID_OathSpeed) && COMMON_SKILL_VALID(SID_OathSpeed))
    if (SkillTester(unit, SID_OathSpeed))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SPD);
#endif

#if (defined(SID_OathLuck) && COMMON_SKILL_VALID(SID_OathLuck))
    if (SkillTester(unit, SID_OathLuck))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_LCK);
#endif

#if (defined(SID_OathDefense) && COMMON_SKILL_VALID(SID_OathDefense))
    if (SkillTester(unit, SID_OathDefense))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_DEF);
#endif

#if (defined(SID_OathResistance) && COMMON_SKILL_VALID(SID_OathResistance))
    if (SkillTester(unit, SID_OathResistance))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_RES);
#endif

#if (defined(SID_OathMovement) && COMMON_SKILL_VALID(SID_OathMovement))
    if (SkillTester(unit, SID_OathMovement))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_MOV);
#endif
}

static void _ClearOathStatDebuf(struct Unit * unit)
{
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_POW);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_MAG);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SKL);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SPD);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_LCK);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_DEF);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_RES);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_MOV);
}

bool ChapterInit_SetOathStatus(ProcPtr proc)
{
    int i, j;
    struct Unit * unit;

    for (i = FACTION_BLUE + 1; i < FACTION_BLUE + 1 + CONFIG_UNIT_AMT_ALLY; i++)
    {
        unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
            continue;
        
        for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
        {
            int _x = unit->xPos + gVecs_1x1[j].x;
            int _y = unit->yPos + gVecs_1x1[j].y;

            struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (AreUnitsAllied(unit->index, unit_ally->index))
            {
                _SetOathStatDebuf(unit);
                break;
            }
        }
    }

    for (i = FACTION_GREEN + 1; i < FACTION_GREEN + 1 + CONFIG_UNIT_AMT_NPC; i++)
    {
        unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
            continue;

        for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
        {
            int _x = unit->xPos + gVecs_1x1[j].x;
            int _y = unit->yPos + gVecs_1x1[j].y;

            struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (AreUnitsAllied(unit->index, unit_ally->index))
            {
                _SetOathStatDebuf(unit);
                break;
            }
        }
    }

    for (i = FACTION_RED + 1; i < FACTION_RED + 1 + CONFIG_UNIT_AMT_ENEMY; i++)
    {
        unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
            continue;

        for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
        {
            int _x = unit->xPos + gVecs_1x1[j].x;
            int _y = unit->yPos + gVecs_1x1[j].y;

            struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (AreUnitsAllied(unit->index, unit_ally->index))
            {
                _SetOathStatDebuf(unit);
                break;
            }
        }
    }

    return false;
}

bool PrePhsae_TickOathSkillStatus(ProcPtr proc)
{
    int i, j;
    struct Unit * unit;
    int allies_gRange1_In1x1 = 0;

    if (gPlaySt.faction == FACTION_BLUE)
    {
        for (i = FACTION_BLUE + 1; i < FACTION_BLUE + 1 + CONFIG_UNIT_AMT_ALLY; i++)
        {
            allies_gRange1_In1x1 = 0;
            unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
            {
                int _x = unit->xPos + gVecs_1x1[j].x;
                int _y = unit->yPos + gVecs_1x1[j].y;

                struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
                if (!UNIT_IS_VALID(unit_ally))
                    continue;

                if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                    continue;

                if (AreUnitsAllied(unit->index, unit_ally->index))
                {
                    allies_gRange1_In1x1 += 1;
                    break;
                }
            }

            NoCashGBAPrintf("The number of allied units are %d", allies_gRange1_In1x1);

            if (allies_gRange1_In1x1 == 0)
                _ClearOathStatDebuf(unit);
            else
                _SetOathStatDebuf(unit);
        }

        for (i = FACTION_GREEN + 1; i < FACTION_GREEN + 1 + CONFIG_UNIT_AMT_NPC; i++)
        {
            allies_gRange1_In1x1 = 0;
            unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

           for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
            {
                int _x = unit->xPos + gVecs_1x1[j].x;
                int _y = unit->yPos + gVecs_1x1[j].y;

                struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
                if (!UNIT_IS_VALID(unit_ally))
                    continue;

                if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                    continue;

                if (AreUnitsAllied(unit->index, unit_ally->index))
                {
                    allies_gRange1_In1x1 += 1;
                    break;
                }
            }

            if (allies_gRange1_In1x1 == 0)
                _ClearOathStatDebuf(unit);
            else
                _SetOathStatDebuf(unit);
        }

        for (i = FACTION_RED + 1; i < FACTION_RED + 1 + CONFIG_UNIT_AMT_ENEMY; i++)
        {
            allies_gRange1_In1x1 = 0;
            unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
            {
                int _x = unit->xPos + gVecs_1x1[j].x;
                int _y = unit->yPos + gVecs_1x1[j].y;

                struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
                if (!UNIT_IS_VALID(unit_ally))
                    continue;

                if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                    continue;

                if (AreUnitsAllied(unit->index, unit_ally->index))
                {
                    allies_gRange1_In1x1 += 1;
                    break;
                }
            }

            if (allies_gRange1_In1x1 == 0)
                _ClearOathStatDebuf(unit);
            else
                _SetOathStatDebuf(unit);
        }
    }
    return false;
}
