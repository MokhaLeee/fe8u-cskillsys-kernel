#include "common-chax.h"
#include "skill-system.h"

/**
 * 0: generic use
 * 1: battle actor
 * 2: battle target
 */
extern struct SkillList sSkillList[3];
struct SkillList * const SkillListGeneric = &sSkillList[0];
struct SkillList * const SkillListBattleActor = &sSkillList[1];
struct SkillList * const SkillListBattleTarget = &sSkillList[2];

void GenerateSkillListExt(struct Unit * unit, struct SkillList * list)
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
    WriteUnitList(unit, &list->header);
}

#if 0
struct SkillList * GetUnitSkillList(struct Unit * unit)
{
    struct SkillList * list = SkillListGeneric;
    if (unit == &gBattleActor.unit)
        list = SkillListBattleActor;
    else if (unit == &gBattleTarget.unit)
        list = SkillListBattleTarget;

    if (!JudgeUnitList(unit, &list->header))
        GenerateSkillListExt(unit, list);

    return list;
}
#endif

void ResetSkillLists(void)
{
    memset(&sSkillList, 0, sizeof(sSkillList));
}
