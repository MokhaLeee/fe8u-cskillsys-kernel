#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitDefense(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->def;

    for (it = gpDefGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int DefGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemDefBonus(weapon);
    return status;
}

int DefGetterSkills(int status, struct Unit * unit)
{
#if defined(SID_DefBonus) && (SID_DefBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefBonus))
        status += 2;
#endif

#if defined(SID_DefiantDef) && (SID_DefiantDef < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefiantDef))
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
        status += 5;
#endif

#if defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_DEF)
        status += 7;
#endif

    if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
    {
#if defined(SID_PushDefense) && (SID_PushDefense < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushDefense))
            status += 5;
#endif

#if defined(SID_PushSpectrum) && (SID_PushSpectrum < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_PushSpectrum))
            status += 5;
#endif
    }

    return status;
}
