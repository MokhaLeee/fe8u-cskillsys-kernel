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
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
    if (SkillTester(unit, SID_LifeAndDeath))
        status += gpSkillExtraInfo[SID_LifeAndDeath].priv[1];
#endif

#if defined(SID_DefBonus) && (COMMON_SKILL_VALID(SID_DefBonus))
    if (SkillTester(unit, SID_DefBonus))
        status += gpSkillExtraInfo[SID_DefBonus].priv[0];
#endif

#if defined(SID_DefiantDef) && (COMMON_SKILL_VALID(SID_DefiantDef))
    if (SkillTester(unit, SID_DefiantDef))
        if ((cur_hp * 4) < max_hp)
            status += gpSkillExtraInfo[SID_DefiantDef].priv[0];
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += gpSkillExtraInfo[SID_Fury].priv[0];
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += gpSkillExtraInfo[SID_FuryPlus].priv[0];
#endif

#if defined(SID_FortressDef) && (COMMON_SKILL_VALID(SID_FortressDef))
    if (SkillTester(unit, SID_FortressDef))
        status += gpSkillExtraInfo[SID_FortressDef].priv[1];
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_DEF)
        status += gpSkillExtraInfo[SID_LuckySeven].priv[0];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushDefense) && (COMMON_SKILL_VALID(SID_PushDefense))
        if (SkillTester(unit, SID_PushDefense))
            status += gpSkillExtraInfo[SID_PushDefense].priv[0];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpSkillExtraInfo[SID_PushSpectrum].priv[0];
#endif
    }

    return status;
}
