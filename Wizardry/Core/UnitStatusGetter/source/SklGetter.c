#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitSkill(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->skl;

    if (unit->state & US_RESCUING)
        status = unit->skl / 2;

    for (it = gpSklGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int SklGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemSklBonus(weapon);
    return status;
}

int SklGetterSkills(int status, struct Unit * unit)
{
#if defined(SID_SklBonus) && (SID_SklBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_SklBonus))
        status += 2;
#endif

#if defined(SID_DefiantSkl) && (SID_DefiantSkl < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefiantSkl))
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
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_SKL)
        status += 7;
#endif

    if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
    {
#if defined(SID_PushSkill) && (SID_PushSkill < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushSkill))
            status += 5;
#endif

#if defined(SID_PushSpectrum) && (SID_PushSpectrum < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushSpectrum))
            status += 5;
#endif
    }

#if (defined(SID_Resolve) && (SID_Resolve < MAX_SKILL_NUM)) 
    if (SkillTester(unit, SID_Resolve))
    {
        if ((GetUnitCurrentHp(unit) * 2) < GetUnitMaxHp(unit))
            status += status / 2;
    }
#endif

    return status;
}
