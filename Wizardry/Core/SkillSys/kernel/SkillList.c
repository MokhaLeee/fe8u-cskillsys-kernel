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

    u8 * tmp_list = gGenericBuffer;

    memset(list, 0, sizeof(*list));
    memset(tmp_list, 0, MAX_SKILL_NUM + 1);

    /* person */
    sid = gpConstSkillTable_Person[pid * 2];
    if (COMMON_SKILL_VALID(sid))
        tmp_list[sid] = true;

    sid = gpConstSkillTable_Person[pid * 2 + 1];
    if (COMMON_SKILL_VALID(sid))
        tmp_list[sid] = true;

    /* job */
    sid = gpConstSkillTable_Job[jid * 2];
    if (COMMON_SKILL_VALID(sid))
        tmp_list[sid] = true;

    sid = gpConstSkillTable_Job[jid * 2 + 1];
    if (COMMON_SKILL_VALID(sid))
        tmp_list[sid] = true;

    /* item */
    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        u8 iid = ITEM_INDEX(unit->items[i]);

        sid = gpConstSkillTable_Item[iid * 2];
        if (COMMON_SKILL_VALID(sid))
            tmp_list[sid] = true;

        sid = gpConstSkillTable_Item[iid * 2 + 1];
        if (COMMON_SKILL_VALID(sid))
            tmp_list[sid] = true;
    }

    /* generic */
    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
    {
        sid = UNIT_RAM_SKILLS(unit)[i];
        if (COMMON_SKILL_VALID(sid))
            tmp_list[sid] = true;
    }

    for (i = 1; i < MAX_SKILL_NUM; i++)
        if (tmp_list[i])
            list->sid[list->amt++] = i;

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
