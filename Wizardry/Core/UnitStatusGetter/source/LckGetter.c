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

#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (SkillTester(unit, SID_PairUp))
        if (unit->state & US_RESCUING)
            status += Div(_GetUnitLuck(GetUnit(unit->rescue)) * SKILL_EFF0(SID_PairUp), 100);
#endif

    return status;
}

int LckPsychUpCheck(int status, struct Unit *unit)
{
    FORCE_DECLARE int stolen_status = 0;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (unit == GetUnit(gBattleActor.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = LckGetterWeaponBonus(0, GetUnit(gBattleTarget.unit.index)) + LckGetterSkills(0, GetUnit(gBattleTarget.unit.index));
        return status + stolen_status;
    }
    else if (unit == GetUnit(gBattleTarget.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = LckGetterWeaponBonus(0, GetUnit(gBattleActor.unit.index)) + LckGetterSkills(0, GetUnit(gBattleActor.unit.index));
        return status + stolen_status;
    }
#endif

    return status;
}