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
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_MagBonus) && (COMMON_SKILL_VALID(SID_MagBonus))
    if (SkillTester(unit, SID_MagBonus))
        status += gpStatusConf_BonusSkills[SKILL_US_MAG];
#endif

#if defined(SID_DefiantMag) && (COMMON_SKILL_VALID(SID_DefiantMag))
    if (SkillTester(unit, SID_DefiantMag))
        if ((cur_hp * 4) < max_hp)
            status += gpStatusConf_DefiantSkills[SKILL_US_MAG];
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += gpSkillMiscConf->Bonus_Fury;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += gpSkillMiscConf->Bonus_FuryPlus;
#endif

#if defined(SID_FortressDef) && (COMMON_SKILL_VALID(SID_FortressDef))
    if (SkillTester(unit, SID_FortressDef))
        status -= 3;
#endif

#if defined(SID_FortressRes) && (COMMON_SKILL_VALID(SID_FortressRes))
    if (SkillTester(unit, SID_FortressRes))
        status -= 3;
#endif

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
    if (SkillTester(unit, SID_RuinedBlade))
        status -= 5;
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_MAG)
        status += gpStatusConf_luckySevenSkill[SKILL_US_MAG];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushMagic) && (COMMON_SKILL_VALID(SID_PushMagic))
        if (SkillTester(unit, SID_PushMagic))
            status += gpStatusConf_PushSkills[SKILL_US_MAG];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpStatusConf_PushSkills[SKILL_US_MAG];
#endif
    }

    return status;
}
