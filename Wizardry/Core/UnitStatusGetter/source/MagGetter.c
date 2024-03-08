#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "strmag.h"
#include "constants/skills.h"

int _GetUnitMagic(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = UNIT_MAG(unit);

    for (it = gMagGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

int MagGetterSkills(int status, struct Unit * unit)
{
    if (SkillTester(unit, SID_MagBonus))
        status += 2;

    if (SkillTester(unit, SID_DefiantMag))
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

    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_MAG)
        status += 7;

    return status;
}
