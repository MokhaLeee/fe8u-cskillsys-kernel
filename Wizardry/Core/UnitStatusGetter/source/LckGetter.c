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
    struct Unit * target = GetUnit(gBattleTarget.unit.index);
    struct Unit * actor = GetUnit(gBattleActor.unit.index);
/*
** This needs to be first, as it returns the status value before changes are applied
*/
#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
        if (unit == target)
        {
            if (SkillTester(actor, SID_Unaware))
                return status;
        }
        else if (unit == actor)
        {
            if (SkillTester(target, SID_Unaware))
                return status;
        }
#endif

    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemLckBonus(weapon);
    return status;
}

int LckGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

    struct Unit * target = GetUnit(gBattleTarget.unit.index);
    struct Unit * actor = GetUnit(gBattleActor.unit.index);
/*
** This needs to be first, as it returns the status value before changes are applied
*/
#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
        if (unit == target)
        {
            if (SkillTester(actor, SID_Unaware))
                return status;
        }
        else if (unit == actor)
        {
            if (SkillTester(target, SID_Unaware))
                return status;
        }
#endif

#if defined(SID_LckBonus) && (COMMON_SKILL_VALID(SID_LckBonus))
    if (SkillTester(unit, SID_LckBonus))
        status += SKILL_EFF0(SID_LckBonus);
#endif

#if defined(SID_DefiantLck) && (COMMON_SKILL_VALID(SID_DefiantLck))
    if (SkillTester(unit, SID_DefiantLck))
        if ((cur_hp * 4) < max_hp)
            status += SKILL_EFF0(SID_DefiantLck);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_LCK)
        status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_LuckBoost) && (COMMON_SKILL_VALID(SID_LuckBoost))
    if (SkillTester(unit, SID_LuckBoost))
    {
        int __buf = SKILL_EFF0(SID_LuckBoost);
        if (gPlaySt.chapterTurnNumber >= __buf)
            status += __buf;
        else
            status += gPlaySt.chapterTurnNumber;
    }
#endif

#if defined(SID_SpectrumBoost) && (COMMON_SKILL_VALID(SID_SpectrumBoost))
    if (SkillTester(unit, SID_SpectrumBoost))
    {
        int __buf = SKILL_EFF0(SID_SpectrumBoost);
        if (gPlaySt.chapterTurnNumber >= __buf)
            status += __buf;
        else
            status += gPlaySt.chapterTurnNumber;
    }
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushLuck) && (COMMON_SKILL_VALID(SID_PushLuck))
        if (SkillTester(unit, SID_PushLuck))
            status += SKILL_EFF0(SID_PushLuck);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += SKILL_EFF0(SID_PushSpectrum);
#endif
    }

    return status;
}
