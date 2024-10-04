#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitSpeed(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->spd;

    if (unit->state & US_RESCUING)
#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (!SkillTester(unit, SID_PairUp))
        status = unit->spd / 2;
#else 
    status = unit->spd / 2;
#endif

#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
    if (unit == GetUnit(gBattleActor.unit.index) && GetUnit(gBattleTarget.unit.index) && SkillTester(GetUnit(gBattleTarget.unit.index), SID_Unaware))
        return status;
    else if (unit == GetUnit(gBattleTarget.unit.index) && GetUnit(gBattleActor.unit.index) && SkillTester(GetUnit(gBattleActor.unit.index), SID_Unaware))
        return status;
#endif

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
        status += SKILL_EFF0(SID_LifeAndDeath);
#endif

#if defined(SID_SpdBonus) && (COMMON_SKILL_VALID(SID_SpdBonus))
    if (SkillTester(unit, SID_SpdBonus))
        status += SKILL_EFF0(SID_SpdBonus);
#endif

#if defined(SID_DefiantSpd) && (COMMON_SKILL_VALID(SID_DefiantSpd))
    if (SkillTester(unit, SID_DefiantSpd))
        if ((cur_hp * 4) < max_hp)
            status += SKILL_EFF0(SID_DefiantSpd);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
    if (SkillTester(unit, SID_RuinedBlade))
        status += SKILL_EFF1(SID_RuinedBlade);
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
    if (SkillTester(unit, SID_RuinedBladePlus))
        status += SKILL_EFF0(SID_RuinedBladePlus);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_SPD)
        status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_SpeedBoost) && (COMMON_SKILL_VALID(SID_SpeedBoost))
    if (SkillTester(unit, SID_SpeedBoost))
    {
        int __buf = SKILL_EFF0(SID_SpeedBoost);
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
#if defined(SID_PushSpeed) && (COMMON_SKILL_VALID(SID_PushSpeed))
        if (SkillTester(unit, SID_PushSpeed))
            status += SKILL_EFF0(SID_PushSpeed);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += SKILL_EFF0(SID_PushSpectrum);
#endif
    }

#if defined(SID_LightWeight) && (COMMON_SKILL_VALID(SID_LightWeight))
    if (SkillTester(unit, SID_LightWeight))
        if (unit->items[3] == 0)
            status += SKILL_EFF0(SID_LightWeight);
#endif

#if (defined(SID_Resolve) && (COMMON_SKILL_VALID(SID_Resolve))) 
    if (SkillTester(unit, SID_Resolve))
    {
        if ((cur_hp * 2) < max_hp)
            status += status / 2;
    }
#endif

#if (defined(SID_Rampage) && (COMMON_SKILL_VALID(SID_Rampage))) 
    if (SkillTester(unit, SID_Rampage))
            status += status / 2;
#endif

#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (SkillTester(unit, SID_PairUp))
        if (unit->state & US_RESCUING)
            status += Div(_GetUnitSpeed(GetUnit(unit->rescue)) * SKILL_EFF0(SID_PairUp), 100);
#endif

    return status;
}

int SpdPsychUpCheck(int status, struct Unit *unit)
{
    FORCE_DECLARE int stolen_status = 0;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (unit == GetUnit(gBattleActor.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = SpdGetterWeaponBonus(0, GetUnit(gBattleTarget.unit.index)) + SpdGetterSkills(0, GetUnit(gBattleTarget.unit.index));
        return status + stolen_status;
    }
    else if (unit == GetUnit(gBattleTarget.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = SpdGetterWeaponBonus(0, GetUnit(gBattleActor.unit.index)) + SpdGetterSkills(0, GetUnit(gBattleActor.unit.index));
        return status + stolen_status;
    }
#endif

    return status;
}