#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitPower(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->pow;

#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
    if (unit == GetUnit(gBattleActor.unit.index) && GetUnit(gBattleTarget.unit.index) && SkillTester(GetUnit(gBattleTarget.unit.index), SID_Unaware))
        return status;
    else if (unit == GetUnit(gBattleTarget.unit.index) && GetUnit(gBattleActor.unit.index) && SkillTester(GetUnit(gBattleActor.unit.index), SID_Unaware))
        return status;
#endif

    for (it = gpPowGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int PowGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemPowBonus(weapon);
    return status;
}

int PowGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
    if (SkillTester(unit, SID_LifeAndDeath))
        status += SKILL_EFF0(SID_LifeAndDeath);
#endif

#if defined(SID_StrBonus) && (COMMON_SKILL_VALID(SID_StrBonus))
    if (SkillTester(unit, SID_StrBonus))
        status += SKILL_EFF0(SID_StrBonus);
#endif

#if defined(SID_DefiantStr) && (COMMON_SKILL_VALID(SID_DefiantStr))
    if (SkillTester(unit, SID_DefiantStr))
        if ((cur_hp * 4) < max_hp)
            status += SKILL_EFF0(SID_DefiantStr);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_FortressDef) && (COMMON_SKILL_VALID(SID_FortressDef))
    if (SkillTester(unit, SID_FortressDef))
        status += SKILL_EFF0(SID_FortressDef);
#endif

#if defined(SID_FortressRes) && (COMMON_SKILL_VALID(SID_FortressRes))
    if (SkillTester(unit, SID_FortressRes))
        status += SKILL_EFF0(SID_FortressRes);
#endif

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
    if (SkillTester(unit, SID_RuinedBlade))
        status += SKILL_EFF0(SID_RuinedBlade);
#endif

#if defined(SID_HeavyBladePlus) && (COMMON_SKILL_VALID(SID_HeavyBladePlus))
    if (SkillTester(unit, SID_HeavyBladePlus))
        status += SKILL_EFF0(SID_HeavyBladePlus);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_POW)
        status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_StrengthBoost) && (COMMON_SKILL_VALID(SID_StrengthBoost))
    if (SkillTester(unit, SID_StrengthBoost))
    {
        int __buf = SKILL_EFF0(SID_StrengthBoost);
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
#if defined(SID_PushStrength) && (COMMON_SKILL_VALID(SID_PushStrength))
        if (SkillTester(unit, SID_PushStrength))
            status += SKILL_EFF0(SID_PushStrength);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += SKILL_EFF0(SID_PushSpectrum);
#endif
    }

#if (defined(SID_Resolve) && (COMMON_SKILL_VALID(SID_Resolve)))
    if (SkillTester(unit, SID_Resolve))
    {
        if ((cur_hp * 2) < max_hp)
            status += _GetUnitPower(unit) / 2;
    }
#endif

#if (defined(SID_Rampage) && (COMMON_SKILL_VALID(SID_Rampage)))
    if (SkillTester(unit, SID_Rampage))
        status += _GetUnitPower(unit) / 2;
#endif

#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (SkillTester(unit, SID_PairUp))
        if (unit->state & US_RESCUING)
            status += Div(_GetUnitPower(GetUnit(unit->rescue)) * SKILL_EFF0(SID_PairUp), 100);
#endif

    return status;
}

int PowPsychUpCheck(int status, struct Unit * unit)
{
    FORCE_DECLARE int stolen_status = 0;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (unit == GetUnit(gBattleActor.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = PowGetterWeaponBonus(0, GetUnit(gBattleTarget.unit.index)) +
            PowGetterSkills(0, GetUnit(gBattleTarget.unit.index));
        return status + stolen_status;
    }
    else if (unit == GetUnit(gBattleTarget.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = PowGetterWeaponBonus(0, GetUnit(gBattleActor.unit.index)) +
            PowGetterSkills(0, GetUnit(gBattleActor.unit.index));
        return status + stolen_status;
    }
#endif

    return status;
}