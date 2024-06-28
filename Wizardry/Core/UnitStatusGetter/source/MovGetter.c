#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "debuff.h"
#include "constants/skills.h"

int _GetUnitMov(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = UNIT_MOV(unit);

    for (it = gpMovGetters; *it; it++)
        status = (*it)(status, unit);

    /* Some special effects */
    if (gpDebuffInfos[GetUnitStatusIndex(unit)].cannot_move)
        status = 0;

    return status;
}

int MovGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_MOV)
        status += 7;
#endif

    if(cur_hp == max_hp)
    {
#if defined(SID_PushMovement) && (COMMON_SKILL_VALID(SID_PushMovement))
        if (SkillTester(unit, SID_PushMovement))
            status += 5;
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += 5;
#endif
    }

#if defined(SID_EvenFooted) && (COMMON_SKILL_VALID(SID_EvenFooted))
    if (SkillTester(unit, SID_EvenFooted) && (gPlaySt.chapterTurnNumber % 2) == 0)
        status += 1;
#endif

#if defined(SID_OddFooted) && (COMMON_SKILL_VALID(SID_OddFooted))
    if (SkillTester(unit, SID_OddFooted) && (gPlaySt.chapterTurnNumber % 2) == 1)
        status += 1;
#endif

#if defined(SID_SpeedBoost) && (COMMON_SKILL_VALID(SID_SpeedBoost))
    if (SkillTester(unit, SID_SpeedBoost))
    {
        if (gPlaySt.chapterTurnNumber >= 6)
            status += 6;
        else
            status += gPlaySt.chapterTurnNumber;
    }
#endif

#if defined(SID_InitNimble) && (COMMON_SKILL_VALID(SID_InitNimble))
        if (SkillTester(unit, SID_InitNimble))
            status += 7;
#endif

#if defined(SID_InitSpectrum) && (COMMON_SKILL_VALID(SID_InitSpectrum))
        if (SkillTester(unit, SID_InitSpectrum))
            status += 7;
#endif

    return status;
}
