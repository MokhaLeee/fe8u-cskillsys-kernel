#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitPower(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->pow;

    for (it = gPowGetters; *it; it++)
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
    if (SkillTester(unit, SID_StrBonus))
        status += 2;

    if (SkillTester(unit, SID_DefiantStr))
        if ((GetUnitCurrentHp(unit) * 4) < GetUnitMaxHp(unit))
            status += 7;

    if (SkillTester(unit, SID_Fury))
        status += 3;

    if (SkillTester(unit, SID_FuryPlus))
        status += 4;

    if (SkillTester(unit, SID_FortressDef))
        status -= 3;

    if (SkillTester(unit, SID_FortressRes))
        status -= 3;

    if (SkillTester(unit, SID_RuinedBlade))
        status -= 5;

    if (SkillTester(unit, SID_HeavyBladePlus))
        status += 5;

    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_POW)
        status += 7;

    return status;
}
