#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitLuck(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->lck;

    for (it = gpLckGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int LckGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemLckBonus(weapon);
    return status;
}

int LckGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LckBonus) && (COMMON_SKILL_VALID(SID_LckBonus))
    if (SkillTester(unit, SID_LckBonus))
        status += gpSkillExtraInfo[SID_LckBonus].priv[0];
#endif

#if defined(SID_DefiantLck) && (COMMON_SKILL_VALID(SID_DefiantLck))
    if (SkillTester(unit, SID_DefiantLck))
        if ((cur_hp * 4) < max_hp)
            status += gpSkillExtraInfo[SID_DefiantLck].priv[0];
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += gpSkillExtraInfo[SID_Fury].priv[0];
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += gpSkillExtraInfo[SID_FuryPlus].priv[0];
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_LCK)
        status += gpSkillExtraInfo[SID_LuckySeven].priv[0];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushLuck) && (COMMON_SKILL_VALID(SID_PushLuck))
        if (SkillTester(unit, SID_PushLuck))
            status += gpSkillExtraInfo[SID_PushLuck].priv[0];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpSkillExtraInfo[SID_PushSpectrum].priv[0];
#endif
    }

    return status;
}
