#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

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
#if defined(SID_SpdBonus) && (SID_SpdBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_SpdBonus))
        status += 2;
#endif

#if defined(SID_DefiantSpd) && (SID_DefiantSpd < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DefiantSpd))
        if ((GetUnitCurrentHp(unit) * 4) < GetUnitMaxHp(unit))
            status += 7;
#endif

#if defined(SID_Fury) && (SID_Fury < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_Fury))
        status += 3;
#endif

#if defined(SID_FuryPlus) && (SID_FuryPlus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FuryPlus))
        status += 4;
#endif

#if defined(SID_RuinedBlade) && (SID_RuinedBlade < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_RuinedBlade))
        status += 5;
#endif

#if defined(SID_RuinedBladePlus) && (SID_RuinedBladePlus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_RuinedBladePlus))
        status += 5;
#endif

#if defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_SPD)
        status += 7;
#endif

#if defined(SID_PushSpeed) && (SID_PushSpeed < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_PushSpeed))
    {
        if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
            status += 5;
    }
#endif

#if defined(SID_PushSpectrum) && (SID_PushSpectrum < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_PushSpectrum))
    {
        if(GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
            status += 5;
    }
#endif

#if defined(SID_LightWeight) && (SID_LightWeight < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LightWeight))
    {
        if (unit->items[3] == 0)
        {
            status += 3;
        }
    }
#endif

    return status;
}
