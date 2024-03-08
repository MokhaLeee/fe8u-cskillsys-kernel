#include "global.h"
#include "bmunit.h"
#include "agb_sram.h"

#include "common-chax.h"
#include "bwl.h"
#include "skill-system.h"

struct LearnedSkillList {
    u16 data[0x10];
};

extern struct LearnedSkillList sLearnedSkillPLists[NEW_BWL_ARRAY_NUM];

#ifdef CONFIG_USE_DEBUG
void * const Identifier_sLearnedSkillPLists = sLearnedSkillPLists;
#endif

/* GameInitHook */
void ResetUnitLearnedSkillLists(void)
{
    CpuFill16(0, sLearnedSkillPLists, sizeof(sLearnedSkillPLists));
}

/* SaveData */
void SaveUnitLearnedSkillLists(u8 * dst, const u32 size)
{
    size_t _size = size > sizeof(sLearnedSkillPLists) ? sizeof(sLearnedSkillPLists) : size;
    WriteAndVerifySramFast(sLearnedSkillPLists, dst, _size);
}

/* LoadData */
void LoadUnitLearnedSkillLists(u8 * src, const u32 size)
{
    size_t _size = size > sizeof(sLearnedSkillPLists) ? sizeof(sLearnedSkillPLists) : size;
    ReadSramFast(src, sLearnedSkillPLists, _size);
}

bool IsSkillLearned(struct Unit * unit, const u8 sid)
{
    u8 lo = (sid & 0x0F);
    u8 hi = (sid & 0xF0) >> 4;
    u8 pid = UNIT_CHAR_ID(unit);

    if (pid < NEW_BWL_ARRAY_NUM)
        return !!(sLearnedSkillPLists[pid].data[hi] & (1 << lo));

    return false;
}

void LearnSkill(struct Unit * unit, const u8 sid)
{
    u8 lo = (sid & 0x0F);
    u8 hi = (sid & 0xF0) >> 4;
    u8 pid = UNIT_CHAR_ID(unit);

    if (pid < NEW_BWL_ARRAY_NUM)
        sLearnedSkillPLists[pid].data[hi] |= 1 << lo;
}

void ForgetSkill(struct Unit * unit, const u8 sid)
{
    u8 lo = (sid & 0x0F);
    u8 hi = (sid & 0xF0) >> 4;
    u8 pid = UNIT_CHAR_ID(unit);

    if (pid < NEW_BWL_ARRAY_NUM)
        sLearnedSkillPLists[pid].data[hi] &= ~(1 << lo);
}
