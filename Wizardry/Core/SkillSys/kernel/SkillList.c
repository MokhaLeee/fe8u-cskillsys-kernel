#include "common-chax.h"
#include "skill-system.h"

/**
 * 0: generic use
 * 1: battle actor
 * 2: battle target
 */
extern struct SkillList sSkillList[3];

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
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    sid = gpConstSkillTable_Person[pid * 2 + 1];
    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    /* job */
    sid = gpConstSkillTable_Job[jid * 2];
    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    sid = gpConstSkillTable_Job[jid * 2 + 1];
    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    /* item */
    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        u8 iid = ITEM_INDEX(unit->items[i]);

        sid = gpConstSkillTable_Item[iid * 2];
        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }

        sid = gpConstSkillTable_Item[iid * 2 + 1];
        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }
    }

    /* generic */
    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
    {
        sid = UNIT_RAM_SKILLS(unit)[i];
        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }
    }
}

struct SkillList * GetUnitSkillList(struct Unit * unit)
{
    struct SkillList * list = &sSkillList[0];    
    if (unit == &gBattleActor.unit)
        list = &sSkillList[1];
    else if (unit == &gBattleTarget.unit)
        list = &sSkillList[2];

    if (!JudgeUnitList(unit, &list->header))
    {
        GenerateSkillListExt(unit, list);
        WriteUnitList(unit, &list->header);
    }
    return list;
}

void ResetSkillLists(void)
{
    memset(&sSkillList, 0, sizeof(sSkillList));
}
