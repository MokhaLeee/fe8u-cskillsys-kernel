#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

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
#if defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_MOV)
        status += 7;
#endif

#if defined(SID_EvenFooted) && (SID_EvenFooted < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_EvenFooted) && (gPlaySt.chapterTurnNumber % 2) == 0)
        status += 1;
#endif

    return status;
}
