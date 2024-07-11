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
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_SklBonus) && (COMMON_SKILL_VALID(SID_SklBonus))
    if (SkillTester(unit, SID_SklBonus))
        status += gpStatusConf_BonusSkills[SKILL_US_SKL];
#endif

#if defined(SID_DefiantSkl) && (COMMON_SKILL_VALID(SID_DefiantSkl))
    if (SkillTester(unit, SID_DefiantSkl))
        if ((cur_hp * 4) < max_hp)
            status += gpStatusConf_BonusSkills[SKILL_US_SKL];
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += gpSkillMiscConf->Bonus_Fury;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += gpSkillMiscConf->Bonus_FuryPlus;
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_SKL)
        status += gpStatusConf_BonusSkills[SKILL_US_SKL];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushSkill) && (COMMON_SKILL_VALID(SID_PushSkill))
        if (SkillTester(unit, SID_PushSkill))
            status += gpStatusConf_PushSkills[SKILL_US_SKL];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpStatusConf_PushSkills[SKILL_US_SKL];
#endif
    }

#if (defined(SID_Resolve) && (COMMON_SKILL_VALID(SID_Resolve))) 
    if (SkillTester(unit, SID_Resolve))
    {
        if ((cur_hp * 2) < max_hp)
            status += status / 2;
    }
#endif

    return status;
}
