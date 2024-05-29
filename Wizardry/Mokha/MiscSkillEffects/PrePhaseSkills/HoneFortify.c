#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

bool PrePhaseFunc_HoneFortify(ProcPtr proc)
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
        bool SID_PowHone_eff = false;
        bool SID_MagHone_eff = false;
        bool SID_SklHone_eff = false;
        bool SID_SpdHone_eff = false;
        bool SID_LckHone_eff = false;
        bool SID_DefHone_eff = false;
        bool SID_ResHone_eff = false;
        bool Hone_eff = false;

        struct Unit * unit = GetUnit(uid);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if defined(SID_PowHone) && (SID_PowHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PowHone))
        {
            SID_PowHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_MagHone) && (SID_MagHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_MagHone))
        {
            SID_MagHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_SklHone) && (SID_SklHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_SklHone))
        {
            SID_SklHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_SpdHone) && (SID_SpdHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_SpdHone))
        {
            SID_SpdHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_LckHone) && (SID_LckHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_LckHone))
        {
            SID_LckHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_DefHone) && (SID_DefHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_DefHone))
        {
            SID_DefHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_ResHone) && (SID_ResHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_ResHone))
        {
            SID_ResHone_eff = true;
            Hone_eff = true;
        }
#endif

        if (Hone_eff)
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
                    if (SID_PowHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_POW);

                    if (SID_MagHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_MAG);

                    if (SID_SklHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SKL);

                    if (SID_SpdHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SPD);

                    if (SID_LckHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_LCK);

                    if (SID_DefHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_DEF);

                    if (SID_ResHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_RES);

                    break;
                }
            }
        }
    }
    return false;
}
