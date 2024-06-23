#include "global.h"
#include "bmunit.h"

#include "common-chax.h"
#include "skill-system.h"

static bool _SkillTester_Generic(struct Unit * unit, const u16 sid)
{
    u8 _sid = SKILL_INDEX_REAL(sid);

    if (UNIT_RAM_SKILLS(unit)[0] == _sid) return true;
    if (UNIT_RAM_SKILLS(unit)[1] == _sid) return true;
    if (UNIT_RAM_SKILLS(unit)[2] == _sid) return true;
    if (UNIT_RAM_SKILLS(unit)[3] == _sid) return true;
    if (UNIT_RAM_SKILLS(unit)[4] == _sid) return true;
    if (UNIT_RAM_SKILLS(unit)[5] == _sid) return true;
    if (UNIT_RAM_SKILLS(unit)[6] == _sid) return true;

    return false;
}

static bool _SkillTester_PInfo(struct Unit * unit, const u16 sid)
{
    int pid = UNIT_CHAR_ID(unit);
    u8 _sid = SKILL_INDEX_REAL(sid);

    if (gpConstSkillTable_Person[pid * 2 + 0] == _sid) return true;
    if (gpConstSkillTable_Person[pid * 2 + 1] == _sid) return true;

    return false;
}

static bool _SkillTester_JInfo(struct Unit * unit, const u16 sid)
{
    int jid = UNIT_CLASS_ID(unit);
    u8 _sid = SKILL_INDEX_REAL(sid);

    if (gpConstSkillTable_Job[jid * 2 + 0] == _sid) return true;
    if (gpConstSkillTable_Job[jid * 2 + 1] == _sid) return true;

    return false;
}

static bool _SkillTester_IInfo(struct Unit * unit, const u16 sid)
{
    int i;
    u8 _sid = SKILL_INDEX_REAL(sid);

    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        int iid = ITEM_INDEX(unit->items[i]);
        if (gpConstSkillTable_Item[iid * 2 + 0] == _sid) return true;
        if (gpConstSkillTable_Item[iid * 2 + 1] == _sid) return true;
    }
    return false;
}

bool _SkillTester(struct Unit * unit, const u16 sid)
{
    if (!UNIT_IS_VALID(unit) || !SKILL_VALID(sid))
        return false;

    switch (SKILL_INDEX_LIST(sid)) {
    case Skill_INFO_ITEM:
        if (_SkillTester_IInfo(unit, sid))
            return true;

        /* fall through */

    case Skill_INFO_PERSON:
    case Skill_INFO_JOB:
        if (_SkillTester_PInfo(unit, sid))
            return true;

        if (_SkillTester_JInfo(unit, sid))
            return true;

        /* fall through */

    case Skill_INFO_GENERIC:
        return _SkillTester_Generic(unit, sid);
    }

    return false;
}
