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

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
    if (SkillTester(unit, SID_LifeAndDeath))
        status += gpSkillExtraInfo[SID_LifeAndDeath].priv[1];
#endif

#if defined(SID_ResBonus) && (COMMON_SKILL_VALID(SID_ResBonus))
    if (SkillTester(unit, SID_ResBonus))
        status += gpSkillExtraInfo[SID_ResBonus].priv[0];
#endif

#if defined(SID_DefiantRes) && (COMMON_SKILL_VALID(SID_DefiantRes))
    if (SkillTester(unit, SID_DefiantRes))
        if ((cur_hp * 4) < max_hp)
            status += gpSkillExtraInfo[SID_DefiantRes].priv[0];
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += gpSkillExtraInfo[SID_Fury].priv[0];
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += gpSkillExtraInfo[SID_FuryPlus].priv[0];
#endif

#if defined(SID_FortressRes) && (COMMON_SKILL_VALID(SID_FortressRes))
    if (SkillTester(unit, SID_FortressRes))
        status += gpSkillExtraInfo[SID_FortressRes].priv[1];
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_RES)
        status += gpSkillExtraInfo[SID_LuckySeven].priv[0];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushResistance) && (COMMON_SKILL_VALID(SID_PushResistance))
        if (SkillTester(unit, SID_PushResistance))
            status += gpSkillExtraInfo[SID_PushResistance].priv[0];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpSkillExtraInfo[SID_PushSpectrum].priv[0];
#endif
    }

    return status;
}
