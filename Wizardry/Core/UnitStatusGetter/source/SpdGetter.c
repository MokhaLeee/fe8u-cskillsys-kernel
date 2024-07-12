#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitSpeed(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->spd;

    if (unit->state & US_RESCUING)
        status = unit->spd / 2;

    for (it = gpSpdGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int SpdGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemSpdBonus(weapon);
    return status;
}

int SpdGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
    if (SkillTester(unit, SID_LifeAndDeath))
        status += gpSkillExtraInfo[SID_LifeAndDeath].priv[0];
#endif

#if defined(SID_SpdBonus) && (COMMON_SKILL_VALID(SID_SpdBonus))
    if (SkillTester(unit, SID_SpdBonus))
        status += gpSkillExtraInfo[SID_SpdBonus].priv[0];
#endif

#if defined(SID_DefiantSpd) && (COMMON_SKILL_VALID(SID_DefiantSpd))
    if (SkillTester(unit, SID_DefiantSpd))
        if ((cur_hp * 4) < max_hp)
            status += gpSkillExtraInfo[SID_DefiantSpd].priv[0];
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += gpSkillExtraInfo[SID_Fury].priv[0];
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += gpSkillExtraInfo[SID_FuryPlus].priv[0];
#endif

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
    if (SkillTester(unit, SID_RuinedBlade))
        status += gpSkillExtraInfo[SID_RuinedBlade].priv[1];
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
    if (SkillTester(unit, SID_RuinedBladePlus))
        status += gpSkillExtraInfo[SID_RuinedBladePlus].priv[0];
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_SPD)
        status += gpSkillExtraInfo[SID_LuckySeven].priv[0];
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushSpeed) && (COMMON_SKILL_VALID(SID_PushSpeed))
        if (SkillTester(unit, SID_PushSpeed))
            status += gpSkillExtraInfo[SID_PushSpeed].priv[0];
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += gpSkillExtraInfo[SID_PushSpectrum].priv[0];
#endif
    }

#if defined(SID_LightWeight) && (COMMON_SKILL_VALID(SID_LightWeight))
    if (SkillTester(unit, SID_LightWeight))
        if (unit->items[3] == 0)
            status += 3;
#endif

#if (defined(SID_Resolve) && (COMMON_SKILL_VALID(SID_Resolve))) 
    if (SkillTester(unit, SID_Resolve))
    {
        if ((cur_hp * 2) < max_hp)
            status += status / 2;
    }
#endif

    return status;
}
