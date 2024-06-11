#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

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
#if defined(SID_ResBonus) && (SID_ResBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_ResBonus))
        status += 2;
#endif

#if defined(SID_DefiantRes) && (SID_DefiantRes < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefiantRes))
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

#if defined(SID_FortressRes) && (SID_FortressRes < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FortressRes))
        status += 5;
#endif

#if defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_RES)
        status += 7;
#endif

    if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
    {
#if defined(SID_PushResistance) && (SID_PushResistance < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushResistance))
            status += 5;
#endif

#if defined(SID_PushSpectrum) && (SID_PushSpectrum < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushSpectrum))
            status += 5;
#endif
    }

    return status;
}
