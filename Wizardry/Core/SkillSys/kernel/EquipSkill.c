#include "global.h"
#include "bmunit.h"

#include "common-chax.h"
#include "skill-system.h"
#include "prep-skill.h"

extern struct PrepEquipSkillList sPrepEquipSkillList;
#define sPrepEquipSkillListExt gGenericBuffer

void ResetPrepEquipSkillList(void)
{
    CpuFastFill16(0, sPrepEquipSkillListExt, 0x100);
    CpuFastFill16(0, &sPrepEquipSkillList,   sizeof(sPrepEquipSkillList));
}

STATIC_DECLAR void RegisterToPrepEquipSkillListExt(const u8 sid)
{
    sPrepEquipSkillListExt[sid] |= 1;
}

STATIC_DECLAR void SetupPrepEquipReal(void)
{
    int i;
    for (i = 1; i < MAX_SKILL_NUM; i++)
        if (sPrepEquipSkillListExt[i] & 1)
            sPrepEquipSkillList.sid[sPrepEquipSkillList.amt++] = i;
}

STATIC_DECLAR void UpdatePrepEquipSkillList(struct Unit * unit)
{
    int i;

    ResetPrepEquipSkillList();

    for (i = 1; i < MAX_SKILL_NUM; i++)
        if (IsSkillLearned(unit, i))
            RegisterToPrepEquipSkillListExt(i);

    SetupPrepEquipReal();
}

struct PrepEquipSkillList * GetPrepEquipSkillList(struct Unit * unit)
{
    if (!JudgeUnitListHeader(unit, &sPrepEquipSkillList.header))
    {
        UpdatePrepEquipSkillList(unit);
        WriteUnitListHeader(unit, &sPrepEquipSkillList.header);
    }
    return &sPrepEquipSkillList;
}
