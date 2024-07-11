#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitResistance(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->res;

    for (it = gpResGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int ResGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemResBonus(weapon);
    return status;
}

int ResGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_ResBonus) && (COMMON_SKILL_VALID(SID_ResBonus))
    if (SkillTester(unit, SID_ResBonus))
        status += gpStatusConf_BonusSkills[SKILL_US_RES];
#endif

#if defined(SID_DefiantRes) && (COMMON_SKILL_VALID(SID_DefiantRes))
    if (SkillTester(unit, SID_DefiantRes))
        if ((cur_hp * 4) < max_hp)
            status += gpStatusConf_DefiantSkills[SKILL_US_RES];
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += gpSkillMiscConf->Bonus_Fury;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += gpSkillMiscConf->Bonus_FuryPlus;
#endif

#if defined(SID_FortressRes) && (COMMON_SKILL_VALID(SID_FortressRes))
    if (SkillTester(unit, SID_FortressRes))
        status += 5;
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_RES)
        status += gpStatusConf_luckySevenSkill[SKILL_US_RES];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushResistance) && (COMMON_SKILL_VALID(SID_PushResistance))
        if (SkillTester(unit, SID_PushResistance))
            status += gpStatusConf_PushSkills[SKILL_US_RES];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpStatusConf_PushSkills[SKILL_US_RES];
#endif
    }

    return status;
}
