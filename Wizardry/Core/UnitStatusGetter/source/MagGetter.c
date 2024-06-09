#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "strmag.h"
#include "constants/skills.h"

int _GetUnitMagic(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = UNIT_MAG(unit);

    for (it = gpMagGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

int MagGetterSkills(int status, struct Unit * unit)
{
#if defined(SID_MagBonus) && (SID_MagBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_MagBonus))
        status += 2;
#endif

#if defined(SID_DefiantMag) && (SID_DefiantMag < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefiantMag))
        if ((GetUnitCurrentHp(unit) * 4) < GetUnitMaxHp(unit))
            status += 7;
#endif

#if defined(SID_Fury) && (SID_Fury < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_Fury))
        status += 3;
#endif

#if defined(SID_FuryPlus) && (SID_FuryPlus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FuryPlus))
        status += 4;
#endif

#if defined(SID_FortressDef) && (SID_FortressDef < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FortressDef))
        status -= 3;
#endif

#if defined(SID_FortressRes) && (SID_FortressRes < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FortressRes))
        status -= 3;
#endif

#if defined(SID_RuinedBlade) && (SID_RuinedBlade < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_RuinedBlade))
        status -= 5;
#endif

#if defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_MAG)
        status += 7;
#endif

#if defined(SID_PushMagic) && (SID_PushMagic < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_PushMagic))
    {
        if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
            status += 5;
    }
#endif

#if defined(SID_PushSpectrum) && (SID_PushSpectrum < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_PushSpectrum))
    {
        if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
            status += 5;
    }
#endif

    return status;
}
