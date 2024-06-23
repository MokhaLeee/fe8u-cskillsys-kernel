#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "skill-system.h"

extern struct SkillList sSkillList;

STATIC_DECLAR void GenerateSkillListExt(struct Unit * unit, struct SkillList * list)
{
    int i, sid;
    int pid = UNIT_CHAR_ID(unit);
    int jid = UNIT_CLASS_ID(unit);

    memset(list, 0, sizeof(*list));

    /* person */
    sid = gpConstSkillTable_Person[pid * 2];
    if (COMMON_SKILL_VALID(sid))
        list->sid[list->amt++] = sid;

    sid = gpConstSkillTable_Person[pid * 2 + 1];
    if (COMMON_SKILL_VALID(sid))
        list->sid[list->amt++] = sid;

    /* job */
    sid = gpConstSkillTable_Job[jid * 2];
    if (COMMON_SKILL_VALID(sid))
        list->sid[list->amt++] = sid;

    sid = gpConstSkillTable_Job[jid * 2 + 1];
    if (COMMON_SKILL_VALID(sid))
        list->sid[list->amt++] = sid;

    /* generic */
    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
    {
        sid = UNIT_RAM_SKILLS(unit)[i];
        if (COMMON_SKILL_VALID(sid))
            list->sid[list->amt++] = sid;
    }

    WriteUnitListHeader(unit, &list->header);
}

struct SkillList * GetUnitSkillList(struct Unit * unit)
{
    struct SkillList * list = &sSkillList;
    if (!JudgeUnitListHeader(unit, &list->header))
        GenerateSkillListExt(unit, list);

    return list;
}

void ResetSkillLists(void)
{
    memset(&sSkillList, 0, sizeof(sSkillList));
}
