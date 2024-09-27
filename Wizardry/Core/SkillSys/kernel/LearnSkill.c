#include "common-chax.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "skill-system.h"

struct LearnedSkillList {
	u32 data[8]; /* 8 * 32 = 0x100 */
};

extern struct LearnedSkillList sLearnedSkillPLists[NEW_BWL_ARRAY_NUM];

/* GameInitHook */
void ResetUnitLearnedSkillLists(void)
{
	CpuFill16(0, sLearnedSkillPLists, sizeof(sLearnedSkillPLists));
}

/* SaveData */
void SaveUnitLearnedSkillLists(u8 *dst, const u32 size)
{
	Assert(size == sizeof(sLearnedSkillPLists));

	WriteAndVerifySramFast(sLearnedSkillPLists, dst, size);
}

/* LoadData */
void LoadUnitLearnedSkillLists(u8 *src, const u32 size)
{
	Assert(size == sizeof(sLearnedSkillPLists));

	ReadSramFast(src, sLearnedSkillPLists, size);
}

bool IsSkillLearned(struct Unit *unit, const u16 sid)
{
	u8 pid = UNIT_CHAR_ID(unit);

	if (EQUIPE_SKILL_VALID(sid) && pid < NEW_BWL_ARRAY_NUM)
		return _BIT_CHK(sLearnedSkillPLists[pid].data, sid);

	return false;
}

void LearnSkill(struct Unit *unit, const u16 sid)
{
	u8 pid = UNIT_CHAR_ID(unit);

	/* Make sure that the enemy is not effective on allies */
	if (UNIT_FACTION(unit) != FACTION_BLUE)
		return;

	if (EQUIPE_SKILL_VALID(sid) && pid < NEW_BWL_ARRAY_NUM)
		_BIT_SET(sLearnedSkillPLists[pid].data, sid);
}

void ForgetSkill(struct Unit *unit, const u16 sid)
{
	u8 pid = UNIT_CHAR_ID(unit);

	if (EQUIPE_SKILL_VALID(sid) && pid < NEW_BWL_ARRAY_NUM)
		_BIT_CLR(sLearnedSkillPLists[pid].data, sid);
}
