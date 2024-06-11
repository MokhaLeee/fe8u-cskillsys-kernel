#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

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
#if defined(SID_LckBonus) && (SID_LckBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LckBonus))
        status += 2;
#endif

#if defined(SID_DefiantLck) && (SID_DefiantLck < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefiantLck))
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

#if defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_LCK)
        status += 7;
#endif

    if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
    {
#if defined(SID_PushLuck) && (SID_PushLuck < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushLuck))
            status += 5;
#endif

#if defined(SID_PushSpectrum) && (SID_PushSpectrum < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushSpectrum))
            status += 5;
#endif
    }

    return status;
}
