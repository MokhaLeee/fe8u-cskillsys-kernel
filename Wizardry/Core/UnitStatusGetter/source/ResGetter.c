#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitResistance(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->res;

    for (it = gResGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int ResGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemResBonus(weapon);
    return status;
}

int ResGetterSkills(int status, struct Unit * unit)
{
    if (SkillTester(unit, SID_ResBonus))
        status += 2;

    if (SkillTester(unit, SID_DefiantRes))
        if ((GetUnitCurrentHp(unit) * 4) < GetUnitMaxHp(unit))
            status += 7;

    if (SkillTester(unit, SID_Fury))
        status += 3;

    if (SkillTester(unit, SID_FuryPlus))
        status += 4;

    if (SkillTester(unit, SID_FortressRes))
        status += 5;

    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_RES)
        status += 7;

    return status;
}
