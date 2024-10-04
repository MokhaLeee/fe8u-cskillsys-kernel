#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "strmag.h"
#include "constants/skills.h"

int _GetUnitMagic(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = UNIT_MAG(unit);

#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
    if (unit == GetUnit(gBattleActor.unit.index) && GetUnit(gBattleTarget.unit.index) && SkillTester(GetUnit(gBattleTarget.unit.index), SID_Unaware))
        return status;
    else if (unit == GetUnit(gBattleTarget.unit.index) && GetUnit(gBattleActor.unit.index) && SkillTester(GetUnit(gBattleActor.unit.index), SID_Unaware))
        return status;
#endif

    for (it = gpMagGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

int MagGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
    if (SkillTester(unit, SID_LifeAndDeath))
        status += SKILL_EFF0(SID_LifeAndDeath);
#endif

#if defined(SID_MagBonus) && (COMMON_SKILL_VALID(SID_MagBonus))
    if (SkillTester(unit, SID_MagBonus))
        status += SKILL_EFF0(SID_MagBonus);
#endif

#if defined(SID_DefiantMag) && (COMMON_SKILL_VALID(SID_DefiantMag))
    if (SkillTester(unit, SID_DefiantMag))
        if ((cur_hp * 4) < max_hp)
            status += SKILL_EFF0(SID_DefiantMag);
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

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_MAG)
        status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_MagicBoost) && (COMMON_SKILL_VALID(SID_MagicBoost))
    if (SkillTester(unit, SID_MagicBoost))
    {
        int __buf = SKILL_EFF0(SID_MagicBoost);
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
#if defined(SID_PushMagic) && (COMMON_SKILL_VALID(SID_PushMagic))
        if (SkillTester(unit, SID_PushMagic))
            status += SKILL_EFF0(SID_PushMagic);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += SKILL_EFF0(SID_PushSpectrum);
#endif
    }

#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (SkillTester(unit, SID_PairUp))
        if (unit->state & US_RESCUING)
            status += Div(_GetUnitMagic(GetUnit(unit->rescue)) * SKILL_EFF0(SID_PairUp), 100);
#endif

    return status;
}

int MagPsychUpCheck(int status, struct Unit *unit)
{
   FORCE_DECLARE int stolen_status = 0;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (unit == GetUnit(gBattleActor.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = MagGetterSkills(0, GetUnit(gBattleTarget.unit.index));
        return status + stolen_status;
    }
    else if (unit == GetUnit(gBattleTarget.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = MagGetterSkills(0, GetUnit(gBattleActor.unit.index));
        return status + stolen_status;
    }
#endif

    return status;
}