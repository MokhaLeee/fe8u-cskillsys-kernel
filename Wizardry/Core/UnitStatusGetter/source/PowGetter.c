#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitPower(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->pow;

    for (it = gpPowGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int PowGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemPowBonus(weapon);
    return status;
}

int PowGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_StrBonus) && (COMMON_SKILL_VALID(SID_StrBonus))
    if (SkillTester(unit, SID_StrBonus))
        status += 2;
#endif

#if defined(SID_DefiantStr) && (COMMON_SKILL_VALID(SID_DefiantStr))
    if (SkillTester(unit, SID_DefiantStr))
        if ((cur_hp * 4) < max_hp)
            status += 7;
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += 3;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += 4;
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

#if defined(SID_HeavyBladePlus) && (COMMON_SKILL_VALID(SID_HeavyBladePlus))
    if (SkillTester(unit, SID_HeavyBladePlus))
        status += 5;
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_POW)
        status += 7;
#endif

    if(cur_hp == max_hp)
    {
#if defined(SID_PushStrength) && (COMMON_SKILL_VALID(SID_PushStrength))
        if (SkillTester(unit, SID_PushStrength))
            status += 5;
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += 5;
#endif
    }
    
#if (defined(SID_Resolve) && (COMMON_SKILL_VALID(SID_Resolve))) 
    if (SkillTester(unit, SID_Resolve))
    {
        if ((cur_hp * 2) < max_hp)
            status += status / 2;
    }
#endif

#if defined(SID_InitStrong) && (COMMON_SKILL_VALID(SID_InitStrong))
        if (SkillTester(unit, SID_InitStrong))
            status += 7;
#endif

#if defined(SID_InitSpectrum) && (COMMON_SKILL_VALID(SID_InitSpectrum))
        if (SkillTester(unit, SID_InitSpectrum))
            status += 7;
#endif

    return status;
}
