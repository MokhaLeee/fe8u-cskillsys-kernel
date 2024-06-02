#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "class-types.h"
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
        bool PowHone_eff = false;
        bool MagHone_eff = false;
        bool SklHone_eff = false;
        bool SpdHone_eff = false;
        bool LckHone_eff = false;
        bool DefHone_eff = false;
        bool ResHone_eff = false;
        bool JobHone_eff = false;
        bool JobFortify_eff = false;

        bool Hone_eff = false;

        struct Unit * unit = GetUnit(uid);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if defined(SID_PowHone) && (SID_PowHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PowHone))
        {
            PowHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_MagHone) && (SID_MagHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_MagHone))
        {
            MagHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_SklHone) && (SID_SklHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_SklHone))
        {
            SklHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_SpdHone) && (SID_SpdHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_SpdHone))
        {
            SpdHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_LckHone) && (SID_LckHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_LckHone))
        {
            LckHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_DefHone) && (SID_DefHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_DefHone))
        {
            DefHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_ResHone) && (SID_ResHone < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_ResHone))
        {
            ResHone_eff = true;
            Hone_eff = true;
        }
#endif

#if defined(SID_HoneCavalry) && (SID_HoneCavalry < MAX_SKILL_NUM)
        if (CheckClassCavalry(UNIT_CLASS_ID(unit)))
        {
            if (SkillTester(unit, SID_HoneCavalry))
            {
                JobHone_eff = true;
                Hone_eff = true;
            }
        }
#endif

#if defined(SID_HoneFlier) && (SID_HoneFlier < MAX_SKILL_NUM)
        if (CheckClassFlier(UNIT_CLASS_ID(unit)))
        {
            if (SkillTester(unit, SID_HoneFlier))
            {
                JobHone_eff = true;
                Hone_eff = true;
            }
        }
#endif

#if defined(SID_HoneArmor) && (SID_HoneArmor < MAX_SKILL_NUM)
        if (CheckClassArmor(UNIT_CLASS_ID(unit)))
        {
            if (SkillTester(unit, SID_HoneArmor))
            {
                JobHone_eff = true;
                Hone_eff = true;
            }
        }
#endif

#if defined(SID_FortifyArmor) && (SID_FortifyArmor < MAX_SKILL_NUM)
        if (CheckClassArmor(UNIT_CLASS_ID(unit)))
        {
            if (SkillTester(unit, SID_FortifyArmor))
            {
                JobFortify_eff = true;
                Hone_eff = true;
            }
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
                    if (PowHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_POW);

                    if (MagHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_MAG);

                    if (SklHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SKL);

                    if (SpdHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SPD);

                    if (LckHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_LCK);

                    if (DefHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_DEF);

                    if (ResHone_eff)
                        SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_RES);

                    if (JobHone_eff)
                    {
                        if (CheckSameClassType(UNIT_CLASS_ID(unit), UNIT_CLASS_ID(tunit)))
                            SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_JOB_HONE);
                    }

                    if (JobFortify_eff)
                    {
                        if (CheckSameClassType(UNIT_CLASS_ID(unit), UNIT_CLASS_ID(tunit)))
                            SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_JOB_FORTIFY);
                    }

                    break;
                }
            }
        }
    }
    return false;
}
