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

#if defined(SID_DefBonus) && (COMMON_SKILL_VALID(SID_DefBonus))
    if (SkillTester(unit, SID_DefBonus))
        status += gpStatusConf_BonusSkills[SKILL_US_DEF];
#endif

#if defined(SID_DefiantDef) && (COMMON_SKILL_VALID(SID_DefiantDef))
    if (SkillTester(unit, SID_DefiantDef))
        if ((cur_hp * 4) < max_hp)
            status += gpStatusConf_DefiantSkills[SKILL_US_DEF];
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
        status += 5;
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_DEF)
        status += gpStatusConf_luckySevenSkill[SKILL_US_DEF];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushDefense) && (COMMON_SKILL_VALID(SID_PushDefense))
        if (SkillTester(unit, SID_PushDefense))
            status += gpStatusConf_PushSkills[SKILL_US_DEF];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpStatusConf_PushSkills[SKILL_US_DEF];
#endif
    }

    return status;
}
