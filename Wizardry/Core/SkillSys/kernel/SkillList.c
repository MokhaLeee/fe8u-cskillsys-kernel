#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "skill-system.h"

extern u32 sSkillListNext;

/**
 * 0 - 3: generic usage
 * 4: active unit
 * 5: battle actor
 * 6: battle target
 */
#define SKILL_LIST_AMT 7
#define SKILL_LIST_NEXT(idx) (((idx) - 1) & 3)
extern struct SkillList sSkillLists[SKILL_LIST_AMT];

STATIC_DECLAR void GenerateSkillListExt(struct Unit * unit, struct SkillList * list)
{
    int i;

    memset(list, 0, sizeof(*list));

    for (i = 1; i < MAX_SKILL_NUM; i++)
    {
        if (SkillTester(unit, i) == true)
        {
            list->sid[list->amt] = i;

            list->amt++;
            if (list->amt > SKILL_LIST_MAX_AMT)
                break;
        }
    }
    WriteUnitListHeader(unit, &list->header);
}

STATIC_DECLAR struct SkillList * GetExistingSkillList(struct Unit * unit)
{
    int i;
    for (i = SKILL_LIST_AMT - 1; i >= 0; i--)
    {
        if (JudgeUnitListHeader(unit, &sSkillLists[i].header))
        {
            if (i <= 3)
            {
                /* Generic list */
                sSkillListNext = SKILL_LIST_NEXT(i);
            }
            return &sSkillLists[i];
        }
    }
    return NULL;
}

struct SkillList * GetUnitSkillList(struct Unit * unit)
{
    struct SkillList * list = GetExistingSkillList(unit);

    if (!list)
    {
        if (unit->index == gActiveUnit->index)
            list = &sSkillLists[SKILL_LIST_AMT - 3];
        else if (unit->index == gBattleActor.unit.index)
            list = &sSkillLists[SKILL_LIST_AMT - 2];
        else if (unit->index == gBattleTarget.unit.index)
            list = &sSkillLists[SKILL_LIST_AMT - 1];
        else
        {
            list = &sSkillLists[sSkillListNext];
            sSkillListNext = SKILL_LIST_NEXT(sSkillListNext);
        }
        GenerateSkillListExt(unit, list);
    }
    return list;
}

void ResetSkillLists(void)
{
    CpuFill16(0, sSkillLists, sizeof(sSkillLists));
}

void DisableUnitSkills(struct Unit * unit)
{
    GetUnitSkillList(unit)->amt = 0;
}
