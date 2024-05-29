#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

bool PrePhaseFunc_StatDebuffOpenning(ProcPtr proc)
{
    u32 i;
    int uid;

    static const struct Vec2 vec_range2[] = {
                            { 0, -2},
                  {-1, -1}, { 0, -1}, { 1, -1},
        {-2,  0}, {-1,  0},           { 1,  0}, { 2,  0},
                  {-1,  1}, { 0,  1}, { 1,  1},
                            { 0,  2},
    };

    for (uid = gPlaySt.faction + 1; uid < gPlaySt.faction + 0x40; uid++)
    {
        bool SID_PowOpenning_eff = false;
        bool SID_MagOpenning_eff = false;
        bool SID_SklOpenning_eff = false;
        bool SID_SpdOpenning_eff = false;
        bool SID_LckOpenning_eff = false;
        bool SID_DefOpenning_eff = false;
        bool SID_ResOpenning_eff = false;
        bool Openning_eff = false;

        struct Unit * unit = GetUnit(uid);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if defined(SID_PowOpenning) && (SID_PowOpenning < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PowOpenning))
        {
            SID_PowOpenning_eff = true;
            Openning_eff = true;
        }
#endif

#if defined(SID_MagOpenning) && (SID_MagOpenning < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_MagOpenning))
        {
            SID_MagOpenning_eff = true;
            Openning_eff = true;
        }
#endif

#if defined(SID_SklOpenning) && (SID_SklOpenning < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_SklOpenning))
        {
            SID_SklOpenning_eff = true;
            Openning_eff = true;
        }
#endif

#if defined(SID_SpdOpenning) && (SID_SpdOpenning < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_SpdOpenning))
        {
            SID_SpdOpenning_eff = true;
            Openning_eff = true;
        }
#endif

#if defined(SID_LckOpenning) && (SID_LckOpenning < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_LckOpenning))
        {
            SID_LckOpenning_eff = true;
            Openning_eff = true;
        }
#endif

#if defined(SID_DefOpenning) && (SID_DefOpenning < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_DefOpenning))
        {
            SID_DefOpenning_eff = true;
            Openning_eff = true;
        }
#endif

#if defined(SID_ResOpenning) && (SID_ResOpenning < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_ResOpenning))
        {
            SID_ResOpenning_eff = true;
            Openning_eff = true;
        }
#endif

        if (Openning_eff)
        {
            for (i = 0; i < ARRAY_COUNT(vec_range2); i++)
            {
                int x = unit->xPos + vec_range2[i].x;
                int y = unit->yPos + vec_range2[i].y;

                struct Unit * tunit = GetUnitAtPosition(x, y);
                if (!tunit)
                    continue;

                if (tunit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                    continue;

                if (AreUnitsAllied(unit->index, tunit->index))
                {
                    if (SID_PowOpenning_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_POW);

                    if (SID_MagOpenning_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_MAG);

                    if (SID_SklOpenning_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SKL);

                    if (SID_SpdOpenning_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SPD);

                    if (SID_LckOpenning_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_LCK);

                    if (SID_DefOpenning_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_DEF);

                    if (SID_ResOpenning_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_RES);

                    break;
                }
            }
        }
    }
    return false;
}
